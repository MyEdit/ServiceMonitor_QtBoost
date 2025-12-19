#include "TCPChecker.h"

TCPChecker::TCPChecker()
    : AbstractChecker(),
    ioWorkGuard(asio::make_work_guard(ioContext)) // Сразу кидаем в контекст фейковую работу
{
    // TODO: Потом, когда сделаю еще пару чекеров на HTTP и т.п посмотреть что можно еще вынести в абстрактный класс
}

TCPChecker::~TCPChecker()
{
    stop();
}

void TCPChecker::start()
{
    if (running.exchange(true))
        return;

    ioThread = std::thread([this]()
        {
            try
            {
                Logger::instance()->info("IO TCP поток запущен");
                ioContext.run();
                Logger::instance()->info("IO TCP поток остановлен");
            }
            catch (const std::exception& e)
            {
                Logger::instance()->error(QString("IO TCP поток выбросил ошибку: %1").arg(e.what()).toStdString());
            }
        });

    // Можно запустить периодический цикл через asio::steady_timer, но для простоты - оставляем checkNow() вызываемым извне.
}

void TCPChecker::stop()
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
        Logger::instance()->error(std::string("TCPChecker::stop: ") + e.what());
    }
}

void TCPChecker::addService(const QSharedPointer<AbstractService>& service)
{
    QSharedPointer<TcpService> tcpService = qSharedPointerDynamicCast<TcpService>(service);
    if (!tcpService)
        return;

    asio::post(ioContext, [this, tcpService]()
        {
            try
            {
                QSharedPointer<ServiceConnection> serviceConnection = QSharedPointer<ServiceConnection>::create
                (
                    tcpService,
                	tcp::socket(ioContext),
                	tcp::endpoint(asio::ip::make_address(tcpService->getHost().toString().toStdString()), tcpService->getPort()),
                	std::chrono::steady_clock::time_point(),
                	std::chrono::steady_clock::time_point()
                );

                std::lock_guard<std::mutex> lock(servicesMutex);
                serviceConnections.push_back(serviceConnection);

            }
            catch (const std::exception& e)
            {
                Logger::instance()->error(std::string("TCPChecker::addService error: ") + e.what());
            }
        });
}

void TCPChecker::removeService(const QString& serviceName)
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

void TCPChecker::check()
{
    asio::post(ioContext, [this]()
        {
            QVector<QSharedPointer<ServiceConnection>> copyServiceConnections;
            {
                std::lock_guard<std::mutex> lock(servicesMutex);
                copyServiceConnections = serviceConnections;
            }

            for (auto& serviceConnection : copyServiceConnections)
            {
                auto start = std::chrono::steady_clock::now();

                serviceConnection->socket.async_connect(serviceConnection->endpoint,
                    [this, serviceConnection, start](const boost::system::error_code& ec)
                    {
                        bool success = !ec;
                        qint64 latency = -1;
                        if (success)
                        {
                            auto end = std::chrono::steady_clock::now();
                            latency = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                        }

                        if (success)
                        {
                            Logger::instance()->info(QString("TCPChecker: %1 connected").arg(serviceConnection->service->getName().toString()).toStdString());
                        }
                        else
                        {
                            Logger::instance()->info(QString("TCPChecker: %1 not connect (%2)").arg(serviceConnection->service->getName().toString(), QString::fromStdString(ec.message())).toStdString());
                        }

                        if (serviceConnection->socket.is_open())
                        {
                        	serviceConnection->socket.close();
                        }
                    });
            }
        });
}