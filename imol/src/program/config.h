#ifndef IMOL_CONFIG_H
#define IMOL_CONFIG_H

#include "core/common.h"

struct ConfigData;
class Config : public QObject
{
    Q_OBJECT

public:
    explicit Config(QObject *parent = nullptr);
    ~Config() override;

    imol::ModuleObject * mobj(const QString &key) const;

    void loadBeforeApplication();

private:
    ConfigData *d;
};

#endif // IMOL_CONFIG_H
