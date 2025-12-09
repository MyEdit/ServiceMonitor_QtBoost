#pragma once

#include <QString>
#include <fstream>
#include <json.hpp>

#include "IConfig.h"
#include "ServiceMonitorException.h"
#include "Logger.h"

class JsonConfig : public IConfig
{
public:
	JsonConfig(const QString& pathToConfig);
	virtual ~JsonConfig() = default;

protected:
	QString pathToConfig;

	virtual nlohmann::json getDefaultJson() const = 0;
	virtual void parse() = 0;
	virtual void createDefaultConfig() const override;
	void loadConfig();
};

