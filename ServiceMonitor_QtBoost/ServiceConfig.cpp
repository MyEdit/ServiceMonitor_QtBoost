#include "ServiceConfig.h"

ServiceConfig::ServiceConfig(const QString& pathToConfig)
{
    this->pathToConfig = pathToConfig;
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

    if (!jsonConfig.contains("services") || !jsonConfig["services"].is_array())
    {
        throw ServiceMonitorException("Неправильная конфигурация: отсутствует массив 'services'");
    }

    services.clear();

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

        }
        catch (const std::exception& e)
        {
            continue;
        }
    }
}