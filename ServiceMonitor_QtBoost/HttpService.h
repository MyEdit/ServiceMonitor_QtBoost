#pragma once

#include "AbstractService.h"

class HttpService : public AbstractService
{
public:
	HttpService(const nlohmann::json& json);
	virtual ~HttpService() = default;

	quint32 getPort() const;
	QStringView getPath() const;
	quint32 getExpectedStatus() const;

protected:
	quint32 port;
	QString path;
	quint32 expectedStatus;

	virtual std::pair<bool, QString> isValid() const override;
};

