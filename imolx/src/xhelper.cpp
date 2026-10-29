#include "xhelper.h"

#include <QCoreApplication>
#include <QColor>
#include <QFont>
#include <QPointF>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QTreeWidget>

#include "xsupport.h"
#include "xtheme.h"

#define NAME "xhelper"

IMOL_VER_REG(mName(MODULE_NAME, NAME), 9)

namespace ix {

Node * createNodeAt(Node *root, const QString &rpath, bool *found)
{
    Node *tar = root->r(rpath);
    if (tar->isEmptyMobj()) {
        root->insert(root, rpath);
        tar = root->r(rpath);
        if (found) *found = false;
    } else {
        if (found) *found = true;
    }
    return tar;
}

Node * moveNodes(const QList<Node *> &nodes, Node *new_root)
{
    foreach (auto *n, nodes) {
        n->p()->remove(new_root, n, false);
        new_root->insert(new_root, n);
    }
    return new_root;
}

Node * globalNode(const QString &key) { static ix::Node *n = createNodeAt(m("imol"), "global"); return createNodeAt(n, key); }

QString resourcePath(const QString &item, QString alias, QString key, const QString &suffix)
{
    alias.replace(IMOL_MODULE_NAME_SEPARATOR, "/");
    key.replace(IMOL_MODULE_NAME_SEPARATOR, "/");
    return QString(":/%1/%2/%3.%4").arg(alias, item, key, suffix);
}

QString localPath(const QString &item, QString alias, QString key, const QString &suffix)
{
    alias.replace(IMOL_MODULE_NAME_SEPARATOR, "/");
    key.replace(IMOL_MODULE_NAME_SEPARATOR, "/");
    return QString("./%1/%2/%3.%4").arg(item, alias, key, suffix);
}

QString templatePath(const QString &key, const QString &alias)
{ return resourcePath("template", alias, key, "json"); }
QString imagePath(const QString &key, const QString &suffix, const QString &alias)
{ return resourcePath("images", alias, key, suffix); }
QString assetPath(const QString &key, const QString &suffix, const QString &alias)
{ return localPath("asset", alias, key, suffix); }
QString cachePath(const QString &key, const QString &suffix, const QString &alias)
{ return localPath("cache", alias, key, suffix); }
QString languagePath(const QString &key, LanguageType type, const QString &alias)
{ return resourcePath("language", alias, key, type == Translation_qm ? "qm" : "json"); }

}

namespace ix {
namespace xnav {

Node * pageNodeOfMenu(const QString &menu)
{
    Node *page_node = m("xnav.page");
    if (!menu.isEmpty()) {
        QString rpath = menu;
        if (rpath.contains(".page.")) rpath.replace(".page.", "."); // support
        rpath.replace(IMOL_MODULE_NAME_SEPARATOR, ".page.");
        page_node = createNodeAt(page_node, mName(rpath, "page"));
    }
    return page_node;
}

Node * registerPage(QObject *context, const QString &lbl, const QString &menu, const QString &page_name, int index)
{
    Node *page_node = pageNodeOfMenu(menu);
    QString new_page_key = page_name.isEmpty() ? Node::generateId(page_node) : page_name;
    Node *new_page_node = page_node->c(new_page_key);
    if (new_page_node->isEmptyMobj()) {
        new_page_node = m().create(new_page_key);
        page_node->insert(context, new_page_node, false, index);
    } else {
        WLOG << "<xnav> register an existing page: " << page_name << " at: " << index;
    }
    return new_page_node->set(context, "label", lbl);
}

bool switchTo(QObject *context, const QString &nav, bool creatable)
{
    if (!creatable && !m("imol.ui.centerwidget.item")->hasRmobj(nav)) return false;
    m("xnav._current")->set(context, nav);
    return true;
}

}
}

