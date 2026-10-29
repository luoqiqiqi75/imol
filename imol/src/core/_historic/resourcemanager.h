#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "core_global.h"

#include <QObject>
#include <QHash>
#include <QJsonObject>

//!
//! \brief The ResourceWatcher class watches one key in a module
//!
class CORESHARED_EXPORT ResourceWatcher : public QObject
{
    Q_OBJECT

public:
    ResourceWatcher(const QString &name, QObject *parent = nullptr);
    //! emit the changed signal
    bool activate(QObject *context, const QJsonValue &value, QObject *changer);

signals:
    void valueChanged(const QJsonValue &value, const QString &name, QObject *changer);

    void keyAdded(const QString &key, const QJsonValue &value, QObject *changer);
    void keyRemoved(const QString &key, QObject *changer);

private:
    QString m_name;
    QObject *m_context;
};

//!
//! \brief The ResourceLocker class controls the permission of one key in a module
//!
//! A ResourceLocker holds the permissions for a single value item, you may also define specific permission for each
//! module. Each condition will be added to a list. For public permission, the list stands for blacklist, and whitelist
//! for private locker.
//!
class ResourceLocker
{
public:
    ResourceLocker();
    virtual ~ResourceLocker();

    void setRootModule(QObject *module_obj);

    bool isPublic();
    void setPermission(bool is_public);

    void addToReadList(const QString &key, QObject *module_obj);
    void addToWriteList(const QString &key, QObject *module_obj);
    bool removeFromReadList(const QString &key, QObject *module_obj = nullptr);
    bool removeFromWriteList(const QString &key, QObject *module_obj = nullptr);

    bool isReadableForModule(const QString &key, QObject *module_obj);
    bool isWriteableForModule(const QString &key, QObject *module_obj);

private:
    QObject *m_root;
    bool m_is_public;
    QHash<QString, QList<QObject *> *> m_filter;
};

//!
//! \brief The ResourceObject class describes the structure of one single resource in ResourceManager
//!
//! A ResourceObject is related to a module, it saves a JSON object for all data; a list of listeners added all other
//! modules; and the private locker for this resource.
//!
class ResourceObject : public QObject
{
    Q_OBJECT

public:
    ResourceObject(QObject *module_ptr = nullptr);
    virtual ~ResourceObject();

    //! get module pointer
    QObject * module();

    //! get member classes
    QJsonObject & json();
    ResourceLocker & locker();

    //! The watchers is recognized with key, and empty name is for the
    //! whole ResourceObject.
    bool addWatcher(const QString &name);
    bool removeWatcher(const QString &name);
    ResourceWatcher * watcher(const QString &name);

private:
    //the bounded module
    QObject *m_module;

    QJsonObject m_json_obj;
    ResourceLocker m_locker;
    QHash<QString, ResourceWatcher *> m_watchers;
};

class QMutex;
//!
//! \brief The ResourceManager class is a global instance data pool
//!
//! The ResourceManager saves data spicified with module, each module has its own JSON object to save all nessassary
//! infomation, so that the interfaces between modules is not necesserily update every time.
//!
class CORESHARED_EXPORT ResourceManager : public QObject
{
    Q_OBJECT

public:
    ResourceManager();
    ~ResourceManager();
    static ResourceManager & instance();

    bool regist(QObject *context, const QString &module_name, bool is_public = true);
    bool cancel(QObject *context, const QString &module_name);
    bool hasModule(const QString &module_name);

    //! The \a context locks the \a key at \a module_name, set whether it is \a readable or \a writeable
    //! for \a target_module_name or \a target_module.
    bool lock(QObject *context, const QString &module_name, const QString &key,
              QObject *target_module, bool readable = true, bool writeable = true);
    bool lock(QObject *context, const QString &module_name, const QString &key,
              const QString &target_module_name, bool readable = true, bool writeable = true);

    //! The specific watcher for the \a key. Use the read lock to watch, try to write an empty value for
    //! force watching.
    bool enableWatcher(QObject *context, const QString &module_name, const QString &key = "", bool is_force = false, bool is_enable = true);
    ResourceWatcher *getWatcher(QObject *context, const QString &module_name, const QString &key = "");
    bool triggerWatcher(QObject *context, const QString &module_name, const QString &key = "");

    //! Operations for resource searched by \a module_name and \a key.
    bool check(QObject *context, const QString &module_name, const QString &key = "", bool is_read = true, QString *reason = nullptr);
    QJsonValue get(QObject *context, const QString &module_name, const QString &key, bool *ok = nullptr);
    QStringList getKeys(QObject *context, const QString &module_name, bool *ok = nullptr);
    QObject *getModulePtr(QObject *context, const QString &module_name, bool *ok = nullptr);
    bool set(QObject *context, const QString &module_name, const QString &key, const QJsonValue &value, bool trigger_watcher = true);
    bool remove(QObject *context, const QString &module_name, const QString &key);

    //! Synchronization with file \a module .json in \a path.
    bool save(QObject *context, const QString &module_name, const QString &path = "");
    bool load(QObject *context, const QString &module_name, const QString &path = "", bool enable_watcher = false);

    //! O(n) functions
    QString searchModuleName(QObject *module_ptr);

    void dump(const QString &module_name = "");

private:
    QHash<QString, ResourceObject *> m_dataset;
    QMutex *m_mutex;
};
//output methods
CORESHARED_EXPORT ResourceManager & resource();

#endif // RESOURCEMANAGER_H
