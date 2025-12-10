#include "ServiceMonitor_QtBoost.h"
#include <QtWidgets/QApplication>
#include <QDir>

#include "ServiceConfig.h"
#include "ServiceFactory.h"

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

    ServiceConfig* serviceConfig = new ServiceConfig(QDir(QCoreApplication::applicationDirPath()).filePath("Configs/Config.json"));

    ServiceMonitor_QtBoost window;
    window.show();
    return app.exec();
}
