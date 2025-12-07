#pragma once

#include <functional>

#include <QMap>
#include <QSharedPointer>

#include "AbstractService.h"
#include "ServiceMonitorException.h"

class ServiceFactory
{
public:
    using Creator = std::function<QSharedPointer<AbstractService>(const nlohmann::json&)>;

    static ServiceFactory* instance();

    void registerService(const QString& type, Creator creator);
    QSharedPointer<AbstractService> create(const nlohmann::json& json);

private:
    QMap<QString, Creator> creators;
};
