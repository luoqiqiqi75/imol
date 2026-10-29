#include "lircossupporter.h"

#include <QDataStream>
#include <QMetaEnum>

#include "xservice.h"
#include "lircos/HMICommonStruct.h"
#include "lircos/editor_struct.h"
#include "lircosgrammar.h"

IMOL_VER_REG("imolx.xsupport.lircos", 4)

HMISettingInfo *hmi_si = nullptr;

class LircosSupporter::Private
{
public:
    LircosSupporter *context;

    MobjHandler *h;
    ix::Node *trigger_n;
    ix::Node *hmicmd_req_n, *hmicmd_res_n;
    ix::Node *event_req_n, *event_res_n;
    ix::Node *manager_req_n, *manager_res_n;

public:
    Private(LircosSupporter *context): context(context),
        h(new MobjHandler(context)),
        trigger_n(m("xsupport.trigger")),
        hmicmd_req_n(m("xsupport.lircos.hmicmd.request")), hmicmd_res_n(hmicmd_req_n->b("response")),
        event_req_n(m("xsupport.lircos.event.request")), event_res_n(event_req_n->b("response")),
        manager_req_n(m("xsupport.lircos.manager.request")), manager_res_n(manager_req_n->b("response"))
    {
    }

    ~Private()
    {
        delete h;
    }

public:
    ix::Node * execHmiCmd(int cmd, int ext, const QByteArray &data);
    ix::Node * execEvent(const QString &event_type, ix::Node *in_n = nullptr);
    ix::Node * execManager(const QString &mgr_func, ix::Node *in_n);
    ix::Node * execManager(const QString &mgr_func, const QVariant &in_var = QVariant());

public:
    void registerCustom(const QString &key, CustomF f)
    {
        h->watchChanged(trigger_n->r(key), [=] (ix::Node *ref) {
            ix::Node *in_n = qobject_cast<ix::Node *>(ref->get().value<QObject *>());
            ix::Node *out_n = qobject_cast<ix::Node *>(h->lastChanger());
            if (!in_n || !out_n) {
                ELOG << "<lircos> invalid in/output for trigger " << ref->fullName();
                return;
            }

            f(in_n, out_n);
        });
    }

    template<int Cmd, int Ext = 0>
    void registerHmiCmd(const QString &key, CmdInF in_f, CmdOutF out_f, ErrF err_f = NULL, ExtF ext_f = NULL)
    {
        registerCustom(key, [=] (ix::Node *in_n, ix::Node *out_n) {
            ix::Node *res_n = execHmiCmd(Cmd, ext_f ? ext_f() : Ext, in_f ? in_f(in_n) : QByteArray());
            if (!res_n->getBool()) {
                out_n->set(context, -1);
                return;
            }
            if (res_n->c("cmd")->getInt() == HMI_ERR) {
                int ext = res_n->c("ext")->getInt();
                if (err_f) {
                    err_f(ext);
                } else {
                    out_n->set(context, ext > 0 ? -ext - 1 : ext - 1);
                }
                return;
            }
            if (!out_f) return;
            if (!out_f(out_n, res_n->c("data")->get().toByteArray())) return; // todo
        });
    }

    void registerManager(const QString &key, const QString &mgr_func)
    {
        registerCustom(key, [=] (ix::Node *in_n, ix::Node *out_n) {
            QString sub_key = context->m_sub_key; // keep cache
            ix::Node *res_n = execManager(mgr_func, in_n);
            out_n->copyFrom(context, sub_key.isEmpty() ? res_n : res_n->r(sub_key));
            return;
        });
    }
};

ix::Node * LircosSupporter::Private::execHmiCmd(int cmd, int ext, const QByteArray &data)
{
    hmicmd_req_n->set(context, "cmd", cmd);
    hmicmd_req_n->set(context, "ext", ext);
    hmicmd_req_n->set(context, "data", data);
    hmicmd_req_n->trigger();
    return hmicmd_res_n;
}

ix::Node * LircosSupporter::Private::execEvent(const QString &event_type, ix::Node *in_n)
{
    ix::Node *n = event_req_n->r(event_type);
    if (in_n) {
        n->quiet(true);
        n->copyFrom(context, in_n, true, true);
        n->quiet(false);
    }
    event_res_n->clear(context);
    n->trigger();
    return event_res_n;
}

ix::Node * LircosSupporter::Private::execManager(const QString &mgr_func, ix::Node *in_n)
{
    ix::Node *n = manager_req_n->r(mgr_func);
    if (in_n) {
        n->quiet(true);
        n->copyFrom(context, in_n, true, true);
        n->quiet(false);
    }
    manager_res_n->clear(context);
    n->trigger();
    return manager_res_n;
}

ix::Node * LircosSupporter::Private::execManager(const QString &mgr_func, const QVariant &in_var)
{
    ix::Node *n = manager_req_n->r(mgr_func);
    n->quiet(true);
    n->importFromVariant(context, in_var, true, true, true);
    n->quiet(false);
    manager_res_n->clear(context);
    n->trigger();
    return manager_res_n;
}

