#include "common.h"

IMOL_VER_REG("imol.core", 26)

imol::VersionManager &ver()
{
    static imol::VersionManager mgr;
    return mgr;
}

int sumVer(imol::ModuleObject *key_mobj)
{
    int v = ver(key_mobj->fullName(ver().keyMobj()));
    foreach (imol::ModuleObject *sub_key_mobj, key_mobj->cmobjs()) {
        v += sumVer(sub_key_mobj);
    }
    return v;
}

int ver(const QString &key, bool sum)
{
    return sum ? sumVer(ver().keyMobj()->r(key)) : ver().create(key);
}