namespace ix {
namespace xtheme {

qreal zoom() { return m("xtheme.zoom")->getDouble(1); }

auto defaultFontGetter = [] (const QString &key) {
    QFont f;
    f.fromString(key);
    return f;
};

QFont font(const QString &key)
{
    auto bm = qobject_cast<XTheme *>(bModule("xtheme"));
    return bm ? bm->asset<QFont>("font", key, defaultFontGetter) : defaultFontGetter(key);
}

auto defaultColorGetter = [] (const QString &key) { return QColor(key); };

QColor color(const QString &key)
{
    auto bm = qobject_cast<XTheme *>(bModule("xtheme"));
    return bm ? bm->asset<QColor>("color", key, defaultColorGetter) : defaultColorGetter(key);
}

auto defaultPixmapGetter = [] (const QString &key) {
    QString path = key;
    path.replace(IMOL_MODULE_NAME_SEPARATOR, "/");
    return QPixmap(QString("asset/%1.png").arg(path));
};

QPixmap pixmap(const QString &key)
{
    auto bm = qobject_cast<XTheme *>(bModule("xtheme"));
    return bm ? bm->asset<QPixmap>("pixmap", key, defaultPixmapGetter) : defaultPixmapGetter(key);
}

QPixmap pixmap(const QString &key, const QColor &to_color)
{ return pixmap(pixmap(key), to_color); }
QPixmap pixmap(const QString &key, const QColor &to_color, const QColor &from_color)
{ return XTheme::recolorPixmap(pixmap(key), from_color, to_color); }
QPixmap pixmap(const QPixmap &pixmap, const QColor &to_color)
{ return XTheme::recolorPixmap(pixmap, QColor(), to_color); }

QString stylizeQss(const QString &styled_qss)
{
    auto bm = qobject_cast<XTheme *>(bModule("xtheme"));
    return bm ? bm->constructQss(styled_qss) : styled_qss;
}

QString stylizeQssFile(const QString &styled_qss_path)
{
    QFile f(styled_qss_path);
    if (!f.open(QIODevice::ReadOnly)) return "";
    QString styled_qss = QString::fromUtf8(f.readAll());
    f.close();
    return stylizeQss(styled_qss);
}

}

QFont xn2Font(const XNode &xn)
{
    QFont f(xn.at("family").getString());
    f.setPixelSize(xn.at("size").getInt(-1));
    return f;
}

QPoint xn2Point(const XNode &xn) { return QPoint(xn.at("x").getInt(), xn.at("y").getInt()); }
QPointF xn2PointF(const XNode &xn) { return QPointF(xn.at("x").getDouble(), xn.at("y").getDouble()); }

QSize xn2Size(const XNode &xn) { return QSize(xn.at("w").getInt(), xn.at("h").getInt()); }
QSizeF xn2SizeF(const XNode &xn) { return QSizeF(xn.at("w").getDouble(), xn.at("h").getDouble()); }

QRect xn2Rect(const XNode &xn) { return QRect(xn2Point(xn), xn2Size(xn)); }
QRectF xn2RectF(const XNode &xn) { return QRectF(xn2PointF(xn), xn2SizeF(xn)); }

QBrush xn2Brush(const XNode &xn)
{
    QString color_str = xn.getString();
    return color_str.isEmpty() ? Qt::NoBrush : QBrush(xtheme::color(color_str));
}

QPen xn2Pen(const XNode &xn)
{
    if (xn.empty()) return Qt::NoPen;
    QPen pen(xtheme::color(xn.getString()));
    if (xn.has("color")) pen.setColor(xtheme::color(xn.at("color").getString()));
    if (xn.has("width")) pen.setWidthF(xn.at("width").getDouble());
    if (xn.has("brush")) pen.setBrush(xn2Brush(xn.at("brush")));
    return pen;
}

QPainter * xn2Painter(const XNode &xn, QPainter *painter)
{
    if (xn.has("pen")) painter->setPen(xn2Pen(xn.at("pen")));
    if (xn.has("brush")) painter->setBrush(xn2Brush(xn.at("brush")));
    if (xn.has("font")) painter->setFont(xn2Font(xn.at("font")));
    return painter;
}

QTreeWidgetItem * mobj2TreeItem(QTreeWidget *tree_wgt, imol::ModuleObject *mobj, int max_level)
{
    QString name = mobj->rname();
    int index = mobj->indexOfRname(name);
    if (index >= 0) name = QString::number(index + 1);

    QString value = mobj->getString();

    QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << name << value);
    item->setData(0, Qt::UserRole, mobj->name());
    item->setData(1, Qt::UserRole, mobj->get());

