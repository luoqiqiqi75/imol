#ifndef PLUGIN_H
#define PLUGIN_H

#define IMOL_API 10

#include <QObject>
#include <QVariant>

#define IMOL_PLUGIN_FUNC imolPlugin
#define IMOL_PLUGIN_FUNC_DEF(Define) void Define(QObject *info)

namespace imol {
typedef IMOL_PLUGIN_FUNC_DEF((*pluginFunc));
}

#define IMOL_PLUGIN(Export, Name) \
extern "C" { \
inline Export IMOL_PLUGIN_FUNC_DEF(IMOL_PLUGIN_FUNC) { \
info->setProperty("api", IMOL_API); \
info->setProperty("name", Name); \
} \
}

#endif // PLUGIN_H
