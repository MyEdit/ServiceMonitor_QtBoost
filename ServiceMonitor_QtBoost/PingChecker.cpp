#include "PingChecker.h"

PingChecker::PingChecker()
    : AbstractChecker(),
    ioWorkGuard(asio::make_work_guard(ioContext)) // Сразу кидаем в контекст фейковую работу
{
    // TODO: Потом, когда сделаю еще пару чекеров на HTTP и т.п посмотреть что можно еще вынести в абстрактный класс
}

PingChecker::~PingChecker()
{
    stop();
}

void PingChecker::start()
{
    if (running.exchange(true))
        return;

    ioThread = std::thread([this]()
        {
            try
            {
                Logger::instance()->info("IO ICMP поток запущен");
                ioContext.run();
                Logger::instance()->info("IO ICMP поток остановлен");
            }
            catch (const std::exception& e)
            {
                Logger::instance()->error(QString("IO ICMP поток выбросил ошибку: %1").arg(e.what()).toStdString());
            }
        });
}

void PingChecker::stop()
{
    if (!running.exchange(false))
        return;

    try
    {
        ioWorkGuard.reset();
        ioContext.stop();
        if (ioThread.joinable())
        {
            ioThread.join();
        }
    }
    catch (const std::exception& e)
    {
        Logger::instance()->error(std::string("PingChecker::stop: ") + e.what());
    }
}

void PingChecker::addService(const QSharedPointer<AbstractService>& service)
{
    QSharedPointer<PingService> pingService = qSharedPointerDynamicCast<PingService>(service);
    if (!pingService)
        return;

    asio::post(ioContext, [this, pingService]()
        {
            try
            {
                QSharedPointer<ServiceConnection> serviceConnection = QSharedPointer<ServiceConnection>::create
                (
                    pingService,
                    icmp::socket(ioContext),
                    icmp::endpoint(asio::ip::make_address(pingService->getHost().toString().toStdString()), 0),
                    std::chrono::steady_clock::time_point(),
                    std::chrono::steady_clock::time_point()
                );

                std::lock_guard<std::mutex> lock(servicesMutex);
                serviceConnections.push_back(serviceConnection);

            }
            catch (const std::exception& e)
            {
                Logger::instance()->error(std::string("PingChecker::addService error: ") + e.what());
            }
        });
}

void PingChecker::removeService(const QString& serviceName)
{
    asio::post(ioContext, [this, serviceName]()
        {
            std::lock_guard<std::mutex> lock(servicesMutex);
            serviceConnections.erase(std::remove_if(serviceConnections.begin(), serviceConnections.end(),
                [&serviceName](const QSharedPointer<ServiceConnection>& serviceConnection)
                {
                    return serviceConnection->service && serviceConnection->service->getName().toString() == serviceName;
                }), serviceConnections.end());
        });
}