    //    tree_wgt->setItemWidget(item, 1, new QLineEdit(value, tree_wgt));

    if (max_level == 0) return item;

    foreach (imol::ModuleObject *cmobj, mobj->cmobjs(true)) {
        QCoreApplication::processEvents(QEventLoop::AllEvents); // keep calm
        item->addChild(mobj2TreeItem(tree_wgt, cmobj, max_level - 1));
    }
    return item;
}

imol::ModuleObject * mobjOfTreeItem(QTreeWidgetItem *item, imol::ModuleObject *relative_mobj)
{
    QStringList keys;
    do {
        keys.append(item->data(0, Qt::UserRole).toString());
        item = item->parent();
    } while (item);

    imol::ModuleObject *mobj = relative_mobj;
    while (mobj && !mobj->isEmptyMobj() && keys.size() > 0) {
        mobj = mobj->c(keys.takeLast(), true);
    }
    return mobj;
}

QTreeWidgetItem * treeItemOfMobj(QTreeWidgetItem *relative_item, imol::ModuleObject *mobj, imol::ModuleObject *relative_mobj)
{
    QList<int> indexes;
    while (!mobj->isEmptyMobj() && mobj != relative_mobj) {
        indexes.append(mobj->indexInPmobj());
        mobj = mobj->p();
    }

    QTreeWidgetItem *item = relative_item;
    while (item && indexes.size() > 0) {
        item = item->child(indexes.takeLast());
    }
    return item;
}

QTreeWidgetItem * treeItemOfMobj(QTreeWidget *relative_wgt, imol::ModuleObject *mobj, imol::ModuleObject *relative_mobj)
{
    QList<int> indexes;
    while (!mobj->isEmptyMobj() && mobj != relative_mobj) {
        indexes.append(mobj->indexInPmobj());
        mobj = mobj->p();
    }

    if (indexes.size() == 0) return nullptr;

    QTreeWidgetItem *item = relative_wgt->topLevelItem(indexes.takeLast());
    while (item && indexes.size() > 0) {
        item = item->child(indexes.takeLast());
    }
    return item;
}

}

#include "support/basesupporter.h"

namespace ix {
namespace grammar {

QString compile(Node *language, Node *err)
{
    QString statement;
    if (supporter->interpreter->compile(language, statement)) {
        if (err) err->clear(supporter);
    } else {
        if (err) err->set(supporter, supporter->interpreter->lastError());
    }
    return statement;
}

bool decompile(const QString &statement, Node *language, Node *err)
{
    if (supporter->interpreter->decompile(statement, language)) {
        if (err) err->clear(supporter);
        return true;
    } else {
        if (err) err->set(supporter, supporter->interpreter->lastError());
        return false;
    }
}

}
}

#include "src/support/formatter.h"

