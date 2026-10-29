#include "xnav.h"

#include "core/commandmanager.h"
#include "bwaf/centerwidget.h"
#include "bwaf/bfactory.h"

#define NAME "xnav"
#define NAV_PAGE_NAME "xnav_page"

IMOL_VER_REG(mName(MODULE_NAME, NAME), 4)

BFACTORY_REGIST_MODULE(mName(MODULE_NAME, NAME), XNav)

class XNavBackCommand : public imol::BaseCommand
{
public:
    explicit XNavBackCommand(QObject *parent = nullptr) : imol::BaseCommand("xnav.back", false, parent) {}

    QString usage() const override { return tr("xnav.back"); }
    QString instruction() const override { return tr("Go back to last navigated page"); }

protected:
    void run(ix::Node *, const QString &) override { m("xnav.back")->trigger(); }
};

class XNavCommand : public imol::BaseCommand
{
public:
    explicit XNavCommand(QObject *parent = nullptr) : imol::BaseCommand("xnav", false, parent)
    {
        m_ui_n = m("imol.ui.centerwidget.item");
        m_b_n = bfactory().keyMobj();
    }

    QString usage() const override { return tr("xnav <path>"); }
    QString instruction() const override { return tr("Navigate to a local page"); }

protected:
    void complete(ix::Node *n, const QString &text) override;
    void hint(ix::Node *n, const QString &text) override;
    void run(ix::Node *n, const QString &param) override;

private:
    ix::Node *m_ui_n;
    ix::Node *m_b_n;
};

void XNavCommand::complete(ix::Node *, const QString &text)
{
    if (text.endsWith(IMOL_MODULE_NAME_SEPARATOR)) return;
    QString last = text.section(IMOL_MODULE_NAME_SEPARATOR, 0, -2);
    QString half = text.section(IMOL_MODULE_NAME_SEPARATOR, -1);
    if (half.length() == 0) return;
    if (last.isEmpty()) {
        foreach (ix::Node *cn, m_ui_n->cmobjs() + m_b_n->cmobjs()) {
            if (cn->name().startsWith(half)) {
                emit input(cn->name().right(cn->name().length() - half.length()));
                break;
            }
        }
        return;
    }
    ix::Node *tar = m_ui_n->r(last);
    if (tar->isEmptyMobj()) tar = m_b_n->r(last);
    if (tar->isEmptyMobj()) return;
    foreach (ix::Node *cn, tar->cmobjs()) {
        if (cn->name().startsWith(half)) {
            emit input(cn->name().right(cn->name().length() - half.length()));
            break;
        }
    }
}

void XNavCommand::hint(ix::Node *, const QString &text)
{
//    if (!text.endsWith(IMOL_MODULE_NAME_SEPARATOR)) return;
    QString last = text.section(IMOL_MODULE_NAME_SEPARATOR, 0, -2);
    QString half = text.section(IMOL_MODULE_NAME_SEPARATOR, -1);
    QStringList keys;
    if (last.isEmpty()) {
        foreach (const QString &key, m_ui_n->cmobjNames() + m_b_n->cmobjNames()) {
            if (key.startsWith(half)) keys.append(key);
        }
    } else {
        ix::Node *tar = m_ui_n->r(last);
        if (tar->isEmptyMobj()) tar = m_b_n->r(last);
        if (tar->isEmptyMobj()) return;
        foreach (const QString &key, tar->cmobjNames()) {
            if (key.startsWith(half)) keys.append(key);
        }
    }
    if (!keys.isEmpty()) emit output(keys.join("\t") + '\n');
}

void XNavCommand::run(ix::Node *, const QString &param)
{
    QString rpath = param.trimmed();
    if (m_ui_n->hasRmobj(rpath)) {
        emit output(tr("switch to existed page"));
    } else if (!m_b_n->hasRmobj(rpath)) {
        emit output(tr("switch to new page"));
    }
    m("xnav._current")->set(this, rpath);
}

