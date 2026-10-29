#ifndef IMOL_COMMON_H
#define IMOL_COMMON_H

#include "core_global.h"

#include <QJsonValue>

#include "logmanager.h"
#include "modulemanager.h"
#include "translationmanager.h"
#include "statemanager.h"
#include "creatormanager.h"

namespace imol {
//version control
typedef CreatorManager<int, int(void)> VersionManager;
CORESHARED_EXPORT imol::VersionManager & versionManager();
CORESHARED_EXPORT QString releaseVersionString(const QString &key);
}

CORESHARED_EXPORT imol::VersionManager & ver(); // obosolette
CORESHARED_EXPORT int ver(const QString &key, bool sum = false);

#define IMOL_VER_REG(KEY, VER) IMOL_AUTO_RUN(imol::versionManager().regist(KEY, [] (void) -> int { return VER; }))

#define IMOL_RELEASE_VER_REG(KEY, MAJOR, MINOR, BASELINE) IMOL_AUTO_RUN( \
imol::versionManager().regist(QString("@0_%1").arg(KEY), [] (void) -> int { return BASELINE; }); \
imol::versionManager().regist(QString("@1_%1").arg(KEY), [] (void) -> int { return MAJOR; }); \
imol::versionManager().regist(QString("@2_%1").arg(KEY), [] (void) -> int { return MINOR; }))

#endif // IMOL_COMMON_H
