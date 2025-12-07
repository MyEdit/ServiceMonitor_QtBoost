#include "AbstractService.h"

AbstractService::AbstractService(const nlohmann::json& json)
{
	this->name = QString::fromStdString(json.value("name", EMPTY_STRING));
	this->host = QString::fromStdString(json.value("host", EMPTY_STRING));
	this->type = QString::fromStdString(json.value("type", EMPTY_STRING));

    if (auto [ok, message] = this->isValid(); !ok)
    {
        throw ServiceMonitorException(message.toStdString());
    }
}

std::pair<bool, QString> AbstractService::isValidBase() const
{
    if (this->name.isEmpty())
    {
        return std::make_pair(false, "Имя сервиса некорректно");
    }
    
    if (this->host.isEmpty())
    {
        return std::make_pair(false, QString("Адрес сервиса %1 некорректен").arg(this->name));
    }

    if (this->type.isEmpty())
    {
        return std::make_pair(false, QString("Тип сервиса %1 некорректен").arg(this->name));
    }

    return this->isValid();
}

std::pair<bool, QString> AbstractService::isValid() const
{
    return { true, EMPTY_STRING };
}

QStringView AbstractService::getName() const
{
    return this->name;
}

QStringView AbstractService::getHost() const
{
    return this->host;
}

QStringView AbstractService::getServiceType() const
{
    return this->type;
}