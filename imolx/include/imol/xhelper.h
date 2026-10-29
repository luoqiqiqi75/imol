#ifndef IMOL_XHELPER_H
#define IMOL_XHELPER_H

#include "core/common.h"
#include "bwaf/bmodule.h"

#include "xnode.h"

#ifndef MODULE_NAME
#define MODULE_NAME "imolx"
#endif

/**
 * basic functions
 */

namespace ix {

// imol util
typedef bwaf::BModule Module;
IMOLX_EXPORT Node * createNodeAt(Node *root, const QString &rpath, bool *found = nullptr);
IMOLX_EXPORT Node * moveNodes(const QList<Node *> &nodes, Node *new_root);

IMOLX_EXPORT Node * globalNode(const QString &key);
template<typename T> inline T globalNodeAs(const QString &key) { return globalNode(key)->get().value<T>(); }


template<typename T>
struct GlobalUniquePtr
{
    const QString key;
    T *ptr;

    T * get() { return ptr; }
    const T * operator ->() const { return ptr; }
    void reset(T *global_ptr, bool auto_delete = true)
    { if (auto_delete) delete globalNodeAs<T *>(key); globalNode(key)->set(nullptr, QVariant::fromValue(ptr = global_ptr)); }

    GlobalUniquePtr(const QString &key) : key(key) { reset(nullptr); }
    GlobalUniquePtr(const QString &key, T *ptr) : key(key) { reset(ptr); }
    ~GlobalUniquePtr() { if (globalNodeAs<T *>(key) == ptr) reset(nullptr, false); }
};

// path util
IMOLX_EXPORT QString templatePath(const QString &key, const QString &alias = MODULE_NAME);
IMOLX_EXPORT QString imagePath(const QString &key, const QString &suffix = "png", const QString &alias = MODULE_NAME);
IMOLX_EXPORT QString assetPath(const QString &key, const QString &suffix = "png", const QString &alias = MODULE_NAME);
IMOLX_EXPORT QString cachePath(const QString &key, const QString &suffix = "json", const QString &alias = MODULE_NAME);

enum LanguageType
{
    Translation_qm,
    Translation_json
};
IMOLX_EXPORT QString languagePath(const QString &key, LanguageType type = Translation_qm, const QString &alias = MODULE_NAME);

}

/**
 *  xNav Module
 */

namespace ix {
namespace xnav {

/**
 * @brief registerPage will insert a page node with specific structure at recursive page menu
 *
 * @c nav   - navigation path
 * @c key   - searching key, seperated by 'c'
 * @c icon  - icon path
 * @c label - (automatically) page visable name from @p lbl
 * @c page  - (automatically) sub pages
 *
 * @example registerPage(this, "Dummy Label", "topmenu1.submenu2")->set(this, "nav", "module.dummy_page");
 */
IMOLX_EXPORT Node * registerPage(QObject *context, const QString &lbl, const QString &menu = "", const QString &page_name = "", int index = -1);

IMOLX_EXPORT bool switchTo(QObject *context, const QString &nav, bool creatable = true);

}
}

/**
 * xTheme Module
 */

class QLayout;
class QPainter;
class QTreeWidget;
class QTreeWidgetItem;
namespace ix {
namespace xtheme {

IMOLX_EXPORT qreal zoom();

IMOLX_EXPORT QFont font(const QString &key);
IMOLX_EXPORT QColor color(const QString &key);
IMOLX_EXPORT QPixmap pixmap(const QString &key);
IMOLX_EXPORT QPixmap pixmap(const QString &key, const QColor &to_color);
IMOLX_EXPORT QPixmap pixmap(const QString &key, const QColor &to_color, const QColor &from_color);
IMOLX_EXPORT QPixmap pixmap(const QPixmap &pixmap, const QColor &to_color);

IMOLX_EXPORT QString stylizeQss(const QString &styled_qss);
IMOLX_EXPORT QString stylizeQssFile(const QString &styled_qss_path);

}

// other theme util
IMOLX_EXPORT QFont xn2Font(const XNode &xn);

IMOLX_EXPORT QPoint xn2Point(const XNode &xn);
IMOLX_EXPORT QPointF xn2PointF(const XNode &xn);

IMOLX_EXPORT QSize xn2Size(const XNode &xn);
IMOLX_EXPORT QSizeF xn2SizeF(const XNode &xn);

IMOLX_EXPORT QRect xn2Rect(const XNode &xn);
IMOLX_EXPORT QRectF xn2RectF(const XNode &xn);

IMOLX_EXPORT QBrush xn2Brush(const XNode &xn);
IMOLX_EXPORT QPen xn2Pen(const XNode &xn);
IMOLX_EXPORT QPainter * xn2Painter(const XNode &xn, QPainter *painter);

IMOLX_EXPORT QTreeWidgetItem * mobj2TreeItem(QTreeWidget *tree_wgt, imol::ModuleObject *mobj, int max_level);
IMOLX_EXPORT imol::ModuleObject * mobjOfTreeItem(QTreeWidgetItem *item, imol::ModuleObject *relative_mobj);
IMOLX_EXPORT QTreeWidgetItem * treeItemOfMobj(QTreeWidgetItem *relative_item, imol::ModuleObject *mobj, imol::ModuleObject *relative_mobj);
IMOLX_EXPORT QTreeWidgetItem * treeItemOfMobj(QTreeWidget *relative_wgt, imol::ModuleObject *mobj, imol::ModuleObject *relative_mobj);

}

/**
 * xSupport Module
 */

#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#define XSUPPORT_QT4
#endif
#include "mobjhandler.h"

namespace ix {
namespace format {

template<typename T> bool fromString(const QString &, T &);
template<typename T> QString toString(const T &);
template<typename T> inline static T fromString(const QString &str) { T t; fromString(str, t); return t; }

template<typename T> bool fromNode(Node *, T &);
template<typename T> Node * toNode(const T &, Node *);
template<typename T> inline static T fromNode(Node *n) { T t; fromNode(n, t); return t; }

}

namespace grammar {

IMOLX_EXPORT QString compile(Node *language, Node *err = nullptr);
IMOLX_EXPORT bool decompile(const QString &statement, Node *language, Node *err = nullptr);

}

namespace calc {

IMOLX_EXPORT Node * rot2Quaternion(Node *rot, Node *dst = nullptr);
IMOLX_EXPORT Node * quaternion2Rot(Node *quaternion, Node *dst = nullptr);
IMOLX_EXPORT Node * transformFrame(Node *l_frame, Node *r_frame, Node *dst = nullptr);
IMOLX_EXPORT Node * inverseFrame(Node *frame, Node *dst = nullptr);
IMOLX_EXPORT Node * calibrateOXY(Node *origin_frame, Node *x_frame, Node *y_frame, Node *dst = nullptr);

}
}

/**
 * Qt util
 */

class QComboBox;
namespace ix {
namespace util {

IMOLX_EXPORT QByteArray loadFileContent(const QString &path);

IMOLX_EXPORT void clearQLayout(QLayout *l);

IMOLX_EXPORT void updateQComboBox(QComboBox *cmb, const QStringList &items);

IMOLX_EXPORT void showMenuOnTop(QWidget *ctrl_wgt, QWidget *menu_wgt, int d = 6);
IMOLX_EXPORT void showMenuOnRight(QWidget *ctrl_wgt, QWidget *menu_wgt, int d = 6);
IMOLX_EXPORT void showMenuOnBottom(QWidget *ctrl_wgt, QWidget *menu_wgt, int d = 1);

}
}

#endif // IMOL_XHELPER_H
