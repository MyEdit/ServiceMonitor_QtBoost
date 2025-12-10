#pragma once

#include "AbstractService.h"

class PingService : public AbstractService
{
public:
	PingService(const nlohmann::json& json);
	virtual ~PingService() = default;

	int getTimeout() const;

protected:
	int timeout;

	virtual std::pair<bool, QString> isValid() const override;
};

