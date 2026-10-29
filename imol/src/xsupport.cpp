#include "../xsupport.h"

#include "../xrobot.h"

#include "configuration/def/conf_path.hpp"
#include "configuration/global_mgr.hpp"
#include "motion/interface/base/mc_interface.hpp"
#include "motion/interface/application/move_conf_ob.hpp"
#include "motion/interface/convert/convert_data.hpp"
#include "driver/driver_manager.hpp"
#include "version.hpp"

IMOL_REGISTER_VERSION(xsupport, 6)

#define MGR_CONTROLLER configuration::GLBConfigurationMgr::GetInstance()->ControllerParamMgr

namespace ix {

namespace info {
std::string version() { return XCORE_VERSION_STR; }
}

namespace mgr {
std::shared_ptr<configuration::param_mgr::InteractiveParamManager> interactive()
{
    return configuration::GLBConfigurationMgr::GetInstance()->InteractiveConfParamMGR();
}
std::string cfgPath(const std::string& file_name)
{
    imol::Strings fi = {configuration::DEFAULT_INTERACTIVE_DATA_WORKSPACE_PATH, MGR_CONTROLLER()->QuerySetting()->robot_model, file_name};
    return fi.toString("/") + ".cfg";
}
}

namespace format {

double getPsi(const CartPos& cart_pos)
{
    return cart_pos.redundant.jnt(0);
}

CartPos& setPsi(CartPos& cart_pos, double psi)
{
    cart_pos.redundant.setJnt(0, psi);
    return cart_pos;
}

imol::Ints getConf(const CartPos& cart_pos, ConfData* conf, int* err)
{
    ConfData c;
    int ret = motion::basic_interface::GetIkSolverPos()->ConfJudge(cart_pos.ik(err), cart_pos, c);
    if (err && *err != 0) *err = ret;
    if (conf) *conf = c;
    return imol::Ints({c.cf1, c.cf2, c.cf3, c.cf4, c.cf5, c.cf6, c.cf7, c.cfx});
}

LoadInertia toLoadInertia(const Load& load)
{
    LoadInertia li(load.mass, load.cog);
    li.SetInetia(load.inertia.x(), load.inertia.y(), load.inertia.z());
    return li;
}

ExpectedSpeed toExpectedSpeed(double speed)
{
    speed = speed < 0 ? 0 : speed;

    ExpectedSpeed es;
    es.tcp_speed = speed;
    es.rot_speed = 200 * KDL::PI / 180;
    es.psi_speed = es.rot_speed;

    if (speed < 0.01) {
        es.jnt_percent = 0.01;
    } else if (speed < 0.02) {
        es.jnt_percent = 0.03;
    } else if (speed < 0.05) {
        es.jnt_percent = 0.05;
    } else if (speed < 0.08) {
        es.jnt_percent = 0.08;
    } else if (speed < 1) {
        es.jnt_percent = es.tcp_speed;
    } else {
        es.jnt_percent = 1;
    }
    return es;
}

SlipData toSlipData(double zone)
{
    zone = zone < 0 ? 0 : zone;

    SlipData sd;
    sd.distance = zone;
    sd.percent = sd.distance * 5;
    sd.left_radio_in = 1.0;

    if (zone < 0.001) {
        sd.percent = 0;
    } else if (zone <= 0.005) {
        sd.percent = 0.01;
    } else if (zone <= 0.01) {
        sd.percent = 0.03;
    }
    return sd;
}

Toolset queryToolset(const std::string& tool_name, const std::string& wobj_name, bool* found)
{
    auto tool = motion::application::movement::move_warden->GetTool(tool_name);
    auto wobj = motion::application::movement::move_warden->GetWobj(wobj_name);
    if (!tool || !wobj) {
        if (found) *found = false;
        return ix::Toolset();
    } else {
        if (found) *found = true;
    }

    ix::Toolset ts;

    auto l = tool->tool_param.rob_hold ? tool->tool_param.tool_load : wobj->wobj_param.wobj_load;
    ts.load = ix::Load(l.mass, imol::Values({l.com_pose.pos.x, l.com_pose.pos.y, l.com_pose.pos.z}), imol::Values({l.ri.ix, l.ri.iy, l.ri.iz}));

    auto t_f = Pose2KdlFrame(tool->tool_param.tool_frame);
    auto w_f = Pose2KdlFrame(wobj->wobj_param.user_frame.user_frame) * Pose2KdlFrame(wobj->wobj_param.wobj_frame);

    if (tool->tool_param.rob_hold) {
        ts.end_in_flan = t_f;
        ts.ref_in_world = w_f;
    } else {
        ts.end_in_flan = w_f;
        ts.ref_in_world = t_f;
    }

    return ts;
}

void refreshBaseCoord()
{
    imol::d<ix::Frame>("ix.coord.base_in_world")->set(motion::basic_interface::GetMechGroupCoord()->GetMechUnitBaseFrame(0));
}

void refreshToolset()
{
    std::string cur_tool_name = motion::application::movement::move_warden->GetCurrentTool()->tool_name;
    std::string cur_wobj_name = motion::application::movement::move_warden->GetCurrentWobj()->wobj_name;
    imol::d<ix::Toolset>("ix.coord.toolset")->set(ix::format::queryToolset(cur_tool_name, cur_wobj_name));
}

}

namespace rtt {

RTT::TaskContext* component(const std::string& name, RTT::TaskContext* ref)
{
    ref = ref ? ref : driver::GetHWComponent();
    return name.empty() ? ref : ref->getPeer(name);
}

RTT::TaskContext* component(const imol::Strings& names, RTT::TaskContext* ref)
{
    for (const auto& name : names) {
        ref = component(name, ref);
        if (!ref) return nullptr;
    }
    return ref;
}

}

}