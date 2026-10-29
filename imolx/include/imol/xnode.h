#ifndef IMOL_XNODE_H
#define IMOL_XNODE_H

#include "imolx_global.h"

#include <QList>

class QObject;
class QString;
class QVariant;

namespace ix { typedef imol::ModuleObject Node; }

class IMOLX_EXPORT XNode
{
    IX_DECLARE_D

public:
    explicit XNode(ix::Node *data_node);
    explicit XNode(ix::Node *data_node, ix::Node *shadow_node);
    explicit XNode(const XNode &data_xnode, ix::Node *shadow_node);
    explicit XNode(ix::Node *data_node, const XNode &shadow_xnode);
    XNode(const XNode &other);
    ~XNode();

    ix::Node * data() const;
    ix::Node * shadow(int level = 0) const;

    bool empty() const;
    int size() const;

    explicit operator bool() const;
    XNode & operator=(const XNode &other);

    bool has(const QString &path) const;

    XNode at(const QString &path) const;

    QVariant get() const;

    ix::Node * set(QObject *context, const QVariant &var);
    ix::Node * set(QObject *context, const QString &path, const QVariant &var);

public:
    bool getBool(bool default_bool) const;
    bool getBool() const;
    int getInt(int default_int) const;
    int getInt() const;
    double getDouble(double default_double) const;
    double getDouble() const;
    QString getString(const QString &default_string) const;
    QString getString() const;
};

#endif // IMOL_XNODE_H
