#pragma once

#include "AbstractService.h"

class TcpService : public AbstractService
{
public:
	TcpService(const nlohmann::json& json);
	virtual ~TcpService() = default;

	int getPort() const;

protected:
	int port;

	virtual std::pair<bool, QString> isValid() const override;
};

