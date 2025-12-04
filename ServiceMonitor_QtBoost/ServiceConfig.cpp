#include "ServiceConfig.h"

ServiceConfig::ServiceConfig(QObject* parent) : QObject(parent)
{

}

ServiceConfig* ServiceConfig::instance()
{
    if (m_instance.isNull())
    {
        if (m_instance.isNull())
        {
            m_instance.reset(new ServiceConfig());
        }
    }

    return m_instance.data();
}