#include "poolloopobject.h"

#include "toolkit/common/global_thread_pool.hpp"

using namespace imol;

PoolLoopObject::PoolLoopObject(const std::string& name) : LoopObject(name)
{
}

void PoolLoopObject::addTask(const Task& t)
{
    toolkit::common::GetGLBThreadPool().commit([=] {
        // task execution
        t();
    });
}

bool PoolLoopObject::isRunning() const
{
    return false;
}

bool PoolLoopObject::hasTask() const
{
    return false;
}
    
bool PoolLoopObject::start()
{
    return true;
}

bool PoolLoopObject::stop()
{
    return true;
}