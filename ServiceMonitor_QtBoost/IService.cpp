#include "IService.h"

IService::IService(const nlohmann::json& json)
{
	this->name = QString::fromStdString(json.value("name", EMPTY_STRING));
	this->host = QString::fromStdString(json.value("host", EMPTY_STRING));
	this->type = stringToServiceType(QString::fromStdString(json.value("host", EMPTY_STRING)));
}

bool IService::isValid() const
{
    std::pair<bool, QString> result;

    if (this->name.isEmpty())
    {
        Logger::instance().warning("Имя сервиса некорректено");
        return false;
    }
    
    if (this->host.isEmpty())
    {
        Logger::instance().warning("Адрес сервиса некорректен");
        return false;
    }

    if (this->type == ServiceType::UNKNOWN)
    {
        Logger::instance().warning("Тип сервиса некорректен");
        return false;
    }

    return true;
}

IService::ServiceType IService::stringToServiceType(const QString& str)
{
    static QMap<QString, ServiceType> mapping =
    {
        { "http",   ServiceType::HTTP},
        { "tcp",    ServiceType::TCP },
        { "ping",   ServiceType::PING},
    };

    return mapping.value(str.toLower(), ServiceType::UNKNOWN);
}