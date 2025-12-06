#include "ServiceMonitor_QtBoost.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ServiceMonitor_QtBoost window;
    window.show();
    return app.exec();
}
