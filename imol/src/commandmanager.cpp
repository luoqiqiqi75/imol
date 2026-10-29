#include "../commandmanager.h"

#include "../common.h"
#include "../module.h"

#define MAX_PROC_CNT 1024

namespace imol {

Result::Result(int code) :
    m_code(code),
    m_data(nullptr)
{
}

Result::Result(bool flag, int error_code) : Result(flag ? SUCCESS : error_code) {}
Result::Result(void) : Result(SUCCESS) {}

int Result::code() const { return m_code; }
DataObject* Result::data() const { return m_data.get(); }

bool Result::isSuccess() const { return m_code == SUCCESS; }
bool Result::isError() const { return m_code < SUCCESS; }
bool Result::isAccepted() const { return m_code > SUCCESS; }

Result::operator bool() const { return isSuccess(); }

Json::Value Result::serialize() const
{
    // simple code
    const JsonInterface* ji = dynamic_cast<const JsonInterface*>(data());
    if (!ji) return m_code;

    // full error info
    Json::Value value;
    if (isError()) value["error"] = m_code;
    if (ji) value["data"] = ji->serialize();
    return value;
}

bool Result::deserialize(const JsonRef &jr)
{
    bool ok = false;

    // only code
    if (jr.objectSize() < 0) {
        m_code = jr.toInt(SUCCESS, &ok);
        return ok;
    }

    // error
    if (auto e = jr.at("error")) {
        m_code = e.toInt(SUCCESS, &ok);
    }
    if (auto d = jr.at("data")) {
        // cannot create T from json
        if (JsonInterface* ji = dynamic_cast<JsonInterface*>(data())) ok = ji->deserialize(d);
    }

    return ok;
}

CommandObject::Procedure::Procedure(const Functor& f, LoopObject* l) : m_f(f), m_l(l ? l : loop_main), m_block(true) {}
CommandObject::Procedure::Procedure(Functor&& f, LoopObject* l) : m_f(std::move(f)), m_l(l ? l : loop_main), m_block(true) {}

Result CommandObject::Procedure::run(CommandObject* cobj) const { return m_f(cobj); }

Map<std::string, int> g_cobj_count;

constexpr const char* CMD_DATA_MGR = "_cmd_data_mgr";

CommandObject::CommandObject(const std::string& name) : Object(name),
    m_mgr(name + CMD_DATA_MGR),
    m_result_handler(NULL)
{
    g_cobj_count[this->name()] = g_cobj_count.value(this->name()) + 1;
}

CommandObject::~CommandObject()
{
    g_cobj_count[this->name()] = g_cobj_count.value(this->name()) - 1;
}

CommandObject* CommandObject::clone() const
{
    CommandObject* obj = new CommandObject(name() + "_copy");
    for (auto& it : m_procedures) {
        obj->appendProc(it);
    }
    return obj;
}

Manager& CommandObject::dataMgr()
{
    return m_mgr;
}

void CommandObject::setResultHandler(const ResultHandler &result_handler)
{
    m_result_handler = result_handler;
}

CommandObject* CommandObject::prependProc(const Procedure &proc)
{
    if (m_procedures.size() < MAX_PROC_CNT) {
        m_procedures.push_front(proc);
    } else {
        LOG(ERROR) << "<imol::command>" << "prepend too many procedures";
    }
    return this;
}

CommandObject* CommandObject::appendProc(const Procedure &proc)
{
    if (m_procedures.sizeAsInt() < MAX_PROC_CNT) {
        m_procedures.push_back(proc);
    } else {
        LOG(ERROR) << "<imol::command>" << "append too many procedures";
    }
    return this;
}

imol::Result CommandObject::start(bool auto_delete)
{
    if (m_procedures.empty()) {
        finish(Result::SUCCESS, auto_delete);
        return Result::SUCCESS;
    }

    Procedure proc = m_procedures.front();
    Procedure::Functor functor = proc.f();

    m_procedures.pop_front();

    proc.l()->addTask([=] {
        // execute task
        Result res = functor(this);

        if (res.isAccepted()) {
            // command accepted
            DLOG(WARNING) << "<imol::command>" << "accepted command: " << name() << " = " << res.code();
        } else if (res.isError() || m_procedures.empty()) {
            // command terminate
            finish(res, auto_delete);
        } else if (res.isSuccess()) {
            // command continue
            start(auto_delete);
        } else {
            LOG(ERROR) << "<imol::command>" << "unprocessed command: " << name();
        }
    });

    return Result::ACCEPT;
}

void CommandObject::report(const Result &result)
{
    if (m_result_handler) m_result_handler(result);
}

void CommandObject::finish(const Result &result, bool auto_delete)
{
    if (!result.isAccepted()) report(result);
    if (auto_delete) loop_main->addTask([=] {delete this;});
}

namespace command {

Manager& mgr()
{
    static Manager m("imol_command_mgr");
    return m;
}

CommandObject* copy(const std::string& name)
{
    if (CommandObject* obj = mgr().get<CommandObject>(name)) {
        obj = obj->clone();
        LOG(INFO) << "<imol::command>" << "copy command: " << name << ", cnt: " << g_cobj_count.value(obj->name());
        return obj;
    }
    LOG(ERROR) << "<imol::command>" << "try to copy non-exist command: " << name;
    return nullptr;
}

CommandObject* copy(const std::string& name, const CommandObject::ResultHandler& result_handler)
{
    CommandObject* new_cobj = copy(name);
    if (new_cobj) new_cobj->setResultHandler(result_handler);
    return new_cobj;
}

bool registerCobj(Module* context, const std::string& key, CommandObject* cobj, bool delete_if_failed)
{
    if (!context || !cobj) return false;
    if (!mgr().add(cobj, delete_if_failed)) return false;
    context->adoptObj(cobj);
    return true;
}

}

}