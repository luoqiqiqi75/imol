#include "bfactory.h"

#include "../core/common.h"

IMOL_VER_REG("imol.bwaf.bfactory", 1)

bwaf::BFactory & bfactory()
{
    static bwaf::BFactory mgr;
    return mgr;
}

namespace imol {

PluginManager & pluginManager()
{
    static PluginManager mgr;
    return mgr;
}

}
