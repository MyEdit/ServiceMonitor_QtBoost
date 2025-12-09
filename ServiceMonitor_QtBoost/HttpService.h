#pragma once

#include "AbstractService.h"

class HttpService : public AbstractService
{
public:
	HttpService(const nlohmann::json& json);
	virtual ~HttpService() = default;

	int getPort() const;
	int getExpectedStatus() const;
	QStringView getPath() const;

protected:
	int port;
	int expectedStatus;
	QString path;

	virtual std::pair<bool, QString> isValid() const override;
};

