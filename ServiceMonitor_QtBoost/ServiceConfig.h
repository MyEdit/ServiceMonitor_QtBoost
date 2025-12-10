#pragma once

#include <Qtclasshelpermacros>
#include <QSharedPointer>
#include <QVector>

#include "JsonConfig.h"
#include "ServiceMonitorException.h"
#include "ServiceFactory.h"
#include "HttpService.h"
#include "TcpService.h"
#include "PingService.h"

class ServiceConfig : public JsonConfig
{
	Q_DISABLE_COPY(ServiceConfig)

public:
    explicit ServiceConfig(const QString& pathToConfig);
	virtual ~ServiceConfig() = default;

private:
	QVector<QSharedPointer<AbstractService>> services;

protected:
    nlohmann::json getDefaultJson() const;
    void parse() override;
};