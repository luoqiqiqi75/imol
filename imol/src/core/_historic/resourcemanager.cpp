#include "resourcemanager.h"

#include <QDir>
#include <QJsonDocument>
#include <QMutex>

#include "logmanager.h"

#define LOCKER_READ "read"
#define LOCKER_WRITE "write"

//! ResourceWatcher
ResourceWatcher::ResourceWatcher(const QString &name, QObject *parent) : QObject(parent),
    m_name(name),
    m_context(parent)
{
}

bool ResourceWatcher::activate(QObject *context, const QJsonValue &value, QObject *changer)
{
    if (context != m_context) return false;
    emit valueChanged(value, m_name, changer);
    return true;
}

//! ResourceLocker
ResourceLocker::ResourceLocker()
{
}

ResourceLocker::~ResourceLocker()
{
    qDeleteAll(m_filter);
}

void ResourceLocker::setRootModule(QObject *module_obj)
{
    m_root = module_obj;
}

bool ResourceLocker::isPublic()
{
    return m_is_public;
}

void ResourceLocker::setPermission(bool is_public)
{
    m_is_public = is_public;
}

void ResourceLocker::addToReadList(const QString &key, QObject *module_obj)
{
    QString read_key = QString("%1_%2").arg(key).arg(LOCKER_READ);
    if (!m_filter.contains(read_key)) m_filter.insert(read_key, new QList<QObject *>());
    m_filter[read_key]->append(module_obj);
}

void ResourceLocker::addToWriteList(const QString &key, QObject *module_obj)
{
    QString write_key = QString("%1_%2").arg(key).arg(LOCKER_WRITE);
    if (!m_filter.contains(write_key)) m_filter.insert(write_key, new QList<QObject *>());
    m_filter[write_key]->append(module_obj);
}

bool ResourceLocker::removeFromReadList(const QString &key, QObject *module_obj)
{
    QString read_key = QString("%1_%2").arg(key).arg(LOCKER_READ);
    if (!m_filter.contains(read_key)) return false;
    if (!module_obj) return m_filter.remove(read_key) > 0;
    return m_filter[read_key]->removeOne(module_obj);
}

bool ResourceLocker::removeFromWriteList(const QString &key, QObject *module_obj)
{
    QString write_key = QString("%1_%2").arg(key).arg(LOCKER_WRITE);
    if (!m_filter.contains(write_key)) return false;
    if (!module_obj) return m_filter.remove(write_key) > 0;
    return m_filter[write_key]->removeOne(module_obj);
}

bool ResourceLocker::isReadableForModule(const QString &key, QObject *module_obj)
{
    if (module_obj == m_root) return true;
    //super_key means all keys are in the list
    QString super_key = QString("_%1").arg(LOCKER_READ);
    if (m_filter.contains(super_key)) return m_filter[super_key]->contains(module_obj) != m_is_public;
    //when the key is not defined in the list, returns default permission
    QString read_key = QString("%1_%2").arg(key).arg(LOCKER_READ);
    if (!m_filter.contains(read_key)) return m_is_public;
    //if the key is in the list, the permission is opposite
    bool in_list = m_filter[read_key]->contains(module_obj);
    return m_is_public != in_list;
}

bool ResourceLocker::isWriteableForModule(const QString &key, QObject *module_obj)
{
    if (module_obj == m_root) return true;

    QString super_key = QString("_%1").arg(LOCKER_WRITE);
    if (m_filter.contains(super_key)) return m_filter[super_key]->contains(module_obj) != m_is_public;

    QString write_key = QString("%1_%2").arg(key).arg(LOCKER_WRITE);
    if (!m_filter.contains(write_key)) return m_is_public;

    bool in_list = m_filter[write_key]->contains(module_obj);
    return m_is_public != in_list;
}

//! ResourceObject
ResourceObject::ResourceObject(QObject *module_ptr) : QObject(),
    m_module(module_ptr)
{
}

ResourceObject::~ResourceObject()
{
    qDeleteAll(m_watchers);
}

QObject * ResourceObject::module() {return m_module;}
QJsonObject & ResourceObject::json() {return m_json_obj;}
ResourceLocker & ResourceObject::locker() {return m_locker;}

bool ResourceObject::addWatcher(const QString &name)
{
    if (m_watchers.contains(name)) return false;
    m_watchers.insert(name, new ResourceWatcher(name, this));
    return true;
}

bool ResourceObject::removeWatcher(const QString &name)
{
    if (!m_watchers.contains(name)) return false;
    ResourceWatcher *watcher = m_watchers.value(name);
    m_watchers.remove(name);
    delete watcher;
    return true;
}

ResourceWatcher * ResourceObject::watcher(const QString &name)
{
    return m_watchers.value(name, nullptr);
}

