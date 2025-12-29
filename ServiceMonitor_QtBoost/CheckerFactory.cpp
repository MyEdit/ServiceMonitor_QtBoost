#include "CheckerFactory.h"

CheckerFactory* CheckerFactory::instance()
{
    static CheckerFactory instance;
    return &instance;
}

void CheckerFactory::registerChecker(const QString& type, Creator creator)
{
    Logger::instance()->info(QString("Зарегистрирована фабрика чекера для типа: %1").arg(type).toStdString());
    creators[type.trimmed().toLower()] = std::move(creator);
}

QSharedPointer<AbstractChecker> CheckerFactory::getOrCreate(const QString& type)
{
    QString key = type.trimmed().toLower();
    if (instances.contains(key))
        return instances[key];

    if (!creators.contains(key))
        throw ServiceMonitorException("CheckerFactory: Неизвестный тип сервиса: " + key.toStdString());

    QSharedPointer<AbstractChecker> inst = creators[key]();
    if (!inst)
        throw ServiceMonitorException("CheckerFactory: Сreator вернул null для типа: " + key.toStdString());

    instances[key] = inst;
    return inst;
}

QVector<QSharedPointer<AbstractChecker>> CheckerFactory::getCheckers() const
{
    return instances.values();
}

