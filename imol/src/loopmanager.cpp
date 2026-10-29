#include "../loopmanager.h"

#include "logger/logger.hpp"
#include "utils/uuid.hpp"

#include "loop/rtloopobject.h"
#include "loop/evpploopobject.h"
#include "loop/poolloopobject.h"

namespace imol {

LoopObject::LoopObject(const std::string& name) : Object(name)
{
}

void LoopObject::addTask(const Task&)
{
    LOG(WARNING) << "<imol::loop>" << this << " - no implementation: " << "addTask";
}

//void LoopObject::processTasks()
//{
//}
//
//bool LoopObject::execUntil(const Condition& cond, double timeout_ms, double interval_ms)
//{
//    LOG(WARNING) << "<imol::loop>" << this << " - no implementation: " << "execUntil";
//    return false;
//}

bool LoopObject::isRunning() const
{
    return false;
}

bool LoopObject::hasTask() const
{
    return false;
}

int LoopObject::taskCount() const
{
    return 0;
}

bool LoopObject::start()
{
    LOG(WARNING) << "<imol::loop>" << this << " - no implementation: " << "start";
    return false;
}

bool LoopObject::stop()
{
    return false;
}

namespace loop {

Manager& mgr()
{
    static Manager m("imol_loop_mgr");
    return m;
}

LoopObject* get(const std::string& name, bool default_pool)
{
    LoopObject* obj = mgr().get<LoopObject>(name);
    if (obj || !default_pool) return obj;

    static LoopObject* null_obj = new PoolLoopObject("");
    return null_obj;
}

LoopObject* create(const std::string &name, LoopType type) {
    LoopObject* obj = nullptr;
    switch (type) {
        case LOOP_EVPP: obj = new EvppLoopObject(name); break;
        case LOOP_RTT: obj = new RtLoopObject(name); break;
        case LOOP_POOL: obj = new PoolLoopObject(name); break;
        default: break;
    }
    if (obj) mgr().add(obj);
    return obj;
}

}

}