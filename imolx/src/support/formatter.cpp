#include "formatter.h"

#include "xhelper.h"

///
/// basic
///

#include <QPoint>
#include <QPointF>
#include <QSize>
#include <QSizeF>
#include <QTransform>
#include <QVector2D>
#include <QVector3D>
#include <QQuaternion>

namespace ix {
namespace format {

template<>
IMOLX_EXPORT bool fromString(const QString &str, bool &b)
{
    if (str.compare("true", Qt::CaseInsensitive) == 0) {
        b = true;
    } else if (str.compare("false", Qt::CaseInsensitive) == 0) {
        b = false;
    } else {
        b = false;
        return false;
    }
    return true;
}

template<>
IMOLX_EXPORT QString toString(const bool &b)
{
    return b ? "true" : "false";
}

template<>
IMOLX_EXPORT bool fromString(const QString &str, int &i)
{
    bool ok = false;
    i = str.toInt(&ok);
    return ok;
}

template<>
IMOLX_EXPORT QString toString(const int &i)
{
    return QString::number(i);
}

template<>
IMOLX_EXPORT bool fromString(const QString &str, double &d)
{
    bool ok = false;
    d = str.toDouble(&ok);
    return ok;
}

template<>
IMOLX_EXPORT QString toString(const double &d)
{
    QString str = QString::number(d, 'f', 12);
    int len = str.length();
    for (int i = str.length() - 1; i > 0; i--) {
        if (str.at(i) == '.') {
            len--;
            break;
        } else if (str.at(i) != '0') {
            break;
        }
        len--;
    }
    return str.left(len);
}

template<>
IMOLX_EXPORT QString toString(const QList<int> &is)
{
    QStringList ss;
    foreach (int i, is) {
        ss.append(toString(i));
    }
    return ss.join(", ");
}

template<>
IMOLX_EXPORT QString toString(const QList<double> &ds)
{
    QStringList ss;
    foreach (double d, ds) {
        ss.append(toString(d));
    }
    return ss.join(", ");
}

Node * resizeNode(Node *n, int size)
{
    int cnt = n->cmobjCount();
    for (int i = cnt; i < size; i++) {
        n->append(n);
    }
    for (int i = size; i < cnt; i++) {
        n->remove(n, n->last());
    }
    return n;
}

template<typename T>
bool fromNodeToQList(Node *n, QList<T> &ts)
{
    ts.clear();
    ts.reserve(n->cmobjCount());
    foreach (Node *int_n, n->cmobjs()) {
        ts.append(int_n->get().value<T>());
    }
    return true;
}

template<typename T>
Node * fromQListToNode(const QList<T> &ts, Node *n)
{
    auto ns = resizeNode(n, ts.size())->cmobjs();
    int cnt = 0;
    foreach (Node *str_n, ns) {
        if (str_n->hasCmobj()) str_n->clear(n);
        str_n->set(n, ts.at(cnt++));
    }
    return n;
}

template<> IMOLX_EXPORT bool fromNode(Node *n, QList<int> &ints) { return fromNodeToQList(n, ints); }
template<> IMOLX_EXPORT Node * toNode(const QList<int> &ints, Node *n) { return fromQListToNode(ints, n); }
template<> IMOLX_EXPORT bool fromNode(Node *n, QList<double> &doubles) { return fromNodeToQList(n, doubles); }
template<> IMOLX_EXPORT Node * toNode(const QList<double> &doubles, Node *n) { return fromQListToNode(doubles, n); }
template<> IMOLX_EXPORT bool fromNode(Node *n, QStringList &strs) { return fromNodeToQList(n, strs); }
template<> IMOLX_EXPORT Node * toNode(const QStringList &strs, Node *n) { return fromQListToNode(strs, n); }

template<>
IMOLX_EXPORT bool fromNode(Node *n, QPoint &point)
{
    point.setX(n->c("x")->getInt());
    point.setY(n->c("y")->getInt());
    return true;
}

template<>
IMOLX_EXPORT Node * toNode(const QPoint &point, Node *n)
{
   return n->set(n, "x", point.x())->set(n, "y", point.y());
}

template<>
IMOLX_EXPORT bool fromNode(Node *n, QPointF &pointf)
{
    pointf.setX(n->c("x")->getDouble());
    pointf.setY(n->c("y")->getDouble());
    return true;
}

template<>
IMOLX_EXPORT Node * toNode(const QPointF &pointf, Node *n)
{
    return n->set(n, "x", pointf.x())->set(n, "y", pointf.y());
}

template<>
IMOLX_EXPORT bool fromNode(Node *n, QSize &s)
{
    s.setWidth(n->c("w")->getInt());
    s.setHeight(n->c("h")->getInt());
    return true;
}

template<>
IMOLX_EXPORT Node * toNode(const QSize &s, Node *n)
{
    return n->set(n, "w", s.width())->set(n, "h", s.height());
}

template<>
IMOLX_EXPORT bool fromNode(Node *n, QSizeF &sf)
{
    sf.setWidth(n->c("w")->getDouble());
    sf.setHeight(n->c("h")->getDouble());
    return true;
}

template<>
IMOLX_EXPORT Node * toNode(const QSizeF &sf, Node *n)
{
    return n->set(n, "w", sf.width())->set(n, "h", sf.height());
}

template<>
IMOLX_EXPORT bool fromNode(Node *n, QVector3D &v)
{
    ix::Node *vn = n->first();
    v.setX(vn->getDouble());
    vn = vn->nextBmobj();
    v.setY(vn->getDouble());
    vn = vn->nextBmobj();
    v.setZ(vn->getDouble());
    return true;
}

template<>
IMOLX_EXPORT Node * toNode(const QVector3D &v, Node *n)
{
    return n->set(n, "#2", v.z())->set(n, "#1", v.y())->set(n, "#0", v.x());
}

}
}

