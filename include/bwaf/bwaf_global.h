#ifndef BWAF_GLOBAL_H
#define BWAF_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BWAF_LIBRARY)
#  define BWAFSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BWAFSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BWAF_GLOBAL_H
