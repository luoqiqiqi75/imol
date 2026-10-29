/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file xsupport.h
 * @brief imol usage for xcore support
 * @author LuoQi
 * @date 2021-11-14
 */

#ifndef IMOL_XSUPPORT_H
#define IMOL_XSUPPORT_H

#include "rtt/TaskContext.hpp"
#include "rtt/InputPort.hpp"
#include "rtt/OutputPort.hpp"
#include "rtt/OperationCaller.hpp"

#include "imol/common.h"

#include "data_serialization/json/serialization.hpp"
#include "data_serialization/json/deserialization.hpp"
#include "configuration/global_mgr.hpp"
#include "robot_algorithms/algo_utils/data_structure_interface.hpp"

namespace ix {

// imol
template<typename T>
inline imol::BasicDataObject<T>* createCfgData(const std::string& name, T* link_ptr = nullptr)
{
    auto serializer = [] (const T& t) { return data_serialization::json::GenerateJson(t); };
    auto deserializer = [] (const imol::JsonRef& jr, T& t) { return data_serialization::json::Deserialize(jr.value(), t); };
    auto dobj_ptr = imol::data::create(name, serializer, deserializer);
    if (link_ptr && dobj_ptr) dobj_ptr->link(link_ptr);
    return dobj_ptr;
}

// static info
namespace info {
std::string version();
}

// original manager
namespace mgr {
std::shared_ptr<configuration::param_mgr::InteractiveParamManager> interactive();
std::string cfgPath(const std::string& file_name);
}

class CartPos;
class Load;
class Toolset;
// data format and conversion
namespace format {

double getPsi(const CartPos& cart_pos);
CartPos& setPsi(CartPos& cart_pos, double psi);
imol::Ints getConf(const CartPos& cart_pos, ConfData* conf = nullptr, int* err = nullptr);

LoadInertia toLoadInertia(const Load& load);
ExpectedSpeed toExpectedSpeed(double speed);
SlipData toSlipData(double zone);

Toolset queryToolset(const std::string& tool_name, const std::string& wobj_name, bool* found = nullptr);

void refreshBaseCoord();
void refreshToolset();

}

// RTT component
namespace rtt {
RTT::TaskContext* component(const std::string& name = "", RTT::TaskContext* ref = nullptr);
RTT::TaskContext* component(const imol::Strings& names, RTT::TaskContext* ref = nullptr);
template<typename T>
T inValue(const RTT::TaskContext* component, const std::string& port_name, bool* found = nullptr)
{
    T t;
    auto port = dynamic_cast<RTT::InputPort<T>*>(component->getPort(port_name));
    if (port) port->getDataSample(t);
    if (found) *found = (port != nullptr);
    return t;
}
template<typename T>
T outValue(const RTT::TaskContext* component, const std::string& port_name, bool* found = nullptr)
{
    auto port = dynamic_cast<RTT::OutputPort<T>*>(component->getPort(port_name));
    if (found) *found = (port != nullptr);
    return port ? port->getLastWrittenValue() : T();
}
template<typename T>
T oprCall(RTT::TaskContext* component, const std::string& opr_name)
{
    RTT::OperationCaller<T()> caller = component->getOperation(opr_name);
    assert(caller.ready());
    std::promise<T> p;
    std::future<T> f = p.get_future();
    imol::loop_rt->addTask([&] { p.set_value(caller()); });
    return f.get();
}
template<typename T, typename... Args>
T oprCall(RTT::TaskContext* component, const std::string& opr_name, Args... args)
{
    RTT::OperationCaller<T(Args...)> caller = component->getOperation(opr_name);
    assert(caller.ready());
    std::promise<T> p;
    std::future<T> f = p.get_future();
    imol::loop_rt->addTask([&] { p.set_value(caller(args...)); });
    return f.get();
}
}

}

#endif // IMOL_XSUPPORT_H