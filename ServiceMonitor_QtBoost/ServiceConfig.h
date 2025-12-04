#pragma once

#include <QObject.h>

class ServiceConfig : public QObject
{
	Q_OBJECT
	Q_DISABLE_COPY(ServiceConfig)

public:
	static ServiceConfig* instance();
	~ServiceConfig() override = default;

private:
	explicit ServiceConfig(QObject* parent = nullptr);

	inline static QScopedPointer<ServiceConfig> m_instance;
};