//! ResourceManager
ResourceManager::ResourceManager() : QObject(),
    m_mutex(new QMutex)
{
}

ResourceManager::~ResourceManager()
{
    qDeleteAll(m_dataset);
}

ResourceManager & ResourceManager::instance()
{
    static ResourceManager manager;
    return manager;
}

bool ResourceManager::regist(QObject *context, const QString &module_name, bool is_public)
{
    if (m_dataset.contains(module_name)) return false;

    ResourceObject *obj = new ResourceObject(context);
    obj->locker().setRootModule(context);
    obj->locker().setPermission(is_public);
    m_dataset.insert(module_name, obj);
    return true;
}

bool ResourceManager::cancel(QObject *context, const QString &module_name)
{
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    if (!obj) return false;
    if (!obj->locker().isWriteableForModule("", context)) return false;

    m_dataset.remove(module_name);
    delete obj;
    return true;
}

bool ResourceManager::hasModule(const QString &module_name)
{
    return m_dataset.contains(module_name);
}

bool ResourceManager::lock(QObject *context, const QString &module_name, const QString &key,
                           QObject *target_module, bool readable, bool writeable)
{
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    if (!obj) return false;
    if (!obj->locker().isWriteableForModule(key, context)) return false;
    if (!target_module) return false;

    //add read lock
    if ((obj->locker().isPublic() && readable) || (!obj->locker().isPublic() && !readable)) {
        obj->locker().removeFromReadList(key, target_module);
    } else {
        obj->locker().addToReadList(key, target_module);
    }
    //add write lock
    if ((obj->locker().isPublic() && writeable) || (!obj->locker().isPublic() && !writeable)) {
        obj->locker().removeFromWriteList(key, target_module);
    } else {
        obj->locker().addToWriteList(key, target_module);
    }
    return true;
}

bool ResourceManager::lock(QObject *context, const QString &module_name, const QString &key,
                           const QString &target_module_name, bool readable, bool writeable)
{
    ResourceObject *target = m_dataset.value(target_module_name, nullptr);
    if (!target) return false;
    return lock(context, module_name, key, target->module(), readable, writeable);
}

bool ResourceManager::enableWatcher(QObject *context, const QString &module_name, const QString &key, bool is_force, bool is_enable)
{
    QString reason;
    if (is_force) {
        if (!check(context, module_name, key, true, &reason)) {
            if (!check(context, module_name, key, false, &reason)) {
                WLOG << "<resource> cannot watch module [" << module_name << "]" << " with new key [" << key << "]" << ": " << reason;
                return false;
            }
            //set will success if check pass
            set(context, module_name, key, QJsonValue());
        }
    } else if (!check(context, module_name, key, true, &reason)) {
        WLOG << "<resource> cannot watch module [" << module_name << "]" << " with key [" << key << "]" << ": " << reason;
        return false;
    }

    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    return is_enable ? obj->addWatcher(key) : obj->removeWatcher(key);
}

ResourceWatcher * ResourceManager::getWatcher(QObject *context, const QString &module_name, const QString &key)
{
    QString reason;
    if (!check(context, module_name, key, true, &reason)) {
        WLOG << "<resource> no watcher for module [" << module_name << "]" << ".[" << key << "]" << ": " << reason;
        return nullptr;
    }

    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    ResourceWatcher *watcher = obj->watcher(key);
    if (!watcher) WLOG << "<resource> watcher for module [" << module_name << "]" << ".[" << key << "] is disabled";
    return watcher;
}

bool ResourceManager::triggerWatcher(QObject *context, const QString &module_name, const QString &key)
{
    QString reason;
    if (!check(context, module_name, key, true, &reason)) {
        WLOG << "<resource> no watcher for module [" << module_name << "]" << ".[" << key << "]" << ": " << reason;
        return false;
    }

    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    ResourceWatcher *watcher = obj->watcher(key);
    if (watcher) watcher->activate(obj, key.isEmpty() ? QJsonValue(key) : obj->json().value(key), context);
    return true;
}

bool ResourceManager::check(QObject *context, const QString &module_name, const QString &key, bool is_read, QString *reason)
{
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    if (!obj) {
        if (reason) *reason = tr("Module does not exist.");
        return false;
    }
    if ((is_read && !obj->locker().isReadableForModule(key, context)) ||
            (!is_read && !obj->locker().isWriteableForModule(key, context))) {
        if (reason) *reason = tr("%1 locked by %2 module.")
                .arg(key.isEmpty() ? tr("All keys are") : tr("Key is"))
                .arg(obj->locker().isPublic() ? tr("public") : tr("private"));
        return false;
    }
    if (is_read && !key.isEmpty() && !obj->json().contains(key)) {
        if (reason) *reason = tr("Key does not exist.");
        return false;
    }
    return true;
}

