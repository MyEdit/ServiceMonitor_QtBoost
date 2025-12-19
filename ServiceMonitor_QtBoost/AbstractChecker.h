#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#include <QSharedPointer>

#include "AbstractService.h"

class AbstractChecker
{
public:
    explicit AbstractChecker() {};
    ~AbstractChecker() = default;

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void check() = 0;

    virtual void addService(const QSharedPointer<AbstractService>& service) = 0;
    virtual void removeService(const QString& serviceName) = 0;
};

