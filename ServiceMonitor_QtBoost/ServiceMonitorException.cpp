#include "ServiceMonitorException.h"

ServiceMonitorException::ServiceMonitorException(const std::string& message, bool autoLog /*= true*/)
    : std::runtime_error(message)
{
    if (autoLog)
    {
        Logger::instance()->fatal(message);
    }
}