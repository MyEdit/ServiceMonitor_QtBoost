#pragma once

#include <QString>
#include <QMap>

#include <json.hpp>

#include "Logger.h"

class IService
{
	enum class ServiceType
	{
		HTTP,
		TCP,
		PING,
		UNKNOWN,
	};

public:
	IService(const nlohmann::json& json);
	virtual ~IService() = default;

	virtual bool isValid() const;

private:
	static const constexpr auto EMPTY_STRING = "";
	QString name;
	QString host;
	ServiceType type;

	ServiceType stringToServiceType(const QString& str);
};

