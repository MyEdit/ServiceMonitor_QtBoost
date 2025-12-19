#include "ServiceFactory.h"

ServiceFactory* ServiceFactory::instance()
{
    static ServiceFactory instance;
    return &instance;
}

void ServiceFactory::registerService(const QString& type, Creator creator)
{
    Logger::instance()->info(QString("Зарегистрирована фабрика сервиса для типа: %1").arg(type).toStdString());
    creators[type.trimmed().toLower()] = creator;
}

QSharedPointer<AbstractService> ServiceFactory::create(const nlohmann::json& json)
{
    QString type = QString::fromStdString(json.value("type", "")).trimmed().toLower();

    if (!creators.contains(type))
        throw ServiceMonitorException("ServiceFactory: Неизвестный тип сервиса: " + type.toStdString());

    return creators[type](json);
}
