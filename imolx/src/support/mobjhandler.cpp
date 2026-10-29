#include "xhelper.h"

struct MobjHandler::Data
{
    QObject *last_changer;
    QHash<imol::ModuleObject *, MobjHandler::ChangedSlot> changed_slots;
    QHash<imol::ModuleObject *, MobjHandler::AddedSlot> added_slots;
    QHash<imol::ModuleObject *, MobjHandler::RemovedSlot> removed_slots;
};

MobjHandler::MobjHandler(QObject *parent) : QObject(parent), d(new Data) { d->last_changer = nullptr; }
MobjHandler::~MobjHandler() { delete d; }

QObject * MobjHandler::lastChanger() const { return d->last_changer; }

void MobjHandler::watchChanged(imol::ModuleObject *mobj, const ChangedSlot &func)
{
    if (!mobj || mobj->isEmptyMobj() || d->changed_slots.contains(mobj)) {
//        ELOG << "watch wrong mobj changed: " << mobj;
        return;
    }
    d->changed_slots.insert(mobj, func);
    connect(mobj, SIGNAL(changed(QVariant,QVariant,QObject*)), this, SLOT(onChanged(QVariant,QVariant,QObject*)));
}

void MobjHandler::watchAdded(imol::ModuleObject *mobj, const AddedSlot &func)
{
    if (!mobj || mobj->isEmptyMobj() || d->added_slots.contains(mobj)) {
        //        ELOG << "watch wrong mobj added: " << mobj;
        return;
    }
    d->added_slots.insert(mobj, func);
    connect(mobj, SIGNAL(added(QString,QObject*)), this, SLOT(onAdded(QString,QObject*)));
}

void MobjHandler::watchRemoved(imol::ModuleObject *mobj, const RemovedSlot &func)
{
    if (!mobj || mobj->isEmptyMobj() || d->removed_slots.contains(mobj)) {
        //        ELOG << "watch wrong mobj removed: " << mobj;
        return;
    }
    d->removed_slots.insert(mobj, func);
    connect(mobj, SIGNAL(removed(QString,QObject*)), this, SLOT(onRemoved(QString,QObject*)));
}

void MobjHandler::link(QObject *obj, const char *signal_func, imol::ModuleObject *trigger_mobj, const TriggerSlot &trigger_slot_func)
{
    QObject::connect(obj, signal_func, trigger_mobj, SLOT(trigger()));
    watchChanged(trigger_mobj, [trigger_slot_func] (ix::Node *) { trigger_slot_func(); });
}

void MobjHandler::onChanged(const QVariant &, const QVariant &, QObject *changer)
{
    imol::ModuleObject *mobj = qobject_cast<imol::ModuleObject *>(sender());
    if (!mobj || !d->changed_slots.contains(mobj)) {
        //        ELOG << "wrong mobj changed by: " << changer;
        return;
    }
    d->last_changer = changer;
    d->changed_slots.value(mobj)(mobj);
}

void MobjHandler::onAdded(const QString &rname, QObject *changer)
{
    imol::ModuleObject *mobj = qobject_cast<imol::ModuleObject *>(sender());
    if (!mobj || !d->added_slots.contains(mobj)) {
        //        ELOG << "wrong mobj added by: " << changer;
        return;
    }
    d->last_changer = changer;
    d->added_slots.value(mobj)(mobj, rname);
}

void MobjHandler::onRemoved(const QString &rname, QObject *changer)
{
    imol::ModuleObject *mobj = qobject_cast<imol::ModuleObject *>(sender());
    if (!mobj || !d->removed_slots.contains(mobj)) {
        //        ELOG << "wrong mobj removed by: " << changer;
        return;
    }
    d->last_changer = changer;
    d->removed_slots.value(mobj)(mobj, rname);
}
