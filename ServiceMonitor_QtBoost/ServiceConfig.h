#pragma once

#include <Qtclasshelpermacros>
#include <QSharedPointer>
#include <QVector>

#include "AbstractService.h"
#include "Logger.h"

class ServiceConfig
{
	Q_DISABLE_COPY(ServiceConfig)

public:
	static ServiceConfig* instance(); // Юзаем Мейер-синглтон
	~ServiceConfig() = default;

private:
	explicit ServiceConfig(/*TODO: Путь до конфига*/);

	inline static QVector<QSharedPointer<AbstractService>> services;
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