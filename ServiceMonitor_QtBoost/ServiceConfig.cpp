#include "ServiceConfig.h"

ServiceConfig::ServiceConfig(/*TODO: Путь до конфига*/)
{

}

ServiceConfig* ServiceConfig::instance()
{
    static ServiceConfig instance;
    return &instance;
}