///
/// advance
///

namespace ix {
namespace format {

template<>
bool fromNode(Node *n, robot::Vector3d &v3d)
{
    v3d = robot::Vector3d(n->c(0)->getDouble(), n->c(1)->getDouble(), n->c(2)->getDouble());
    return n->cmobjCount() == 3;
}

template<>
Node * toNode(const robot::Vector3d &v3d, Node *n)
{
    n->set(n, "#0", v3d.x())->set(n, "#1", v3d.y())->set(n, "#2", v3d.z());
    return resizeNode(n, 3);
}

template<>
bool fromNode(Node *n, robot::Quaternion &quaternion)
{
    quaternion = robot::Quaternion(n->c(0)->getDouble(), n->c(1)->getDouble(), n->c(2)->getDouble(), n->c(3)->getDouble());
    return n->cmobjCount() == 4;
}

template<>
Node * toNode(const robot::Quaternion &quaternion, Node *n)
{
    n->set(n, "#0", quaternion.w())->set(n, "#1", quaternion.x())->set(n, "#2", quaternion.y())->set(n, "#3", quaternion.z());
    return resizeNode(n, 4);
}

template<>
bool fromNode(Node *n, robot::Frame &frame)
{
    frame = robot::Frame::Identity();
    ix::Node *r = n->c("rot");
    if (r->cmobjCount() == 4) {
        frame.rotate(fromNode<robot::Quaternion>(r));
    } else {
        ix::Node q("q");
        q.copyFrom(r, r);
        frame.rotate(fromNode<robot::Quaternion>(calc::rot2Quaternion(&q)));
    }
    frame.pretranslate(fromNode<robot::Translation>(n->c("trans")));
    return true;
}

template<>
Node * toNode(const robot::Frame &frame, Node *n)
{
    calc::quaternion2Rot(toNode(robot::Quaternion(frame.rotation()), createNodeAt(n, "rot")));
    return n->set(n, "trans.#0", frame(0, 3))->set(n, "trans.#1", frame(1, 3))->set(n, "trans.#2", frame(2, 3));
}

}
}
