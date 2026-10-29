#include "evpploopobject.h"

#include <condition_variable>

#include "evpp/libevent.h"
#include "utils/time.hpp"

using namespace imol;

EvppLoopObject::EvppLoopObject(const std::string& name) : LoopObject(name),
    m_elt(new evpp::EventLoopThread),
    m_loop(m_elt->loop())
{
}

EvppLoopObject::EvppLoopObject(const std::string &name, evpp::EventLoop *current_loop) : LoopObject(name),
    m_elt(nullptr),
    m_loop(current_loop)
{
}

EvppLoopObject::~EvppLoopObject()
{
    if (isRunning()) stop();
    delete m_elt;
}

void EvppLoopObject::addTask(const Task& t)
{
    m_loop->RunInLoop(t);
}

bool EvppLoopObject::isRunning() const
{
    return m_loop->IsRunning();
}

bool EvppLoopObject::hasTask() const
{
    return taskCount() > 0;
}

int EvppLoopObject::taskCount() const
{
    return m_loop->pending_functor_count();
}

bool EvppLoopObject::start()
{
    if (!m_elt) return false;
    if (m_elt->IsRunning()) return true;
    return m_elt->Start();
}

bool EvppLoopObject::stop()
{
    if (!m_elt) return false;
    if (m_elt->IsStopped()) return true;
    m_elt->Stop(true);
    m_elt->event_loop_.reset(m_loop = new evpp::EventLoop);
    return true;
}
