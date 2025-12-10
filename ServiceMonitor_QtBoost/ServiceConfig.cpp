#include "ServiceConfig.h"

ServiceConfig::ServiceConfig(const QString& pathToConfig)
    : JsonConfig(pathToConfig)
{
    JsonConfig::loadConfig();
    this->parse();
}

void ServiceConfig::parse()
{
    std::ifstream fileStream(this->pathToConfig.toStdString());

    if (!fileStream.is_open())
    {
        throw ServiceMonitorException("Не удалось открыть конфиг: " + this->pathToConfig.toStdString());
    }

    nlohmann::json jsonConfig = nlohmann::json::parse(fileStream);
    fileStream.close();

    if (!jsonConfig.contains("services") || !jsonConfig["services"].is_array())
    {
        throw ServiceMonitorException("Неправильная конфигурация: отсутствует массив 'services'");
    }

    services.clear();
    Logger::instance()->info("======================");

    for (const auto& serviceJson : jsonConfig["services"])
    {
        try
        {
            QSharedPointer<AbstractService> service = ServiceFactory::instance()->create(serviceJson);

            Logger::instance()->info(service->getName().toString().toStdString());
            Logger::instance()->info(service->getHost().toString().toStdString());
            Logger::instance()->info(service->getServiceType().toString().toStdString());

            if (HttpService* httpService = dynamic_cast<HttpService*>(service.get()))
            {
                Logger::instance()->info(QString::number(httpService->getPort()).toStdString());
                Logger::instance()->info(httpService->getPath().toString().toStdString());
                Logger::instance()->info(QString::number(httpService->getExpectedStatus()).toStdString());
			}
            else if (TcpService* tcpService = dynamic_cast<TcpService*>(service.get()))
            {
                Logger::instance()->info(QString::number(tcpService->getPort()).toStdString());
            }
            else if (PingService* pingService = dynamic_cast<PingService*>(service.get()))
            {
                Logger::instance()->info(QString::number(pingService->getTimeout()).toStdString());
            }

            Logger::instance()->info("======================");
        }
        catch (const std::exception& e)
        {
            continue;
        }
    }
}

nlohmann::json ServiceConfig::getDefaultJson() const
{
    nlohmann::json json;

    json["services"] =
    {
        {
            {"name", "Google HTTP"},
            {"type", "http"},
            {"host", "google.com"},
            {"port", 80},
            {"path", "/"},
            {"expected_status", 200},
        },
        {
            {"name", "Local SSH"},
            {"type", "tcp"},
            {"host", "localhost"},
            {"port", 80},
        },
        {
            {"name", "Cloudflare DNS"},
            {"type", "ping"},
            {"host", "1.1.1.1"},
            {"timeout_ms", 2000},
        },
        {
            {"name", "Custom API"},
            {"type", "http"},
            {"host", "api.example.com"},
            {"port", 443},
            {"path", "/health"},
            {"expected_status", 200},
        },
    };

    return json;
}