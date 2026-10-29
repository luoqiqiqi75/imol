#include "lircosformatter.h"

#include <qmath.h>

#include "lircosgrammar.h"

namespace ix {
namespace format {

template<>
bool fromString(const QString &str, DataType &dt)
{
    dt = static_cast<DataType>(LircosGrammar::string2DataType(str));
    return dt != DATA_NONE;
}

template<>
QString toString(const DataType &dt)
{
    return LircosGrammar::dataType2String(dt);
}

/// robotic

template<>
bool fromNode(Node *n, Pos &pos)
{
    if (n->isEmptyMobj()) return false;
    pos.X = n->c(0)->getDouble() * 1000;
    pos.Y = n->c(1)->getDouble() * 1000;
    pos.Z = n->c(2)->getDouble() * 1000;
    return true;
}

template<>
Node * toNode(const Pos &pos, Node *n)
{
    return n->set(n, "#0", pos.X / 1000)->set(n, "#1", pos.Y / 1000)->set(n, "#2", pos.Z / 1000);
}

template<>
bool fromNode(Node *n, Euler &euler)
{
    euler.A = n->c(0)->getDouble() / M_PI * 180;
    euler.B = n->c(1)->getDouble() / M_PI * 180;
    euler.C = n->c(2)->getDouble() / M_PI * 180;
    return true;
}

template<>
Node * toNode(const Euler &euler, Node *n)
{
    return n->set(n, "#0", euler.A / 180 * M_PI)->set(n, "#1", euler.B / 180 * M_PI)->set(n, "#2", euler.C / 180 * M_PI);
}

template<>
bool fromNode(Node *n, Quaternion &quaternion)
{
    quaternion.Q1 = n->c(0)->getDouble();
    quaternion.Q2 = n->c(1)->getDouble();
    quaternion.Q3 = n->c(2)->getDouble();
    quaternion.Q4 = n->c(3)->getDouble();
    return true;
}

template<>
Node * toNode(const Quaternion &quaternion, Node *n)
{
    return n->set(n, "#0", quaternion.Q1)->set(n, "#1", quaternion.Q2)->set(n, "#2", quaternion.Q3)->set(n, "#3", quaternion.Q4);
}

template<>
bool fromNode(Node *n, Orient &orient)
{
    if (n->isEmptyMobj()) return false;
    Node o("o");
    if (n->cmobjCount() == 4) {
        orient.Q1 = n->c(0)->getDouble();
        orient.Q2 = n->c(1)->getDouble();
        orient.Q3 = n->c(2)->getDouble();
        orient.Q4 = n->c(3)->getDouble();
        calc::quaternion2Rot(n, &o);
        orient.A = o.c(0)->getDouble() / M_PI * 180;
        orient.B = o.c(1)->getDouble() / M_PI * 180;
        orient.C = o.c(2)->getDouble() / M_PI * 180;
    } else {
        orient.A = n->c(0)->getDouble() / M_PI * 180;
        orient.B = n->c(1)->getDouble() / M_PI * 180;
        orient.C = n->c(2)->getDouble() / M_PI * 180;
        calc::rot2Quaternion(n, &o);
        orient.Q1 = o.c(0)->getDouble();
        orient.Q2 = o.c(1)->getDouble();
        orient.Q3 = o.c(2)->getDouble();
        orient.Q4 = o.c(3)->getDouble();
        // todo ABC: XYZ / ZYX
    }
    return true;
}

template<>
Node * toNode(const Orient &orient, Node *n)
{
    bool is_q = n->cmobjCount() == 4;
    n->set(n, "#0", orient.Q1);
    n->set(n, "#1", orient.Q2);
    n->set(n, "#2", orient.Q3);
    n->set(n, "#3", orient.Q4);
    return is_q ? n : calc::quaternion2Rot(n);
}

template<>
bool fromNode(Node *n, ConfData &conf)
{
    conf.cf1 = n->c(0)->getDouble();
    conf.cf4 = n->c(1)->getDouble();
    conf.cf6 = n->c(2)->getDouble();
    conf.cfx = n->c(3)->getDouble();
    return n->cmobjCount() == 4;
}

template<>
Node * toNode(const ConfData &conf, Node *n)
{
    return n->set(n, "#0", conf.cf1)->set(n, "#1", conf.cf4)->set(n, "#2", conf.cf6)->set(n, "#3", conf.cfx);
}

template<>
bool fromNode(Node *n, Frame &frame)
{
    if (n->isEmptyMobj()) return false;
    fromNode(n->c("trans"), frame.origin);
    fromNode(n->c("rot"), frame.ori);
    return true;
}

template<>
Node * toNode(const Frame &frame, Node *n)
{
//    n->set(n, "id", frame.id); // bad design
    toNode(frame.origin, createNodeAt(n, "trans"));
    toNode(frame.ori, createNodeAt(n, "rot"));
    return n;
}

template<>
bool fromNode(Node *n, RobPose &robpose)
{
    fromNode(n, robpose.tool_frame);
    fromNode(n->c("conf"), robpose.axis_config);
    return true;
}

template<>
Node * toNode(const RobPose &robpose, Node *n)
{
    toNode(robpose.tool_frame, n);
    toNode(robpose.axis_config, createNodeAt(n, "conf"));
    return n;
}

template<>
bool fromNode(Node *n, ERobJoint &erobjoint)
{
    int i = 0;
    for (auto jn = n->c("near")->first(); jn != n->c("near")->last(); jn = jn->nextBmobj()) {
        erobjoint.robot_joint[i++] = jn->getDouble();
    }
    i = 0;
    for (auto jn = n->c("external")->first(); jn != n->c("external")->last(); jn = jn->nextBmobj()) {
        erobjoint.ext_joint[i++] = jn->getDouble();
    }
    return true;
}

template<>
Node * toNode(const ERobJoint &erobjoint, Node *n)
{
    bool has_ext = false;
    for (int i = 0; i < 6; i++) {
        n->set(n, mName("near", mIndex(i)), erobjoint.robot_joint[i]);
        if (!has_ext && qAbs(erobjoint.ext_joint[i]) > 0.000001) has_ext = true;
    }
    if (has_ext) {
        for (int i = 0; i < 6; i++) {
            n->set(n, mName("external", mIndex(i)), erobjoint.ext_joint[i]);
        }
    } else {
        n->remove(n, "external");
    }
    return n;
}

template<>
bool fromNode(Node *n, Position &position)
{
    fromNode(n, position.rob_pose);
    fromNode(n, position.joint_position);
    return true;
}

template<>
Node * toNode(const Position &position, Node *n)
{
    toNode(position.rob_pose, n);
    toNode(position.joint_position, n);
    return n;
}

/// toolset

template<>
bool fromNode(Node *n, Load &load)
{
    if (n->isEmptyMobj()) return false;
    if (n->hasCmobj("mass")) load.mass = n->c("mass")->getDouble();
    if (n->hasCmobj("inertia")) {
        load.ix = n->r("inertia.#0")->getDouble();
        load.iy = n->r("inertia.#1")->getDouble();
        load.iz = n->r("inertia.#2")->getDouble();
    }
    fromNode(n->c("cog"), load.cog);
    return true;
}

template<>
Node * toNode(const Load &load, Node *n)
{
    n->set(n, "mass", load.mass);
    // no aom
    n->set(n, "inertia.#0", load.ix);
    n->set(n, "inertia.#1", load.iy);
    n->set(n, "inertia.#2", load.iz);
    toNode(load.cog, createNodeAt(n, "cog"));
    return n;
}

template<>
bool fromNode(Node *n, Toolset &toolset)
{
    QString id = n->c("id")->getString();
    if (!id.isEmpty()) {
        toolset.tool.id = id.section(",", 0, 0).toInt();
        toolset.wobj.id = id.section(",", 1, 1).toInt();
    }
    QString key = n->c("key")->getString();
    if (!key.isEmpty()) {
        qstrcpy(toolset.tool.name, key.section(",", 0, 0).toStdString().c_str());
        qstrcpy(toolset.wobj.name, key.section(",", 1, 1).toStdString().c_str());
    }
    fromNode(n->c("load"), toolset.tool.load);
    toolset.tool.rob_hold = true;
    toolset.wobj.rob_hold = false;
    fromNode(n->c("end"), toolset.tool.frame);
    fromNode(n->c("ref"), toolset.wobj.oframe);
    return true;
}

template<>
Node * toNode(const Toolset &toolset, Node *n)
{
    n->set(n, "id", QString("%1,%2").arg(toolset.tool.id).arg(toolset.wobj.id));
    n->set(n, "key", QString("%1,%2").arg(toolset.tool.name, toolset.wobj.name));
    toNode(toolset.tool.load, createNodeAt(n, "load"));
    // no workbench
    if (toolset.tool.rob_hold) {
        toNode(toolset.tool.frame, createNodeAt(n, "end"));
        toNode(toolset.wobj.oframe, createNodeAt(n, "ref"));
    } else {
        toNode(toolset.wobj.oframe, createNodeAt(n, "end"));
        toNode(toolset.tool.frame, createNodeAt(n, "ref"));
    }
    return n;
}

/// project

template<>
bool fromNode(Node *n, CodeVar &cv)
{
    if (n->hasCmobj("_name")) cv.name = n->c("_name")->getString();
    if (cv.type == DATA_NONE) cv.type = fromString<DataType>(n->c("type")->getString());
    switch (cv.type) {
    case DATA_ROBTARGET: cv.value = associateVar<DATA_ROBTARGET>(n); break;
    case DATA_JOINTTARGET: cv.value = associateVar<DATA_JOINTTARGET>(n); break;
    default: cv.value = n->c("value")->getString(); break;
    }
    return true;
}

template<>
Node * toNode(const CodeVar &cv, Node *n)
{
    n->set(n, "name", cv.name);
    n->set(n, "type", toString(cv.type));
    switch (cv.type) {
    case DATA_ROBTARGET: resolveVar<DATA_ROBTARGET>(cv.value, n); break;
    case DATA_JOINTTARGET: resolveVar<DATA_JOINTTARGET>(cv.value, n); break;
    default: n->set(n, "value", cv.value); break;
    }
    return n;
}

template<>
bool fromNode(Node *n, RoutineInfo &ri)
{
    // todo
    auto mgr_req_n = m("xsupport.lircos.manager.request");
    mgr_req_n->set(n, "task.GetCurrentTaskID", QVariant());
    auto mgr_res_n = mgr_req_n->b("response");
    ri.task_id = mgr_res_n->getInt();
    qstrcpy(ri.file_path, n->c("path")->getString().toStdString().c_str());
    qstrcpy(ri.routine_name, n->c("func")->getString().toStdString().c_str());
    return true;
}

/// hmicmd

template<>
Node * toNode(const HMICmd &hmicmd, Node *n)
{
    n->set(n, "cmd", hmicmd.cmd);
    n->set(n, "ext", hmicmd.cmd_extension);
    n->set(n, "data", QVariant::fromValue(QByteArray(hmicmd.data, hmicmd.data_length)));
    return n;
}

/// event

template<>
bool fromNode(Node *n, ThinFilmButtonEvent &e)
{
    QString key = n->c("key")->getString();
    bool pressed = n->c("pressed")->getBool();
    if (key == "back") {
        e.iKey = THIN_FILM_KEY_BACK;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "forward") {
        e.iKey = THIN_FILM_KEY_FORWARD;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f1") {
        e.iKey = THIN_FILM_KEY_F1;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f2") {
        e.iKey = THIN_FILM_KEY_F2;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f3") {
        e.iKey = THIN_FILM_KEY_F3;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f4") {
        e.iKey = THIN_FILM_KEY_F4;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f5") {
        e.iKey = THIN_FILM_KEY_F5;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "f6") {
        e.iKey = THIN_FILM_KEY_F6;
        e.iKeyBoard = KEYBOARD_EVENT_0;
    } else if (key == "start") {
        e.iKey = THIN_FILM_KEY_START;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "stop") {
        e.iKey = THIN_FILM_KEY_STOP;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j1-") {
        e.iKey = THIN_FILM_KEY_JOG1_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j1+") {
        e.iKey = THIN_FILM_KEY_JOG1_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j2-") {
        e.iKey = THIN_FILM_KEY_JOG2_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j2+") {
        e.iKey = THIN_FILM_KEY_JOG2_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j3-") {
        e.iKey = THIN_FILM_KEY_JOG3_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j3+") {
        e.iKey = THIN_FILM_KEY_JOG3_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j4-") {
        e.iKey = THIN_FILM_KEY_JOG4_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j4+") {
        e.iKey = THIN_FILM_KEY_JOG4_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j5-") {
        e.iKey = THIN_FILM_KEY_JOG5_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j5+") {
        e.iKey = THIN_FILM_KEY_JOG5_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j6-") {
        e.iKey = THIN_FILM_KEY_JOG6_SUB;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "j6+") {
        e.iKey = THIN_FILM_KEY_JOG6_ADD;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    } else if (key == "menu") {
        e.iKey = THIN_FILM_KEY_MENU;
        e.iKeyBoard = KEYBOARD_EVENT_1;
    }
    e.eState = pressed ? EFBS_PRESS : EFBS_RELEASE;
    return true;
}

//template<>
//bool fromNode(ix::Node *n, SignalContent &sc)
//{
//}

template<>
Node * toNode(const SignalContent &sc, ix::Node *n)
{
    n->set(n, "id", sc.id);
    n->set(n, "name", QString(sc.name));
    switch (sc.signalType) {
    case SIGNAL_TYPE_DI: n->set(n, "type", "DI"); break;
    case SIGNAL_TYPE_DO: n->set(n, "type", "DO"); break;
    case SIGNAL_TYPE_GI: n->set(n, "type", "GI"); break;
    case SIGNAL_TYPE_GO: n->set(n, "type", "GO"); break;
    case SIGNAL_TYPE_AI: n->set(n, "type", "AI"); break;
    case SIGNAL_TYPE_AO: n->set(n, "type", "AO"); break;
    default: break;
    }
    n->set(n, "board", sc.physicalBoard);
    if (sc.startMappingAddr == sc.endMappingAddr) {
        n->set(n, "addr", sc.startMappingAddr);
    } else {
        n->set(n, "addr.start", sc.startMappingAddr);
        n->set(n, "addr.end", sc.endMappingAddr);
    }
    return n;
}

template<>
Node * toNode(const MaterialPallet &mp, ix::Node *n)
{
    n->set(n, "id", mp.ID);
    n->set(n, "toolset.id", QString("%1,%2").arg(mp.toolID, mp.wobjID));
    n->set(n, "num.column", mp.num_column);
    n->set(n, "num.row", mp.num_row);
    n->set(n, "obj.l", mp.l / 1000);
    n->set(n, "obj.w", mp.w / 1000);
    n->set(n, "obj.h", mp.h / 1000);
    // no dXp dYp
    toNode(mp.start_pos, createNodeAt(n, "point.start"));
    toNode(mp.end_pos, createNodeAt(n, "point.end"));
    toNode(mp.initial_pos_x, createNodeAt(n, "point.x"));
    toNode(mp.initial_pos_y, createNodeAt(n, "point.y"));
    return n;
}

}
}
