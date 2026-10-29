#include "xnode.h"

#include "core/common.h"

IMOL_VER_REG("imolx.xnode", 2)

struct XNode::Data
{
    QList<ix::Node *> ns;

    Data() {}
    Data(const QList<ix::Node *> &mobjs) : ns(mobjs) {}
};

XNode::XNode(ix::Node *dn) : d(new Data) { d->ns << dn; }
XNode::XNode(ix::Node *dn, ix::Node *sn) : d(new Data) { d->ns << dn << sn; }
XNode::XNode(const XNode &dxn, ix::Node *sn) : d(new Data(dxn.d->ns)) { d->ns << sn; }
XNode::XNode(ix::Node *dn, const XNode &sxn) : d(new Data) { d->ns << dn << sxn.d->ns; }
XNode::XNode(const XNode &other) : d(new Data(other.d->ns)) {}

XNode::~XNode() { delete d; }

ix::Node * XNode::data() const { return d->ns.first(); }
ix::Node * XNode::shadow(int level) const { return d->ns.value(level + 1, m().emptyMobj()); }

bool XNode::empty() const
{
    foreach (auto mobj, d->ns) {
        if (mobj->hasCmobj()) return false;
    }
    return true;
}

int XNode::size() const
{
    int s = 0;
    foreach (auto mobj, d->ns) {
        s = qMax(s, mobj->size());
    }
    return s;
}

XNode::operator bool() const
{
    foreach (auto mobj, d->ns) {
        if (!mobj->isEmptyMobj()) return true;
    }
    return false;
}

XNode & XNode::operator=(const XNode &other)
{
    d->ns = other.d->ns;
    return *this;
}

bool XNode::has(const QString &path) const
{
    foreach (auto mobj, d->ns) {
        if (mobj->hasRmobj(path)) return true;
    }
    return false;
}

XNode XNode::at(const QString &path) const
{
    XNode xn(d->ns.first()->r(path));
    for (int i = 1; i < d->ns.size(); i++) {
        ix::Node *rmobj = d->ns.at(i)->r(path);
        if (!rmobj->isEmptyMobj()) xn.d->ns.append(rmobj);
    }
    return xn;
}

QVariant XNode::get() const
{
    foreach (auto mobj, d->ns) {
        if (!mobj->isEmptyMobj()) return mobj->get();

//        QVariant var = mobj->get();
//        if (!var.isNull()) return var;
    }
    return QVariant();
}

ix::Node * XNode::set(QObject *context, const QVariant &var)
{
    return data()->set(context, var);
}

ix::Node * XNode::set(QObject *context, const QString &path, const QVariant &var)
{
    return data()->set(context, path, var);
}

bool XNode::getBool(bool default_bool) const
{
    QVariant var = get();
    return var.isNull() ? default_bool : var.toBool();
}

bool XNode::getBool() const { return getBool(false); }

int XNode::getInt(int default_int) const
{
    QVariant var = get();
    return var.isNull() ? default_int : var.toInt();
}

int XNode::getInt() const { return getInt(0); }

double XNode::getDouble(double default_double) const
{
    QVariant var = get();
    return var.isNull() ? default_double : var.toDouble();
}

double XNode::getDouble() const { return getDouble(0); }

QString XNode::getString(const QString &default_string) const
{
    QVariant var = get();
    return var.isNull() ? default_string : var.toString();
}

QString XNode::getString() const { return get().toString(); }
