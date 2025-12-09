#include "JsonConfig.h"

JsonConfig::JsonConfig(const QString& pathToConfig)
{
	this->pathToConfig = pathToConfig;
}

void JsonConfig::createDefaultConfig() const
{
    QDir configDir = QFileInfo(pathToConfig).absoluteDir();

    if (!configDir.exists())
    {
        Logger::instance()->info("Создание директории: " + configDir.absolutePath().toStdString());
        if (!configDir.mkpath("."))
        {
            throw ServiceMonitorException("Не удалось создать директорию для конфигурации");
        }
    }

    nlohmann::json json = getDefaultJson();
    Logger::instance()->info("Запись конфигурации в файл");

    std::ofstream outputFileStream(this->pathToConfig.toStdString());

    if (!outputFileStream.is_open())
    {
        throw ServiceMonitorException("Не удалось открыть файл для записи: " + this->pathToConfig.toStdString());
    }

    outputFileStream << std::setw(4) << json << std::endl;
    outputFileStream.close();

    if (outputFileStream.fail())
    {
        throw ServiceMonitorException("Ошибка при записи конфигурации");
    }

    Logger::instance()->info("Конфигурация успешно создана");
}

void JsonConfig::loadConfig()
{
    QFileInfo configFileInfo(pathToConfig);

    if (!configFileInfo.exists())
    {
        Logger::instance()->info("Файл конфигурации не найден, создание конфигурации по умолчанию");
        createDefaultConfig();
    }
}