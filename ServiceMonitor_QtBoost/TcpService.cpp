#include "TcpService.h"

TcpService::TcpService(const nlohmann::json& json)
	: AbstractService(json)
{
	this->port = json.value("port", EMPTY_INT);

	if (auto [ok, message] = this->isValid(); !ok)
	{
		throw ServiceMonitorException(message.toStdString());
	}
}

std::pair<bool, QString> TcpService::isValid() const
{
	if (this->port == EMPTY_INT)
	{
		return std::make_pair(false, QString("Порт сервиса %1 некорректен").arg(this->name));
	}

	return { true, EMPTY_STRING };
}

int TcpService::getPort() const
{
	return this->port;
}
