#include "config.h"

#include <QSettings>

IMOL_VER_REG("imol.cfg", 1)

#define CONFIG_FILE "config.ini"

struct ConfigData
{
    imol::ModuleObject *mobj;

    ConfigData() {}
};

Config::Config(QObject *parent) : QObject{parent},
    d(new ConfigData)
{
    d->mobj = m().regist(this, "imol.cfg");
}

Config::~Config()
{
    delete d;
}

imol::ModuleObject * Config::mobj(const QString &key) const { return d->mobj->r(key); }

void Config::loadBeforeApplication()
{
    QSettings config(CONFIG_FILE, QSettings::IniFormat);
    foreach (const QString &key, config.allKeys()) {
        QString path = key;
        path.replace("/", IMOL_MODULE_NAME_SEPARATOR);
        d->mobj->set(this, path, config.value(key));
    }
}