namespace ix {
namespace calc {

Node * rot2Quaternion(Node *rot, Node *dst)
{
    if (!dst) dst = rot;
    robot::Rotation r(rot->c(0)->getDouble(), robot::Vector3d::UnitX());
    robot::Rotation p(rot->c(1)->getDouble(), robot::Vector3d::UnitY());
    robot::Rotation y(rot->c(2)->getDouble(), robot::Vector3d::UnitZ());
    robot::Quaternion q = y * p * r;
    format::toNode(q, dst);
    return dst;
}

Node * quaternion2Rot(Node *quaternion, Node *dst)
{
    if (!dst) dst = quaternion;
    auto q = format::fromNode<robot::Quaternion>(quaternion);
//    q.normalize();
    robot::Vector3d rot = q.toRotationMatrix().eulerAngles(2, 1, 0);
    qSwap(rot.x(), rot.z());
    format::toNode(rot, dst);
    while (dst->cmobjCount() > 3) dst->remove(supporter, dst->last());
    return dst;
}

Node * transformFrame(Node *l_frame, Node *r_frame, Node *dst)
{
    if (!dst) dst = l_frame;
//    lM * rM, lM * rp + lp
    auto lf = format::fromNode<robot::Frame>(l_frame);
    auto rf = format::fromNode<robot::Frame>(r_frame);
    return format::toNode(lf * rf, dst);
}

Node * inverseFrame(Node *frame, Node *dst)
{
    if (!dst) dst = frame;
    auto f = format::fromNode<robot::Frame>(frame);
    return format::toNode(f.inverse(), dst);
}

Node * calibrateOXY(Node *origin_frame, Node *x_frame, Node *y_frame, Node *dst)
{
    if (!dst) dst = origin_frame;
    auto of = format::fromNode<robot::Frame>(origin_frame);
    auto xf = format::fromNode<robot::Frame>(x_frame);
    auto yf = format::fromNode<robot::Frame>(y_frame);
    robot::Vector3d ot = of.translation();
    robot::Vector3d xt = xf.translation();
    robot::Vector3d yt = yf.translation();
    robot::Vector3d ox = xt - ot;
    robot::Vector3d oy = yt - ot;
    robot::Vector3d oz = ox.cross(oy);
    oy = oz.cross(ox);
    ox.normalize();
    oy.normalize();
    oz.normalize();
    Eigen::Matrix3d r;
    r << ox[0], oy[0], oz[0],
        ox[1], oy[1], oz[1],
        ox[2], oy[2], oz[2];
    robot::Vector3d rot = r.eulerAngles(2, 1, 0);
    qSwap(rot.x(), rot.z());
    format::toNode(rot, createNodeAt(dst, "rot"));
    format::toNode(ot, createNodeAt(dst, "trans"));
    return dst;
}

}
}

#include <QComboBox>
#include <QLayout>
#include <QLineEdit>

namespace ix {
namespace util {

QByteArray loadFileContent(const QString &path)
{
    QByteArray bytes;
    if (path.isEmpty()) return bytes;
    QFile f(path);
    if (f.exists() && f.open(QIODevice::ReadOnly)) {
        bytes = f.readAll();
        f.close();
    }
    return bytes;
}

void clearQLayout(QLayout *l)
{
    while (QLayoutItem *layout_item = l->itemAt(0)) {
        l->removeItem(layout_item);
        if (layout_item->widget()) delete layout_item->widget();
        delete layout_item;
    }
}

void updateQComboBox(QComboBox *cmb, const QStringList &items)
{
#define UPDATE_QCOMBOBOX_OPERATION\
    cmb->hidePopup(); \
    cmb->clear(); \
    cmb->addItems(items)

    if (cmb->lineEdit()) {
        QString cur_text = cmb->lineEdit()->text();
        UPDATE_QCOMBOBOX_OPERATION;
        cmb->lineEdit()->setText(cur_text);
    } else {
        int cur_index = cmb->currentIndex();
        UPDATE_QCOMBOBOX_OPERATION;
        cmb->setCurrentIndex(cur_index);
    }
}

void raiseWidgetAtDxy(QWidget *wgt, int dx, int dy)
{
    wgt->show();
    //    wgt->raise();
    wgt->move(dx, dy);
}

void showMenuOnTop(QWidget *ctrl_wgt, QWidget *menu_wgt, int d)
{
    int dx = ctrl_wgt->mapToGlobal(QPoint(0, 0)).x();
    int dy = ctrl_wgt->mapToGlobal(QPoint(0, 0)).y() - menu_wgt->height() - d;
    raiseWidgetAtDxy(menu_wgt, dx, dy);
}

void showMenuOnRight(QWidget *ctrl_wgt, QWidget *menu_wgt, int d)
{
    int dx = ctrl_wgt->mapToGlobal(QPoint(ctrl_wgt->width(), 0)).x() + d;
    int dy = ctrl_wgt->mapToGlobal(QPoint(0, 0)).y() /*+ ctrl_wgt->height() / 2 - menu_wgt->height() / 2 + theme()->getSize(d)*/;
    raiseWidgetAtDxy(menu_wgt, dx, dy);
}

void showMenuOnBottom(QWidget *ctrl_wgt, QWidget *menu_wgt, int d)
{
    int dx = ctrl_wgt->mapToGlobal(QPoint(0, 0)).x() + d;
    int dy = ctrl_wgt->mapToGlobal(QPoint(0, 0)).y() + ctrl_wgt->height() + d;
    raiseWidgetAtDxy(menu_wgt, dx, dy);
}

}
}