QJsonValue ResourceManager::get(QObject *context, const QString &module_name, const QString &key, bool *ok)
{
    QString reason = "";
    if (!check(context, module_name, key, true, &reason)) {
        if (ok) *ok = false;
        return QJsonValue();
    }
    if (ok) *ok = true;
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    return obj->json().value(key);
}

QStringList ResourceManager::getKeys(QObject *context, const QString &module_name, bool *ok)
{
    QString reason = "";
    if (!check(context, module_name, "", true, &reason)) {
        if (ok) *ok = false;
        return QStringList();
    }
    if (ok) *ok = true;
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    return obj->json().keys();
}

QObject * ResourceManager::getModulePtr(QObject *context, const QString &module_name, bool *ok)
{
    if (!check(context, module_name)) {
        if (ok) *ok = false;
        return nullptr;
    }
    if (ok) *ok = true;
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    return obj->module();
}

bool ResourceManager::set(QObject *context, const QString &module_name, const QString &key, const QJsonValue &value, bool trigger_watcher)
{
    if (!check(context, module_name, key, false)) return false;
    ResourceObject *obj = m_dataset.value(module_name, nullptr);

    m_mutex->lock();
    obj->json().insert(key, value);
    m_mutex->unlock();

    if (!trigger_watcher) return true;

    //notify module changing
    ResourceWatcher *module_watcher = obj->watcher("");
    if (module_watcher) module_watcher->activate(obj, QJsonValue(key), context);

    //notify key changing
    ResourceWatcher *watcher = obj->watcher(key);
    if (watcher) watcher->activate(obj, value, context);

    return true;
}

bool ResourceManager::remove(QObject *context, const QString &module_name, const QString &key)
{
    if (!check(context, module_name, key, false)) return false;
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    obj->json().remove(key);
    obj->removeWatcher(key);
    return true;
}

bool ResourceManager::save(QObject *context, const QString &module_name, const QString &path)
{
    if (!check(context, module_name, "")) return false;
    //open file
    QString json_path = path.isEmpty() ? "." : path;
    json_path.replace("/", QDir::separator());
    if (json_path.endsWith(QDir::separator())) json_path.append(module_name + ".json");
    QFile json_file(json_path);
    if (!json_file.open(QIODevice::WriteOnly)) {
        ELOG << "<resouce> cannot save module [" << module_name << "] to file \""  << json_path << "\"";
        return false;
    }
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    QJsonObject json_obj = obj->json();
    QJsonDocument json_doc(json_obj);
    json_file.write(json_doc.toJson());
    json_file.close();
    return true;
}

bool ResourceManager::load(QObject *context, const QString &module_name, const QString &path, bool enable_watcher)
{
    if (!check(context, module_name, "", false)) return false;
    //open file
    QString json_path = path.isEmpty() ? "." : path;
    json_path.replace("/", QDir::separator());
    if (json_path.endsWith(QDir::separator())) json_path.append(module_name + ".json");
    QFile json_file(json_path);
    if (!json_file.open(QIODevice::ReadOnly)) {
        WLOG << "<resouce> cannot load module [" << module_name << "] from file \""  << json_path << "\"";
        return false;
    }
    QJsonDocument json_doc = QJsonDocument::fromJson(json_file.readAll());
    json_file.close();

    QJsonObject json_obj = json_doc.object();
    ResourceObject *obj = m_dataset.value(module_name, nullptr);
    foreach (QString key, json_obj.keys()) {
        obj->json().insert(key, json_obj.value(key));
        if (enable_watcher) obj->addWatcher(key);
    }
    return true;
}

QString ResourceManager::searchModuleName(QObject *module_ptr)
{
    foreach (QString name, m_dataset.keys()) {
        if (m_dataset.value(name)->module() == module_ptr) return name;
    }
    return "";
}

void ResourceManager::dump(const QString &module_name)
{
    SLOG << "------ resource dump start ------";
    QStringList names = module_name.isEmpty() ? m_dataset.keys() : (QStringList() << module_name);
    foreach (QString name, names) {
        ResourceObject *obj = m_dataset.value(name, nullptr);
        if (!obj) {
            ELOG << "#### " << "no module:" << name;
            continue;
        }
        SLOG << "#### " << (obj->locker().isPublic() ? "public" : "private") << " module:" << name << " ####";
        QJsonObject json_obj = obj->json();
        foreach (QString key, json_obj.keys()) {
            SLOG << "json:" << key << " -> " << json_obj.value(key);
        }
    }
    SLOG << "------ resource dump end ------";
}

ResourceManager & resource()
{
    return ResourceManager::instance();
}
