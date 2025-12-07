#pragma once

#include <stdexcept>
#include <string>
#include "Logger.h"

class ServiceMonitorException : public std::runtime_error
{
public:
    ServiceMonitorException(const std::string& message, bool autoLog = true);
};

