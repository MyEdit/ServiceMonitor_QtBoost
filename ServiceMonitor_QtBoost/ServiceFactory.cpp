#include "ServiceFactory.h"

ServiceFactory* ServiceFactory::instance()
{
    static ServiceFactory instance;
    return &instance;
}

void ServiceFactory::registerService(const QString& type, Creator creator)
{
    creators[type.toLower()] = creator;
}

QSharedPointer<AbstractService> ServiceFactory::create(const nlohmann::json& json)
{
    QString type = QString::fromStdString(json.value("type", "")).trimmed().toLower();

    if (!creators.contains(type))
        throw ServiceMonitorException("Неизвестный тип сервиса: " + type.toStdString());

    return creators[type](json);
}