void PingChecker::check()
{
    asio::post(ioContext, [this]()
        {
            QVector<QSharedPointer<ServiceConnection>> copy;
            {
                std::lock_guard<std::mutex> lg(servicesMutex);
                copy = serviceConnections;
            }

            static std::atomic<uint16_t> sequenceCounter{ 0 };

#if defined(_WIN32)
            auto get_identifier = []() -> uint16_t { return static_cast<uint16_t>(::GetCurrentProcessId()); };
#else
            auto get_identifier = []() -> uint16_t { return static_cast<uint16_t>(::getpid()); };
#endif

            for (auto& serviceConnection : copy)
            {
                try
                {
                    if (!serviceConnection->socket.is_open())
                        serviceConnection->socket.open(icmp::v4());
                }
                catch (const std::exception& e)
                {
                    Logger::instance()->error(std::string("PingChecker::check open socket error: ") + e.what());
                    continue;
                }

                const std::string body = "ServiceMonitorPing";
                auto request_buffer = std::make_shared<asio::streambuf>();
                uint16_t id = get_identifier();
                uint16_t seq = ++sequenceCounter;
                {
                    std::ostream os(request_buffer.get());
                    icmp_header echo_req;
                    echo_req.type(icmp_header::echo_request);
                    echo_req.code(0);
                    echo_req.identifier(id);
                    echo_req.sequence_number(seq);
                    compute_checksum(echo_req, body.begin(), body.end());
                    os << echo_req << body;
                }

                auto reply_buffer = std::make_shared<asio::streambuf>();
                auto timer = std::make_shared<asio::steady_timer>(ioContext);
                serviceConnection->startTime = std::chrono::steady_clock::now();
                int timeout_ms = serviceConnection->service->getTimeout();

                timer->expires_after(std::chrono::milliseconds(timeout_ms));
                timer->async_wait([this, serviceConnection, timer](const boost::system::error_code& ec)
                    {
                        if (ec)
                            return;

                        Logger::instance()->info(QString("PingChecker: %1 timeout")
                            .arg(serviceConnection->service->getName().toString()).toStdString());
                        
                        closeSocket(serviceConnection->socket);
                    });

                serviceConnection->socket.async_send_to(request_buffer->data(), serviceConnection->endpoint,
                    [this, serviceConnection, request_buffer, reply_buffer, timer, id, seq](const boost::system::error_code& send_ec, std::size_t)
                    {
                        if (send_ec)
                        {
                            Logger::instance()->info(QString("PingChecker: %1 not connect (%2)")
                                .arg(serviceConnection->service->getName().toString(), QString::fromStdString(send_ec.message())).toStdString());
                            
                            closeSocket(serviceConnection->socket);
                            return;
                        }

                        serviceConnection->socket.async_receive(
                            boost::asio::buffer(reply_buffer->prepare(65536)),
                            [serviceConnection, reply_buffer, timer, id, seq, this](const boost::system::error_code& ec, std::size_t length)
                            {
                                handle_icmp_receive(
                                    serviceConnection,
                                    reply_buffer,
                                    timer,
                                    id,
                                    seq,
                                    ec,
                                    length
                                );
                            }
                        );
                    });
            }
        });
}

void PingChecker::handle_icmp_receive(
    QSharedPointer<PingChecker::ServiceConnection> serviceConnection,
    std::shared_ptr<boost::asio::streambuf> reply_buffer,
    std::shared_ptr<boost::asio::steady_timer> timer,
    uint16_t identifier,
    uint16_t sequence,
    const boost::system::error_code& ec,
    std::size_t length)
{
    timer->cancel();

    if (ec)
    {
        if (ec != boost::asio::error::operation_aborted)
        {
            Logger::instance()->info(QString("PingChecker: %1 not connect (%2)")
                .arg(serviceConnection->service->getName().toString(), QString::fromStdString(ec.message())).toStdString());
        }

        closeSocket(serviceConnection->socket);
        return;
    }

    reply_buffer->commit(length);
    std::istream is(reply_buffer.get());
    ipv4_header ip_hdr;
    icmp_header icmp_hdr;
    is >> ip_hdr >> icmp_hdr;

    if (is && icmp_hdr.type() == icmp_header::echo_reply
        && icmp_hdr.identifier() == identifier
        && icmp_hdr.sequence_number() == sequence)
    {
        serviceConnection->endTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(serviceConnection->endTime - serviceConnection->startTime).count();

        Logger::instance()->info(QString("PingChecker: %1 connected, latency %2 ms")
            .arg(serviceConnection->service->getName().toString()).arg(QString::number(elapsed)).toStdString());
    }
    else
    {
        Logger::instance()->info(QString("PingChecker: %1 not connect (no matching ICMP reply)")
            .arg(serviceConnection->service->getName().toString()).toStdString());
    }

    closeSocket(serviceConnection->socket);
}

void PingChecker::closeSocket(icmp::socket& socket)
{
    if (socket.is_open())
    {
        socket.close();
    }
}