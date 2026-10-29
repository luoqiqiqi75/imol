#include "../xrobot.h"

#include "../xsupport.h"

#include "state/controller_state_mgr.hpp"
#include "state/states.hpp"
#include "driver/driver_manager.hpp"
#include "driver/executor/hardware.hpp"
#include "RTTI/hw_interface.hpp"
#include "motion/interface/base/mc_interface.hpp"

IMOL_REGISTER_VERSION(xrobot, 4)

auto g_coord_base = imol::data::create("ix.coord.base_in_world", ix::Frame())->readonly();
auto g_coord_toolset = imol::data::create("ix.coord.toolset", ix::Toolset());

#define MGR_ROBOT configuration::GLBConfigurationMgr::GetInstance()->RobParamMgr

namespace ix {

imol::LoopObject* loop_robot = nullptr;

int robotJointNum() { return MGR_ROBOT()->GetRobotModelConfiguration()->axes_cnt; }
int robotRedundantJointNum() { return robotJointNum() > 6 ? robotJointNum() - 6 : 0; }
int robotExternalJointNum() { return 0; }
std::string robotType() { return MGR_ROBOT()->GetRobotModelName(); }
bool robotIsCobot() { return !motion::basic_interface::GetMechGroupModel()->GetRobotModel()->TheRobotIsIndustrialRobot(); }

bool isAuto() { return OPERATION_MODE() == state::CTRL_MODE_AUTO; }
//bool isOn() { return dynamic_cast<driver::Hardware*>(driver::GetHWComponent())->IsPowerOn(); }
bool isOn() { return MOTOR_POWER_STATE() == state::MOTOR_POWER_ON; }
bool isReady() { return TASK_SPACE() == state::TS_READY; }

bool checkOperationMode(int rc_value) { return rc_value == RC_ANY || rc_value == (isAuto() ? RC_AUTO : RC_MANUAL); }
bool checkPowerState(int rc_value) { return rc_value == RC_ANY || rc_value == (isOn() ? RC_ON : RC_OFF); }

bool checkTaskSpace(int rc_value)
{
    if (rc_value == RC_ANY) return true;
    
    int ts = TASK_SPACE(), v = rc_value;
    bool match = false;
    do {
        if (ts == (v & 0xFF)) {
            match = true;
            break;
        } else {
            v = v >> 8;
        }
    } while (v > 0);
    return match;
}

bool checkControlMode(int rc_value) { return rc_value == RC_ANY || rc_value == MOTOR_CONTROL_MODE(); }

int checkerValue(const RCChecker& checker, unsigned short index)
{
    return index >= 0 && index < checker.size() ? checker[index] : 0;
}

ErrorCode robotCondGuard(const RCChecker& rc_checker, bool enable_default_checker)
{
    // system checker
    if (enable_default_checker) {
        // system error

        // safety stop
        if ((ENGINE_SYSTEM() & state::SSTOP_NON_STOP) != 0) return ERR_SAFETY_STOP;
    }

    // rc checker
    if (!checkOperationMode(checkerValue(rc_checker, RC_OPERATION_MODE))) return ERR_RC_OPERATION_MODE; // operation mode
    if (!checkPowerState(checkerValue(rc_checker, RC_POWER_STATE))) return ERR_RC_POWER_STATE; // power state
    if (!checkTaskSpace(checkerValue(rc_checker, RC_TASK_SPACE))) return ERR_RC_TASK_SPACE; // task space
    if (!checkControlMode(checkerValue(rc_checker, RC_CONTROL_MODE))) return ERR_RC_CONTROL_MODE; // control mode

    // no important checker
    if (enable_default_checker) {
        // robot action
        if (ROBOT_ACTION() != state::RobotAction::RA_IDLE) return ERR_RC_ROBOT_ACTION;
    }

    return SUCCESS;
}

namespace rt {

namespace raw {

namespace ctrl {
typedef std::pair<std::vector<double>, std::vector<boost::shared_ptr<data::motion::Trigger>>> JntPosCmdPair;
imol::Values ts_pos_cmd() { return rtt::inValue<JntPosCmdPair>(driver::GetHWComponent(), RTTI::PORT_NAME_JNT_POS_CMD).first; }
imol::Values ts_trq_cmd() { return rtt::inValue<std::vector<double>>(driver::GetHWComponent(), RTTI::PORT_NAME_JNT_TRQ_CMD); }
FeedbackInfo fs_feedback() { return rtt::outValue<FeedbackInfo>(driver::GetHWComponent(), RTTI::PORT_NAME_FEEDBACK_INFO); }
JntPosControlInfo fs_pos_info() { return rtt::outValue<JntPosControlInfo>(driver::GetHWComponent(), RTTI::PORT_NAME_JNT_POS_CONTROL_INFO); }
}

}

imol::Values jntPos() { return raw::ctrl::fs_feedback().jnt_pos_measured; }
imol::Values jntVel() { return raw::ctrl::fs_feedback().jnt_vel_measured; }
imol::Values jntTrq() { return robotIsCobot() ? raw::ctrl::fs_feedback().jnt_trq_sensor_measured : raw::ctrl::fs_feedback().jnt_trq_measured; }
imol::Values motorCurrent() { return raw::ctrl::fs_feedback().motor_current_measured; }

imol::Values jntFollowingError() { return raw::ctrl::fs_pos_info().jnt_following_err; }

}

JntArray::JntArray(const imol::Values &joints) : KDL::JntArray(joints.size())
{
    for (size_t i = 0; i < joints.size(); ++i) {
        (*this)(i) = joints.at(i);
    }
}

imol::Values JntArray::toValues() const
{
    imol::Values values(rows());
    for (size_t i = 0; i < rows(); i++){
        values[i] = (*this)(i);
    }
    return values;
}

size_t JntArray::size() const { return rows(); }
int JntArray::sizeAsInt() const { return static_cast<int>(rows()); }
bool JntArray::empty() const { return rows() == 0; }

double JntArray::jnt(int i) const
{
    return i >= 0 && i < sizeAsInt() ? (*this)(i) : 0;
}

JntArray& JntArray::setJnt(int i, double jnt)
{
    if (i >= 0 && i < sizeAsInt()) (*this)(i) = jnt;
    return *this;
}

Json::Value JntArray::serialize() const
{
    return ic::array2Json(toValues());
}

bool JntArray::deserialize(const imol::JsonRef& jr)
{
    imol::Values values = jr.toValues();
    if (values.size() != rows()) resize(values.size());
    for (size_t i = 0; i < values.size(); ++i) {
        (*this)(i) = values.at(i);
    }
    return true;
}

Frame::Frame(const imol::Values& translation, const imol::Values& rotation) : KDL::Frame()
{
    setTranslation(translation).setRotation(rotation);
}

imol::Values Frame::translation() const
{
    return imol::Values({p.x(), p.y(), p.z()}).setUnit(imol::Values::M);
}

Frame& Frame::setTranslation(const imol::Values &values)
{
    p.x(values.value(0));
    p.y(values.value(1));
    p.z(values.value(2));
    return *this;
}

imol::Values Frame::rotation(RotationType rt) const
{
    if (rt == AUTO) rt = XYZ; // todo read from settings
    double a = 0, b = 0, c = 0, w = 0;
    switch (rt) {
    case XYZ: M.GetRPY(a, b, c); break;
    case ZYX: M.GetEulerZYX(a, b, c); break;
    case ZYZ: M.GetEulerZYZ(a, b, c); break;
    case QUATERNION: M.GetQuaternion(a, b, c, w); break;
    default: break;
    }
    return rt == QUATERNION ? imol::Values({a, b, c, w}) : imol::Values({a, b, c}).setUnit(imol::Values::RAD);
}

Frame& Frame::setRotation(const imol::Values &values, RotationType rt)
{
    if (rt == AUTO) rt = values.size() == 4 ? QUATERNION : XYZ; // todo changes from settings
    double a = values.value(0), b = values.value(1), c = values.value(2), w = values.value(3);
    switch (rt) {
    case XYZ: M = KDL::Rotation::RPY(a, b, c); break;
    case ZYX: M = KDL::Rotation::EulerZYX(a, b, c); break;
    case ZYZ: M = KDL::Rotation::EulerZYZ(a, b, c); break;
    case QUATERNION: M = KDL::Rotation::Quaternion(a, b, c, w);
    default: break;
    }
    return *this;
}

Json::Value Frame::serialize() const
{
    Json::Value value;
    value["trans"] = ic::array2Json(translation());
    value["rot"] = ic::array2Json(rotation());
    return value;
}

bool Frame::deserialize(const imol::JsonRef& jr)
{
    if (auto trans = jr.at("trans")) setTranslation(trans.toValues());
    if (auto rot = jr.at("rot")) setRotation(rot.toValues());
    return true;
}

Load::Load() : mass(0) {}
Load::Load(double mass, const KDL::Vector& cog, const KDL::Vector& inertia) : mass(mass), cog(cog), inertia(inertia) {}
Load::Load(double mass, const imol::Values& cog, const imol::Values& inertia) : mass(mass)
{
    setCog(cog);
    setInertia(inertia);
}

imol::Values Load::toValues() const { return {mass, cog.x(), cog.y(), cog.z(), inertia.x(), inertia.y(), inertia.z()}; }

bool Load::isNull() const { return toValues().equals(Load().toValues()); }

imol::Values Load::getCog() const { return {cog.x(), cog.y(), cog.z()}; }
Load& Load::setCog(const imol::Values& cog)
{
    this->cog.x(cog[0]);
    this->cog.y(cog[1]);
    this->cog.z(cog[2]);
    return *this;
}

imol::Values Load::getInertia() const { return {inertia.x(), inertia.y(), inertia.z()}; }
Load& Load::setInertia(const imol::Values &inertia)
{
    this->inertia.x(inertia[0]);
    this->inertia.y(inertia[1]);
    this->inertia.z(inertia[2]);
    return *this;
}

Json::Value Load::serialize() const
{
    Json::Value value;
    value["mass"] = mass;
    value["cog"] = ic::array2Json(getCog());
    value["inertia"] = ic::array2Json(getInertia());
    return value;
}

bool Load::deserialize(const imol::JsonRef &jr)
{
    if (auto m = jr.at("mass")) mass = m.toDouble();
    if (auto c = jr.at("cog")) setCog(c.toValues());
    if (auto i = jr.at("inertia")) setInertia(i.toValues());
    return true;
}

Toolset::Toolset() {}
Toolset::Toolset(const Load &load, const Frame &end_in_flan, const Frame &ref_in_world) : load(load), end_in_flan(end_in_flan), ref_in_world(ref_in_world) {}

Frame Toolset::calcFlanInBase(const Frame& end_in_ref) const { return baseFrame().Inverse() * ref_in_world * end_in_ref * end_in_flan.Inverse(); }
Frame Toolset::calcEndInRef(const Frame& flan_in_base) const { return ref_in_world.Inverse() * baseFrame() * flan_in_base * end_in_flan; }

Json::Value Toolset::serialize() const
{
    Json::Value value;
//    if (!load.isNull()) value["load"] = load.serialize();
    value["load"] = load.serialize();
    value["end"] = end_in_flan.serialize();
    value["ref"] = ref_in_world.serialize();
    return value;
}

bool Toolset::deserialize(const imol::JsonRef &jr)
{
    if (auto l = jr.at("load")) load.deserialize(l);
    if (auto e = jr.at("end")) end_in_flan.deserialize(e);
    if (auto r = jr.at("ref")) ref_in_world.deserialize(r);
    return true;
}

JntPos::JntPos(const JntArray& joints, const JntArray& external) : JntArray(joints), external(external)
{
}

void JntPos::setJntArray(const JntArray& jnt_array)
{
    if (rows() != jnt_array.rows()) resize(jnt_array.rows());
    for (int i = 0; i < static_cast<int>(rows()); i++) {
        setJnt(i, jnt_array.jnt(i));
    }
}

CartPos JntPos::fk() const
{
    auto fk_solver = motion::basic_interface::GetFkSolverPos();
    auto ik_solver = motion::basic_interface::GetIkSolverPos();

    CartPos cp(Frame(), *this, JntArray(robotRedundantJointNum()), external);
    fk_solver->JntToCart(*this, cp);
    cp.setFrame(curToolset().calcEndInRef(cp));

    // todo bad design
    if (!cp.redundant.empty()) {
        double psi = 0;
        ik_solver->GetCurPsi(*this, psi);
        format::setPsi(cp, psi);
    }
    return cp;
}

Json::Value JntPos::serialize() const
{
    Json::Value value = JntArray::serialize();
    for (int i = 0; i < external.sizeAsInt(); i++) {
        value[i + sizeAsInt()] = external.jnt(i);
    }
    return value;
}

bool JntPos::deserialize(const imol::JsonRef& jr)
{
    if (jr.arraySize() >= robotJointNum()) {
        if (sizeAsInt() != robotJointNum()) resize(robotJointNum());
        for (int i = 0; i < robotJointNum(); i++) {
            setJnt(i, jr.at(i).toDouble());
        }
    }
    if (jr.arraySize() >= robotJointNum() + robotExternalJointNum()) {
        if (external.sizeAsInt() != robotExternalJointNum()) external.resize(robotExternalJointNum());
        for (int i = 0; i < robotExternalJointNum(); i++) {
            external.setJnt(i, jr.at(i + robotJointNum()).toDouble());
        }
    }
    return true;
}

CartPos::CartPos(const Frame& frame, const JntArray& near, const JntArray& redundant, const JntArray& external) :
    Frame(frame), near(near), redundant(redundant), external(external)
{
}

void CartPos::setFrame(const Frame& frame)
{
    p = frame.p;
    M = frame.M;
}

JntPos CartPos::ik(int* ret) const
{
    auto ik_solver = motion::basic_interface::GetIkSolverPos();

    JntPos jp(JntArray(robotJointNum()), external);
    auto gf = GeneralizedFrame(curToolset().calcFlanInBase(*this), format::getPsi(*this));
    int result = ik_solver->CartToJnt(near.empty() ? curJntPos() : near, gf, jp);
    if (ret) {
        *ret = result;
        if (result == 0 && !jp.fk().translation().equals(translation())) *ret = ERR_CALC_IK; // for sure
    }
    return jp;
}

Json::Value CartPos::serialize() const
{
    Json::Value value = Frame::serialize();
    if (!near.empty()) value["near"] = near.serialize();
    if (!external.empty()) value["external"] = external.serialize();

    // todo bad design
    if (redundant.size() == 1) {
        value["elb"] = format::getPsi(*this);
    } else if (!redundant.empty()) {
        value["redundant"] = redundant.serialize();
    }

    return value;
}

bool CartPos::deserialize(const imol::JsonRef& jr)
{
    Frame::deserialize(jr);
    if (auto n = jr.at("near")) near.deserialize(n);
    if (auto e = jr.at("external")) external.deserialize(e);

    // todo bad design
    if (auto elb = jr.at("elb")) {
        format::setPsi(*this, elb.toDouble());
    } else if (auto i = jr.at("redundant")) {
        redundant.deserialize(i);
    }
    if (auto c = jr.at("conf")) {
        auto ik_solver = motion::basic_interface::GetIkSolverPos();
        JntPos jp(JntArray(robotJointNum()), external);
        ConfData conf;
        conf.Set(c[7].toInt(), c[0].toInt(), c[1].toInt(), c[2].toInt(), c[3].toInt(), c[4].toInt(), c[5].toInt(), c[6].toInt());
        int result = ik_solver->CartToJntWithConf(GeneralizedFrame(curToolset().calcFlanInBase(*this), format::getPsi(*this)), conf, jp);
        if (result != 0) return false;
    }

    return true;
}

Toolset curToolset() { return g_coord_toolset->get(); }
JntPos curJntPos() { return JntPos(rt::jntPos()); }
CartPos curCartPos() { return curJntPos().fk(); }

Frame baseFrame() { return g_coord_base->get(); }

}