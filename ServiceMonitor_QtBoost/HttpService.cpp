#include "HttpService.h"

HttpService::HttpService(const nlohmann::json& json)
	: AbstractService(json)
{
	this->port = json.value("port", EMPTY_INT);
	this->path = QString::fromStdString(json.value("path", EMPTY_STRING));
	this->expectedStatus = json.value("expected_status", EMPTY_INT);
}

std::pair<bool, QString> HttpService::isValid() const
{
	if (this->port == EMPTY_INT)
	{
		return std::make_pair(false, QString("Порт сервиса %1 некорректен").arg(this->name));
	}

	if (this->path.isEmpty())
	{
		return std::make_pair(false, QString("Путь сервиса %1 некорректен").arg(this->name));
	}

	if (this->expectedStatus == EMPTY_INT)
	{
		return std::make_pair(false, QString("Ожидаемый статус сервиса %1 некорректен").arg(this->name));
	}

	return { true, EMPTY_STRING };
}

quint32 HttpService::getPort() const
{
	return this->port;
}

QStringView HttpService::getPath() const
{
	return this->path;
}

quint32 HttpService::getExpectedStatus() const
{
	return this->expectedStatus;
}