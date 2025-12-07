#pragma once

#include <fstream>

#include <Qtclasshelpermacros>
#include <QSharedPointer>
#include <QVector>

#include "Logger.h"
#include "AbstractService.h"
#include "ServiceMonitorException.h"
#include "ServiceFactory.h"
#include "HttpService.h"

class ServiceConfig
{
	Q_DISABLE_COPY(ServiceConfig)

public:
    explicit ServiceConfig(const QString& pathToConfig);
	virtual ~ServiceConfig() = default;

private:
    void parse();

	QVector<QSharedPointer<AbstractService>> services;
    QString pathToConfig;
};

// Примерный Config.json
/*
{
    "services" :
    [
        {
          "name": "Google HTTP",            // AbstractService
          "type" : "http",                  // AbstractService
          "host" : "google.com",            // AbstractService
          "port" : 80,                      // HttpService
          "path" : "/",                     // HttpService
          "expected_status" : 200           // HttpService
        },
        {
          "name": "Local SSH",              // AbstractService
          "type" : "tcp",                   // AbstractService
          "host" : "localhost",             // AbstractService
          "port" : 22                       // TcpService
        },
        {
          "name": "Cloudflare DNS",         // AbstractService
          "type" : "ping",                  // AbstractService
          "host" : "1.1.1.1",               // AbstractService
          "timeout_ms" : 2000               // PingService
        },
        {
          "name": "Custom API",             // AbstractService
          "type" : "http",                  // AbstractService
          "host" : "api.example.com",       // AbstractService
          "port" : 443,                     // HttpService
          "path" : "/health",               // HttpService
          "expected_status" : 200           // HttpService
        }
    ]
}
*/