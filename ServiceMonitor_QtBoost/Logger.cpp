#include "Logger.h"

Logger::Logger()
{
    auto logFormat =
        (
            boostExpression::stream
            << boostExpression::format_date_time<boost::posix_time::ptime>("TimeStamp", "[%H:%M:%S.%f]")
            << " [" << boostLogger::trivial::severity << "]: "
            << boostExpression::smessage
        );

    boostLogger::add_common_attributes();
    boostLogger::register_simple_formatter_factory<LogLevel, char>("Severity");
    boostLogger::add_file_log
        (
            boostKeywords::target = "logs/", boostKeywords::file_name = "%d-%m-%y-%3N.log",
            boostKeywords::rotation_size = 10 * 1024 * 1024,
            boostKeywords::scan_method = boostSinks::file::scan_matching,
            boostKeywords::format = logFormat
        );

    boostLogger::add_console_log(std::cout, boostKeywords::format = logFormat);
    boostLogger::core::get()->set_filter(boostLogger::trivial::severity >= boostLogger::trivial::info);
}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::printLog(const std::string_view& message, LogLevel logLevel) const
{
    BOOST_LOG_TRIVIAL(logLevel) << message;

    // Qt ломает std::cout, так что для вывода в консоль будем принтовать и форматировать вручную, надеюсь временно пока не найду инфу 
    QMutexLocker locker(&mutex);
    QDateTime now = QDateTime::currentDateTime();
    QString timestamp = now.toString("[HH:mm:ss.zzz]");
    
    QString levelStr;
    switch (logLevel)
    {
        case boostLogger::trivial::debug:    levelStr = "Debug";    break;
        case boostLogger::trivial::info:     levelStr = "Info";     break;
        case boostLogger::trivial::warning:  levelStr = "Warning";  break;
        case boostLogger::trivial::error:    levelStr = "Error";    break;
        case boostLogger::trivial::fatal:    levelStr = "Fatal";    break;
        default:                             levelStr = "Unknown";  break;
    }
    
    QString formatted = QString("%1 [%2]: %3")
        .arg(timestamp)
        .arg(levelStr)
        .arg(QString::fromUtf8(message));
    
    qDebug() << formatted;
}

void Logger::debug(const std::string_view& message) const
{
    printLog(message, LogLevel::debug);
}

void Logger::info(const std::string_view& message) const
{
    printLog(message, LogLevel::info);
}

void Logger::warning(const std::string_view& message) const
{
    printLog(message, LogLevel::warning);
}

void Logger::error(const std::string_view& message) const
{
    printLog(message, LogLevel::error);
}

void Logger::fatal(const std::string_view& message) const
{
    printLog(message, LogLevel::fatal);
}