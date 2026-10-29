#include "xsupport.h"

#include "core/common.h"
#include "core/networkmanager.h"
#include "bwaf/bfactory.h"

#include "xservice.h"
#include "support/lircossupporter.h"

#define NAME "xsupport"

IMOL_VER_REG(mName(MODULE_NAME, NAME), 6)

BFACTORY_REGIST_MODULE(mName(MODULE_NAME, NAME), XSupport)

BaseSupporter *supporter = nullptr;

XSupport::XSupport(QObject *parent) : bwaf::BModule(parent)
{
    bindMobjJson(NAME, ix::templatePath(NAME));

    connect(m().emptyMobj(), SIGNAL(destroyed()), this, SLOT(guard()));
}

QString XSupport::name() const  { return NAME; }

void XSupport::initXcore()
{
    supporter = new BaseSupporter(new BaseGrammar(this), this);
    m("xservice.host")->set(this, "ip", "192.168.40.128")->set(this, "port", 6666);
    m("xservice.trigger.connect")->trigger();
}

void XSupport::initLircos()
{
    supporter = new LircosSupporter(this);
    connect(qobject_cast<XService *>(xservice()), SIGNAL(support(ix::Node*,const ix::Node*)),
            supporter, SLOT(supportXService(ix::Node*,const ix::Node*)));
}

void XSupport::init()
{
    // default
    if (m("xcore")->isEmptyMobj()) mobj()->c("type")->set(this, "lircos");

    // cfg
    QString support_type = m("imol.cfg.xsupport.type")->getString();
    if (!support_type.isEmpty()) mobj()->c("type")->set(this, support_type);
}

void XSupport::afterInit()
{
    QString t = mobj()->c("type")->getString();
    ILOG << "<xsupport> init as " << t;
    if (t == "none") {
        return;
    } else if (t == "lircos") {
        initLircos();
    } else if (t == "xcore") {
        initXcore();
    } else {
        initXcore();
    }
    if (supporter) supporter->init();
}

void XSupport::detach()
{
    delete supporter;
    supporter = nullptr;
}

void XSupport::guard()
{
    ELOG << "<xsupport> FATAL: empty mobj destroyed";
}
