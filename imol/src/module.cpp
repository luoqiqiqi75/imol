#include "../module.h"

#include "imol/common.h"

namespace imol {

Module::Module() : Object() {}
Module::~Module() {}

std::string Module::name() const { return Object::name() == IMOL_UNDEFINED_OBJECT_NAME ? entry::creatingModuleName() : Object::name(); }
std::string Module::name(const std::string &suffix) const { return suffix.empty() ? name() : name() + "." + suffix; }

Module::ModuleStateSignal Module::state() const { return m_state; }

void Module::startToInit()
{
    trigger(m_state = GONNA_INIT);
    std::string key = name();
    init();
    if (entry::module(key)) trigger(m_state = INITED);
}

void Module::startToIdle()
{
    trigger(m_state = GONNA_IDLE);
    std::string key = name();
    idle();
    if (entry::module(key)) trigger(m_state = IDLED);
}

void Module::startToDeinit()
{
    trigger(m_state = GONNA_DEINIT);
    std::string key = name();
    deinit();
    if (entry::module(key)) trigger(m_state = DEINITED);
}

void Module::adoptObj(Object *obj)
{
    connect(DELETE, obj, [=] { delete obj; });
}

void Module::abandonObj(Object *obj, bool auto_delete)
{
    disconnect(DELETE, obj);
    if (auto_delete) delete obj;
}

void Module::init() {}
void Module::idle() {}
void Module::deinit() {}

}