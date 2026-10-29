#ifndef IMOL_MOBJHANDLER_H
#define IMOL_MOBJHANDLER_H

#include <functional>

#include <QObject>

#include "imolx_global.h"

class IMOLX_EXPORT MobjHandler : public QObject
{
    Q_OBJECT

    IX_DECLARE_D

public:
    typedef std::function<void(imol::ModuleObject*)> ChangedSlot;
    typedef std::function<void(imol::ModuleObject*, const QString &)> AddedSlot;
    typedef std::function<void(imol::ModuleObject*, const QString &)> RemovedSlot;
    typedef std::function<void()> TriggerSlot;

public:
    explicit MobjHandler(QObject *parent = nullptr);
    virtual ~MobjHandler();

    QObject * lastChanger() const;

    void watchChanged(imol::ModuleObject *mobj, const ChangedSlot &func);
    void watchAdded(imol::ModuleObject *mobj, const AddedSlot &func);
    void watchRemoved(imol::ModuleObject *mobj, const RemovedSlot &func);

    void link(QObject *obj, const char *signal_func, imol::ModuleObject *trigger_mobj, const TriggerSlot &trigger_slot_func);

private slots:
    void onChanged(const QVariant &var, const QVariant &old_var, QObject *changer);
    void onAdded(const QString &rname, QObject *changer);
    void onRemoved(const QString &rname, QObject *changer);
};

#endif // IMOL_MOBJHANDLER_H
