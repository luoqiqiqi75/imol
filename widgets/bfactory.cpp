#include "bfactory.h"

#include "../core/common.h"

IMOL_VER_REG("imol.bwaf.bfactory", 1)

bwaf::BFactory & bfactory()
{
    return IMOL_CREATOR(bwaf::BModule *, (QObject *));
}
