#ifndef IMOLX_XNAV_H
#define IMOLX_XNAV_H

#include "xhelper.h"
#include "xnode.h"
#include "xsupport.h"

class XNav : public ix::Module
{
    Q_OBJECT

public:
    explicit XNav(QObject *parent = nullptr);

    QString name() const override;

private:
    void init() override;
    void idle() override;
    void detach() override;

private slots:
    void createBModuleCenterWidget(const QVariant &key_var, const QVariant &old_key_var, QObject *changer);
    void backToLastBMoudule(const QVariant &, const QVariant &, QObject *changer);

private:
    void removeCurrentBModuleCenterWidget(QObject *changer = nullptr);

private:
    ix::Module *m_cur_bm;
    QString m_cur_key;
    int m_cur_index;
    QStringList m_history;
#ifdef XSUPPORT_QT4
    MobjHandler *m_handler;
#endif
};

#endif // IMOLX_XNAV_H
