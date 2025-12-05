#include "ServiceConfig.h"

ServiceConfig::ServiceConfig()
{

}

ServiceConfig* ServiceConfig::instance()
{
    if (m_instance.isNull())
    {
        m_instance.reset(new ServiceConfig());
    }

    return m_instance.data();
}