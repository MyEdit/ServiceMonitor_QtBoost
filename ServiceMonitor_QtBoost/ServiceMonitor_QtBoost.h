#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ServiceMonitor_QtBoost.h"

class ServiceMonitor_QtBoost : public QMainWindow
{
    Q_OBJECT

public:
    ServiceMonitor_QtBoost(QWidget *parent = nullptr);
    ~ServiceMonitor_QtBoost();

private:
    Ui::ServiceMonitor_QtBoostClass ui;
};

