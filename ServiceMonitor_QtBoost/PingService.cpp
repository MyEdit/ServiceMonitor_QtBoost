#include "PingService.h"

PingService::PingService(const nlohmann::json& json)
	: AbstractService(json)
{
	this->timeout = json.value("timeout_ms", EMPTY_INT);

	if (auto [ok, message] = this->isValid(); !ok)
	{
		throw ServiceMonitorException(message.toStdString());
	}
}

std::pair<bool, QString> PingService::isValid() const
{
	if (this->timeout == EMPTY_INT)
	{
		return std::make_pair(false, QString("Время ожидания сервиса %1 некорректено").arg(this->name));
	}

	return { true, EMPTY_STRING };
}

int PingService::getTimeout() const
{
	return this->timeout;
}
