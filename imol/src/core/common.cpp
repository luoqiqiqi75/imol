#include "common.h"

IMOL_VER_REG("imol.core", 27)

int sumVer(imol::ModuleObject *key_mobj)
{
    int v = ver(key_mobj->fullName(imol::versionManager().keyMobj()));
    foreach (imol::ModuleObject *sub_key_mobj, key_mobj->cmobjs()) {
        v += sumVer(sub_key_mobj);
    }
    return v;
}

imol::VersionManager & ver() { return imol::versionManager(); }

int ver(const QString &key, bool sum)
{
    return sum ? sumVer(imol::versionManager().keyMobj()->r(key)) : imol::versionManager().create(key);
}

namespace imol {

imol::VersionManager & versionManager()
{
    static imol::VersionManager mgr;
    return mgr;
}

QString releaseVersionString(const QString &key)
{
    int baseline = versionManager().create(QString("@0_%1").arg(key));
    int major = versionManager().create(QString("@1_%1").arg(key));
    int minor = versionManager().create(QString("@2_%1").arg(key));
    QStringList ver_str_list;
    for (int i = 3; i < 10; i++) {
        QString sub_ver_key = QString("@%1_%2").arg(i).arg(key);
        if (!versionManager().keyMobj(sub_ver_key)->isEmptyMobj()) ver_str_list.append(QString::number(versionManager().create(sub_ver_key)));
    }

    QString ver_str = QString("v%1.%2.%3").arg(major).arg(minor).arg(ver(key, true) - baseline);
    if (ver_str_list.size() > 0) ver_str.append(".").append(ver_str_list.join("."));
    return ver_str;
}

}
