#pragma once

#include <QString>
#include <QMap>

#include <json.hpp>

class AbstractService
{
	enum class ServiceType
	{
		HTTP,
		TCP,
		PING,
		UNKNOWN,
	};

public:
	AbstractService(const nlohmann::json& json);
	virtual ~AbstractService() = default;

	QStringView getName() const;
	QStringView getHost() const;
	ServiceType getServiceType() const;

private:
	static const constexpr auto EMPTY_STRING = "";
	QString name;
	QString host;
	ServiceType type;

	virtual std::pair<bool, QString> isValid() const;
	ServiceType stringToServiceType(const QString& str);
};