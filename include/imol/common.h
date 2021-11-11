#ifndef COMMON_H
#define COMMON_H

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
}

/**
 * @brief VersionManager is extremly useful for finer defined versions, the version saved in
 * a CreatorManager instance, and can be obtained by means of CreatorManager methods
 * @return global version manager
 */
CORESHARED_EXPORT imol::VersionManager & ver();
CORESHARED_EXPORT int ver(const QString &key, bool sum = false);

#define IMOL_VER_REG(KEY, VER) IMOL_AUTO_RUN(ver().regist(KEY, [] (void) -> int {return VER;}))

#endif // COMMON_H
