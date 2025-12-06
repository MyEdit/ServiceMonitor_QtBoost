#include "AbstractService.h"

AbstractService::AbstractService(const nlohmann::json& json)
{
	this->name = QString::fromStdString(json.value("name", EMPTY_STRING));
	this->host = QString::fromStdString(json.value("host", EMPTY_STRING));
	this->type = stringToServiceType(QString::fromStdString(json.value("type", EMPTY_STRING)));

    if (auto [ok, message] = this->isValid(); !ok)
    {
        throw std::runtime_error(message.toStdString());
    }
}

std::pair<bool, QString> AbstractService::isValid() const
{
    if (this->name.isEmpty())
    {
        return std::make_pair(false, "Имя сервиса некорректно");
    }
    
    if (this->host.isEmpty())
    {
        return std::make_pair(false, "Адрес сервиса некорректен");
    }

    if (this->type == ServiceType::UNKNOWN)
    {
        return std::make_pair(false, "Тип сервиса некорректен");
    }

    return std::make_pair(true, EMPTY_STRING);
}

AbstractService::ServiceType AbstractService::stringToServiceType(const QString& str)
{
    static QMap<QString, ServiceType> mapping =
    {
        { "http",   ServiceType::HTTP },
        { "tcp",    ServiceType::TCP  },
        { "ping",   ServiceType::PING },
    };

    return mapping.value(str.trimmed().toLower(), ServiceType::UNKNOWN);
}

QStringView AbstractService::getName() const
{
    return this->name;
}

QStringView AbstractService::getHost() const
{
    return this->host;
}

AbstractService::ServiceType AbstractService::getServiceType() const
{
    return this->type;
}