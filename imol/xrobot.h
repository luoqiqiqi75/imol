/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file xrobot.h
 * @brief imol usage for robot
 * @author LuoQi
 * @date 2021-04-22
 */

#ifndef IMOL_XROBOT_H
#define IMOL_XROBOT_H

#include "imol/common.h"

#include "kdl/jntarray.hpp"
#include "kdl/frames.hpp"

struct FeedbackInfo;
struct JntPosControlInfo;

namespace ix {

// robot work rt loop
extern imol::LoopObject* loop_robot;

// robot info
int robotJointNum();
int robotRedundantJointNum();
int robotExternalJointNum();
std::string robotType();
bool robotIsCobot();

// robot condition control
enum ErrorCode : int
{
    SUCCESS                 = 0,
    PREFIX                  = -0x0000, // top-level prefix, -1 is occupied
    ERR_LAUNCHER            = -0x0002,
    ERR_SAFETY_STOP         = -0x0003,
    ERR_RC_OPERATION_MODE   = -0x0010,
    ERR_RC_POWER_STATE      = -0x0011,
    ERR_RC_TASK_SPACE       = -0x0012,
    ERR_RC_CONTROL_MODE     = -0x0013,
    ERR_RC_ROBOT_ACTION     = -0x0014,
    ERR_CALC_IK             = -0x0020
};

// robot condition: rc_index definition
enum RCIndex : unsigned short
{
    RC_OPERATION_MODE = 0,  // MANUAL = 0, AUTO = 1
    RC_POWER_STATE,         // OFF = 0, ON = 1, OTHER = ERROR
    RC_TASK_SPACE,          // NORMAL = TS, NORMAL | READY = TS << 8, NORMAL | NORMAL = (TS << 8) | TS ...
    RC_CONTROL_MODE         // POSITION = 0, TORQUE = 1
};

// robot condition: rc_value definition
const int RC_ANY = -1;      // index do not check
const int RC_DEFAULT = 0;   // index related value if null

const int RC_MANUAL = RC_DEFAULT;
const int RC_AUTO   = 1;

const int RC_OFF    = RC_DEFAULT;
const int RC_ON     = 1;

const int RC_READY  = RC_DEFAULT;

const int RC_POSITION   = RC_DEFAULT;
const int RC_TORQUE     = 1;

// robot condition: rc_checker saves rc_values ordered by rc_index
typedef imol::Ints RCChecker;

bool isAuto();
bool isOn();
bool isReady();

bool checkOperationMode(int rc_value);
bool checkPowerState(int rc_value);
bool checkTaskSpace(int rc_value);
bool checkControlMode(int rc_value);

/**
 * @brief robot condition control: check all robot conditions within rc_checker
 * @param rc_checker is the rc_values container, will check every index
 * @param enable_default_checker will take care some basic & unusual conditions, such as safety stop and robot action
 * @return validation error code
 */
ErrorCode robotCondGuard(const RCChecker& rc_checker, bool enable_default_checker = true);

// realtime data
namespace rt {

/**
 * ts: to servo
 * fs: from servo
 */
namespace raw {

namespace ctrl {
imol::Values ts_pos_cmd();
imol::Values ts_trq_cmd();
FeedbackInfo fs_feedback();
JntPosControlInfo fs_pos_info();
}

}

imol::Values jntPos();
imol::Values jntVel();
imol::Values jntTrq();
imol::Values motorCurrent();

imol::Values jntFollowingError();
}

class JntArray : public KDL::JntArray, public imol::JsonInterface
{
public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(JntArray, JntArray, KDL::JntArray)

    JntArray(const imol::Values& joints);

    imol::Values toValues() const;

    size_t size() const;
    int sizeAsInt() const;
    bool empty() const;

    double jnt(int i) const;
    JntArray& setJnt(int i, double jnt);

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

class Frame : public KDL::Frame, public imol::JsonInterface
{
public:
    enum RotationType {
        AUTO,           // RPY or QUATERNION
        XYZ,            // RPY
        ZYX,            // Euler ZYX
        ZYZ,            // Euler ZYZ
        QUATERNION      // Quaternion
    };

public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(Frame, Frame, KDL::Frame)

    Frame(const imol::Values& translation, const imol::Values& rotation);

    imol::Values translation() const;
    Frame& setTranslation(const imol::Values& values);

    imol::Values rotation(RotationType rt = AUTO) const;
    Frame& setRotation(const imol::Values& values, RotationType rt = AUTO);

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

class Load : public imol::JsonInterface
{
public:
    double mass = 0;
    KDL::Vector cog;
    KDL::Vector inertia;

public:
    Load();
    Load(double mass, const KDL::Vector& cog, const KDL::Vector& inertia);
    Load(double mass, const imol::Values& cog, const imol::Values& inertia);

    imol::Values toValues() const;

    bool isNull() const;

    imol::Values getCog() const;
    Load& setCog(const imol::Values& cog);

    imol::Values getInertia() const;
    Load& setInertia(const imol::Values& inertia);

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

class Toolset : public imol::JsonInterface
{
public:
    Load load;
    Frame end_in_flan;
    Frame ref_in_world;

public:
    Toolset();
    Toolset(const Load& load, const Frame& end_in_flan, const Frame& ref_in_world);

    Frame calcFlanInBase(const Frame& ref_in_end) const;
    Frame calcEndInRef(const Frame& flan_in_base) const;

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

class CartPos;
/**
 * @brief JntPos is the absolute position of a series of joints
 */
class JntPos : public JntArray
{
public:
    JntArray external;

public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(JntArray, JntPos, JntArray)

    JntPos(const JntArray& joints, const JntArray& external);

    void setJntArray(const JntArray& jnt_array);

    CartPos fk() const;

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

/**
 * @brief CartPos is the complete description of posture, witch stands for the flan in base frame
 */
class CartPos : public Frame
{
public:
    JntArray near;
    JntArray redundant;
    JntArray external;

public:
    IMOL_INHERIT_CONSTRUCTOR_IMPL(Frame, CartPos, Frame)

    CartPos(const Frame& frame, const JntArray& near, const JntArray& redundant, const JntArray& external);

    void setFrame(const Frame& frame);

    JntPos ik(int* ret = nullptr) const;

    Json::Value serialize() const override;
    bool deserialize(const imol::JsonRef& jr) override;
};

Toolset curToolset();
JntPos curJntPos();
CartPos curCartPos();

Frame baseFrame();

}

// output macros
#define IX_RCG(...) if (int rcg = ix::robotCondGuard({__VA_ARGS__})) return rcg

#endif // IMOL_XROBOT_H