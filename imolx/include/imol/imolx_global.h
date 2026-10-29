#ifndef IMOLX_GLOBAL_H
#define IMOLX_GLOBAL_H

#include "core/core_global.h"

#if defined(IMOLX_LIBRARY)
#  define IMOLX_EXPORT Q_DECL_EXPORT
#else
#  define IMOLX_EXPORT Q_DECL_IMPORT
#endif

#define IX_MEMBER_0(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this)
#define IX_MEMBER_1(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1)
#define IX_MEMBER_2(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1, std::placeholders::_2)
#define IX_MEMBER_3(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define IX_DECLARE_D \
protected: \
    struct Data; \
    Data *d;

#define IX_DECLARE_P \
protected: \
    class Private; \
    Private *p;

namespace imol { class ModuleObject; }

#ifndef NULL
#define NULL 0
#endif

#endif // IMOLX_GLOBAL_H
