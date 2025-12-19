#pragma once

#include <functional>

#include <QMap>
#include <QSharedPointer>

#include "AbstractService.h"
#include "AbstractChecker.h"
#include "ServiceMonitorException.h"

class CheckerFactory
{
public:
    using Creator = std::function<QSharedPointer<AbstractChecker>()>;

    static CheckerFactory* instance();

    void registerChecker(const QString& type, Creator creator);
    QSharedPointer<AbstractChecker> create(const QString& type);

private:
    QMap<QString, Creator> creators;
    QMap<QString, QSharedPointer<AbstractChecker>> instances;
};
