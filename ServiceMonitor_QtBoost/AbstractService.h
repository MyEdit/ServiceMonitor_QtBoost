#pragma once

#include <QString>
#include <QMap>

#include <json.hpp>

#include "ServiceMonitorException.h"

class AbstractService
{
public:
	AbstractService(const nlohmann::json& json);
	virtual ~AbstractService() = default;

	virtual QStringView getName() const;
	virtual QStringView getHost() const;
	virtual QStringView getServiceType() const;

protected:
	static const constexpr auto EMPTY_STRING = "";
	static const constexpr auto EMPTY_INT = -1;
	QString name;
	QString host;
	QString type;

	virtual std::pair<bool, QString> isValid() const;

private:
	std::pair<bool, QString> isValidBase() const;
};