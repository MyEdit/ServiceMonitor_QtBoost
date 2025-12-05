#pragma once

#include <Qtclasshelpermacros>
#include <QScopedPointer>
#include <QVector>

class ServiceConfig
{
	Q_DISABLE_COPY(ServiceConfig)

public:
	static ServiceConfig* instance();
	~ServiceConfig() = default;

private:
	explicit ServiceConfig();

	inline static QScopedPointer<ServiceConfig> m_instance;
	inline static QVector<int> services;
};