LircosSupporter::LircosSupporter(QObject *parent) : BaseSupporter(new LircosGrammar(parent), parent),
    p(new Private(this))
{
}

LircosSupporter::~LircosSupporter()
{
    delete interpreter;
    delete p;
}

template<typename T>
LircosSupporter::CmdInF buildInF()
{
    return [] (ix::Node *n) -> QByteArray {
        QByteArray ba(sizeof(T), 0);
        ix::format::fromNode(n, *(reinterpret_cast<T *>(ba.data())));
        return ba;
    };
}

template<typename T>
LircosSupporter::CmdOutF buildOutF()
{
    return [] (ix::Node *n, const QByteArray &ba) -> bool {
        T t;
        memcpy(&t, ba.constData(), ba.size());
        ix::format::toNode(t, n);
        return true;
    };
}

void LircosSupporter::init()
{
    // setting
    hmi_si = reinterpret_cast<HMISettingInfo*>(m("xsupport.lircos.setting.hmisettinginfo")->get().value<void *>());

    // sys
    if (hmi_si) {
        p->registerCustom("g.sys.mode", [=] (ix::Node *, ix::Node *out_n) { out_n->set(this, hmi_si->eOperMode == OPERATING_MODE_AUTO ? "auto" : "manual"); });
        p->registerCustom("g.sys.state", [=] (ix::Node *, ix::Node *out_n) {
            switch (hmi_si->eProgExecState) {
            case PROG_EXECUTE_STATE_NOT_LOAD: out_n->set(this, "not_loaded"); break;
            case PROG_EXECUTE_STATE_RUNNING: out_n->set(this, "busy"); break;
            case PROG_EXECUTE_STATE_STOP: out_n->set(this, "idle"); break;
            default: out_n->set(this, "unknown"); break;
            }
        });
        p->registerCustom("g.sys.power", [=] (ix::Node *, ix::Node *out_n) {
            switch (hmi_si->eRobotState) {
            case ROBOT_MOTOR_OFF: out_n->set(this, "off"); break;
            case ROBOT_MOTOR_ON: out_n->set(this, "on"); break;
            case ROBOT_GENERAL_STOPPED: out_n->set(this, "gstop"); break;
            case ROBOT_EMERGENCY_STOPPED: out_n->set(this, "estop"); break;
            case ROBOT_ERROR: out_n->set(this, "error"); break;
            default: out_n->set(this, "unknown"); break;
            }
        });
    }
    p->registerHmiCmd<HMI_GET_POSITION>("g.sys.posture", NULL, buildOutF<Position>());
    p->registerHmiCmd<HMI_EULER_TO_QUATERNION>("c.sys.calc.rot2quaternion", buildInF<Euler>(), buildOutF<Quaternion>());
    p->registerHmiCmd<HMI_QUATERNION_TO_EULER>("c.sys.calc.quaternion2rot", buildInF<Quaternion>(), buildOutF<Euler>());
    p->registerManager("g.sys.toolset", "toolset.GetCurrent");
    p->registerManager("s.sys.toolset", "toolset.SetCurrent");

    // data
    p->registerManager("g.data.var", "coder.GetVar");
    p->registerManager("s.data.var", "coder.SetVar");
    p->registerManager("g.data.toolset", "toolset.GetToolset");
    p->registerManager("s.data.toolset", "toolset.SetToolset");

    // device
    p->registerManager("g.device.io", "io.GetSignalValue");
    p->registerManager("s.device.io", "io.SetSignalValue");
    p->registerManager("g.config.io_signal", "io.GetSignalContent");

    // project
    auto thinfilm_click = [=] (const QString &key) {
        ix::Node tmp("");
        tmp.set(this, "key", key);
        tmp.set(this, "pressed", true);
        p->execEvent("thinfilm", &tmp)->getBool();
        tmp.set(this, "pressed", false);
        p->execEvent("thinfilm", &tmp);
    };
    p->registerCustom("c.project.next", [=] (ix::Node *, ix::Node *) { thinfilm_click("forward"); });
    p->registerCustom("c.project.prev", [=] (ix::Node *, ix::Node *) { thinfilm_click("back"); });
    p->registerCustom("c.project.start", [=] (ix::Node *, ix::Node *) { thinfilm_click("start"); });
    p->registerCustom("c.project.stop", [=] (ix::Node *, ix::Node *) { thinfilm_click("stop"); });
    p->registerManager("c.project.pp.refresh", "coder.RefreshPP");
    p->registerHmiCmd<HMI_PP_TO_MAIN>("c.project.pp.main", NULL, NULL);
    p->registerHmiCmd<HMI_PP_TO_ROUTINE>("c.project.pp.routine", buildInF<RoutineInfo>(), NULL);
    p->registerHmiCmd<HMI_LOAD_PROJECT>("c.project.reload", [this] (ix::Node *n) {
        return n->c("path")->getString(p->execManager("task.GetCurrentProject")->getString()).toUtf8();
    }, NULL, NULL, [this] {
        return p->execManager("task.GetCurrentTaskID")->getInt();
    });
}
