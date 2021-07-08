#ifndef BUNIT_H
#define BUNIT_H

#include "bwaf_global.h"

#include <QStringList>
#include <QHash>

#ifndef MODULE_NAME
#define MODULE_NAME ""
#endif

class QWidget;
class QObject;

namespace imol {
class ModuleObject;
}
namespace bwaf {
class BWAFSHARED_EXPORT BUnit
{
public:
    BUnit(const QString &unit_name, QWidget *widget);
    virtual ~BUnit();

    imol::ModuleObject * mobj() const {return m_mobj;}

    static QString itemRname(const QString &item_name);

    imol::ModuleObject * itemMobj(const QString &item_name) const;
    bool hasItem(const QString &item_name) const;
    int itemCount() const;
    QStringList itemNames() const;

    QWidget * getItemWgt(const QString &item_name);

protected:
    QObject * getItemObj(const QString &item_name);

    bool insertItemObj(QObject *context, const QString &item_name, QObject *item_obj);
    bool removeItemObj(QObject *context, const QString &item_name, bool auto_delete = true);

private:
    QObject *m_context;
    imol::ModuleObject *m_mobj;
};
}

#endif // BUNIT_H