XNav::XNav(QObject *parent) : bwaf::BModule(parent),
    m_cur_bm(nullptr),
    m_cur_index(0)
{
#ifdef XSUPPORT_QT4
    m_handler = new MobjHandler(this);
#endif
    bindMobjJson(NAME, ix::templatePath(NAME));
}

QString XNav::name() const { return NAME; }

void XNav::init()
{
    command().regist(new XNavBackCommand(this));
    command().regist(new XNavCommand(this));
#ifdef XSUPPORT_QT4
    m_handler->watchChanged((mobj()->c("_current")), [this] (ix::Node *ref) {
        createBModuleCenterWidget(ref->get(), QVariant(), this);
    });
    m_handler->watchChanged((mobj()->c("back")), [this] (ix::Node *ref) {
        backToLastBMoudule(ref->get(), QVariant(), this);
    });
#else
    connect(mobj()->c("_current"), &imol::ModuleObject::changed, this, &XNav::createBModuleCenterWidget);
    connect(mobj()->c("back"), &imol::ModuleObject::changed, this, &XNav::backToLastBMoudule);
#endif
}

void XNav::idle()
{
    QString home_nav = m("imol.lic.home")->getString();
    if (!home_nav.isEmpty()) {
        mobj()->c("_current")->set(this, home_nav);
        return;
    }

    home_nav = mobj()->c("home")->getString();
    if (!home_nav.isEmpty()) {
        mobj()->c("_current")->set(this, home_nav);
        return;
    }
}

void XNav::detach()
{
    removeCurrentBModuleCenterWidget(this);
}

void XNav::createBModuleCenterWidget(const QVariant &key_var, const QVariant &old_key_var, QObject *changer)
{
    Q_UNUSED(old_key_var)

    auto center_wgt = instantiatedCenterWidget();
    if (!center_wgt) {
        ELOG << "<xnav> No center widget!";
        return;
    }

    QString key = key_var.toString();
    if (key == m_cur_key) return;

    if (!m_history.endsWith(key)) m_history.append(key);

    // imol control
    if (key.startsWith("[imol")) {
        // todo terminal control
        if (key == "[imol.terminal]") m("imol.terminal.show")->set(this, true);
        return;
    }

    // try switch
    if (center_wgt->hasItem(key)) {
        center_wgt->switchTo(changer, key);
        removeCurrentBModuleCenterWidget(changer);
        return;
    }

    // create new
    BModule *bm = bfactory().create(key, this);
    if (!bm) {
        ELOG << "<xnav> nav to unexsiting bmodule: " << key << " from " << changer;
        return;
    }

    // switch
    bm->runToState(moduleState());
    if (!bm->wgt()) {
        WLOG << "<xnav> nav to non-visible bmodule: " << key << " from " << changer;
        m_cur_key = key;
        m_cur_bm = bm;
        return;
    }

    int index = m_cur_index + 1;
    QString page_name = QString("%1_%2").arg(NAV_PAGE_NAME).arg(index);
    center_wgt->addPage(changer, page_name, bm->wgt());
    center_wgt->switchTo(changer, page_name);
    removeCurrentBModuleCenterWidget(changer); // remove old

    m_cur_index = index;
    m_cur_key = key;
    m_cur_bm = bm;
}

void XNav::backToLastBMoudule(const QVariant &, const QVariant &, QObject *changer)
{
    if (m_history.size() < 2) return;
    m_history.takeLast();
//    createBModuleCenterWidget(m_history.takeLast(), QVariant(), changer);
    ix::xnav::switchTo(changer, m_history.takeLast());
}

void XNav::removeCurrentBModuleCenterWidget(QObject *changer)
{
    if (!m_cur_bm) return;

    QString page_name = QString("%1_%2").arg(NAV_PAGE_NAME).arg(m_cur_index);
    instantiatedCenterWidget()->removeItem(changer, page_name);
    m_cur_bm->startDetach();
    m_cur_bm->deleteLater();
    m_cur_bm = nullptr;
    m_cur_key = "";
}
