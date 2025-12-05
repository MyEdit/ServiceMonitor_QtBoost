#pragma once

#include <string>

#include <Qtclasshelpermacros>
#include <QScopedPointer>
#include <QString>
#include <QDateTime>
#include <QMutex>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions/formatters/date_time.hpp>
#include <boost/log/support/date_time.hpp>

namespace boostLogger = boost::log;
namespace boostSinks = boost::log::sinks;
namespace boostKeywords = boost::log::keywords;
namespace boostExpression = boost::log::expressions;

using LogLevel = boost::log::trivial::severity_level;

// Чутка переопределим под наши цели 
#define BOOST_LOG_TRIVIAL(lvl)\
    BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
        (::boost::log::keywords::severity = lvl))

class Logger
{
	Q_DISABLE_COPY(Logger)

public:
	static Logger& instance(); // Юзаем Мейер-синглтон
	~Logger() = default;

	void printLog(const std::string_view& message, LogLevel logLevel) const;
	void debug(const std::string_view& message) const;
	void info(const std::string_view& message) const;
	void warning(const std::string_view& message) const;
	void error(const std::string_view& message) const;
	void fatal(const std::string_view& message) const;

private:
	mutable QMutex mutex;

	explicit Logger();
};

