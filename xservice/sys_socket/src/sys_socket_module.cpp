//
// Created by LuoQi on 2021/8/17.
//

#include "sys_socket_module.h"

#include "imol/netmanager.h"

#include "state/controller_state_mgr.hpp"
#include "state/states.hpp"
#include "motion/interface/application/movement.hpp"
#include "motion/interface/base/mc_interface.hpp"

IMOL_MODULE(ix.sys_socket, ix::SysSocketModule, 3)

using namespace ix;

Json::Value param2Json(const SysSocketParam& param)
{
    Json::Value value;
    value["enable"] = param.enable;
    value["ip"] = param.ip;
    value["port"] = param.port;
    return value;
}

bool json2Param(const imol::JsonRef& jr, SysSocketParam& param)
{
    jr.at("enable").toT(param.enable);
    jr.at("ip").toT(param.ip);
    jr.at("port").toT(param.port);
    return true;
}

SysSocketModule::SysSocketModule() : Module()
{
    m_param = bindData(imol::data::create(name(), &param2Json, &json2Param))->ptr();

    adoptObj(m_net = imol::net::createClient("sdk", imol::NET_BACKSLASH_R));
    m_net->setHandler(IC_MEMBER_2(&SysSocketModule::handleMsg));

    m_cmd_map["motor_on"] = "sys.power.on";
    m_cmd_map["motor_off"] = "sys.power.off";
    m_cmd_map["switch_mode:auto"] = "sys.mode.auto";
    m_cmd_map["switch_mode:manual"] = "sys.mode.manual";
    m_cmd_map["pp_to_main"] = "project.pp.main";
    m_cmd_map["start"] = "project.start";
    m_cmd_map["stop"] = "project.stop";
    m_cmd_map["clear_alarm"] = "system.servo.clear_alarm";

#define BOOL_QUERY(...) [] { return ic::bool2String(__VA_ARGS__); }

    m_query_map["motor_on_state"] = BOOL_QUERY(ix::isOn());
    m_query_map["robot_running_state"] = BOOL_QUERY(!ix::isReady());
    m_query_map["estop_state"] = BOOL_QUERY((ENGINE_SYSTEM() & state::SSTOP_NON_STOP) != 0);
    m_query_map["operating_mode"] = BOOL_QUERY(ix::isAuto());
    m_query_map["home_state"] = BOOL_QUERY(motion::application::movement::IsAtHome());

    m_query_map["fault_state"] = [] {
        auto rs = RC_STATE() & (state::RcState::RC_FATAL_MASK | state::RcState::RC_ERROR_MASK | state::RcState::RC_BLOCK);
        auto es = ENGINE_SYSTEM() & (state::MotorPowerState::MOTOR_FATAL | state::MotorPowerState::MOTOR_ERROR);
        return ic::bool2String(rs != 0 || es != 0);
    };

    m_query_map["collision_state"] = [] {
        int axis;
        return ic::bool2String(motion::basic_interface::CollisionisDetected(axis));
    };

    m_query_map["cart_pos"] = [] {
        auto cp = ix::curCartPos();
        return cp.translation().m2mm().toString() + "," + cp.rotation(ix::Frame::QUATERNION).toString();
    };
    m_query_map["cart_pos_name"] = [this] { return "cart_pos : " + m_query_map["cart_pos"](); };

    m_query_map["jnt_pos"] = [] { return ix::rt::jntPos().rad2degree().toString(); };
    m_query_map["jnt_pos_name"] = [this] { return "jnt_pos : " + m_query_map["jnt_pos"](); };

    m_query_map["jnt_vel"] = [] { return ix::rt::jntVel().rad2degree().toString(); };
    m_query_map["jnt_vel_name"] = [this] { return "jnt_vel : " + m_query_map["jnt_vel"](); };

    m_query_map["jnt_trq"] = [] { return ix::rt::jntTrq().toString(); };
    m_query_map["jnt_trq_name"] = [this] { return "jnt_trq : " + m_query_map["jnt_trq"](); };
}

void SysSocketModule::init()
{
    registerFuncCmd("connect", [=] { return m_net->connectToHost(m_param->ip, m_param->port); });
    registerFuncCmd("disconnect", [=] { return m_net->disconnectFromHost(); });
}

void SysSocketModule::idle()
{
    if (m_param->enable) m_net->connectToHost(m_param->ip, m_param->port);
}

int SysSocketModule::handleMsg(const std::string&, const std::string& content) const
{
    if (m_cmd_map.has(content)) { // command
        execCmd(m_cmd_map.value(content));
    } else if (m_query_map.has(content)) { // query
        execLater(m_query_map.value(content));
    } // nothing
    return imol::Result::SUCCESS;
}

void SysSocketModule::execCmd(const std::string &cmd) const
{
    imol::command::copy(cmd, [=] (const imol::Result& res) {
        m_net->send(ic::bool2String(res.isSuccess()) + "\r");
    })->start();
}

void SysSocketModule::execLater(const std::function<std::string()> &func) const
{
    imol::loop_main->addTask([=] { m_net->send(func() + "\r"); });
}