#include "ServiceMonitor_QtBoost.h"
#include <QtWidgets/QApplication>
#include <QDir>

#include "ServiceConfig.h"
#include "ServiceFactory.h"
#include "CheckerFactory.h"

#include "TCPChecker.h"
#include "PingChecker.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	// Регистрация типов сервисов
    ServiceFactory::instance()->registerService("http",
        [](const nlohmann::json& json) { return QSharedPointer<HttpService>::create(json); });

    ServiceFactory::instance()->registerService("tcp",
        [](const nlohmann::json& json) { return QSharedPointer<TcpService>::create(json); });

    ServiceFactory::instance()->registerService("ping",
        [](const nlohmann::json& json) { return QSharedPointer<PingService>::create(json); });

	// Регистрация типов чекеров
    CheckerFactory::instance()->registerChecker("tcp", []() -> QSharedPointer<AbstractChecker> 
        {
            return QSharedPointer<TCPChecker>::create();
        });

    CheckerFactory::instance()->registerChecker("ping", []() -> QSharedPointer<AbstractChecker>
        {
            return QSharedPointer<PingChecker>::create();
        });

    ServiceConfig* serviceConfig = new ServiceConfig(QDir(QCoreApplication::applicationDirPath()).filePath("Configs/Config.json"));
    QVector<QSharedPointer<AbstractService>> servises = serviceConfig->getServises();
    for (auto& service : servises)
    {
        try
        {
            QSharedPointer<AbstractChecker> checker = CheckerFactory::instance()->getOrCreate(service->getServiceType().toString());
            checker->addService(service);
        }
        catch (const std::exception& e)
        {
            // EMPTY
        }
    }

    for (auto& checker : CheckerFactory::instance()->getCheckers())
    {
        checker->start();
        checker->check();
    }

    ServiceMonitor_QtBoost window;
    window.show();
    return app.exec();
}
