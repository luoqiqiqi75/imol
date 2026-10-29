#include "rtloopobject.h"

#include <rtt/Activity.hpp>

#include "logger/logger.hpp"
#include "RTTI/threads_def.hpp"

using namespace imol;

#define RT_PERIOD 0.001

RtLoopObject::RtLoopObject(const std::string& name) : RTT::TaskContext(name + "_rtt"), LoopObject(name)
{
    auto activity = new RTT::Activity(ORO_SCHED_RT, RTTI::RT_THREAD_PRIORITY_NET_PROCESSOR, RT_PERIOD,
                                    RTTI::RT_CPU_IDX_LARGE_COMPUTATION_TASK, 0, name + "_act");
    setActivity(activity);
}

void RtLoopObject::addTask(const Task& t)
{
    if (!m_tasks.try_enqueue(t)) {
        DLOG(ERROR) << "enqueue failed!";
    }
}

bool RtLoopObject::isRunning() const
{
    return RTT::TaskContext::isRunning();
}

bool RtLoopObject::hasTask() const
{
    return m_tasks.size_approx() > 0;
}
    
bool RtLoopObject::start()
{
    return RTT::TaskContext::start();
}

bool RtLoopObject::stop()
{
    return RTT::TaskContext::stop();
}

void RtLoopObject::updateHook()
{
    List<Task> ts;

    // task prepare
    while (hasTask()) {
        Task t;
        if (!m_tasks.try_dequeue(t) || !t) {
            DLOG(ERROR) << "<imol::loop::rt>dequeue failed!"; // impossible
            break;
        }
        ts.push_back(t);
    }

    // task execution
    for (auto& task : ts) {
        // todo realtime performance control
        task();
    }
}
