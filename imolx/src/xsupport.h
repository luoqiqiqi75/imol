#ifndef IMOL_XSUPPORT_H
#define IMOL_XSUPPORT_H

#include "xhelper.h"

class BaseSupporter;
extern BaseSupporter *supporter;

class XSupport : public bwaf::BModule
{
    Q_OBJECT

public:
    explicit XSupport(QObject *parent = nullptr);

    QString name() const;

private:
    void initXcore();
    void initLircos();

private:
    void init();
    void afterInit();
    void detach();

private slots:
    void guard();
};

#endif // IMOL_XSUPPORT_H
