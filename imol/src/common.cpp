#include "../common.h"
#include "../module.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include <fstream>

#include "support/CPPUtils.h"
#include "utils/json/json_util.hpp"
#include "utils/uuid.hpp"
#include "utils/fs/fs.hpp"
#include "logger/logger.hpp"

#ifdef DEBUG
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define PURPLE  "\033[1;35m"
#define RESET   "\033[0m"
#else
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define PURPLE  ""
#define RESET   ""
#endif

#define IMOL_VERSION "2.1"

namespace imol {

const std::string id_path = "./imol_id";

std::string generateID()
{
    std::string unique_id;

    // load current id
    std::ifstream id_if(id_path);
    if (id_if.is_open()) {
        std::getline(id_if, unique_id);
        id_if.close();
        if (!unique_id.empty()) return unique_id;
    }

    // generate new id
    unique_id = utils::uuid::GenerateUUID();
    std::ofstream id_of(id_path);
    if (id_of.is_open()) {
        id_of << unique_id << std::endl;
        id_of.close();
    } else {
        unique_id = IMOL_UNDEFINED_OBJECT_NAME;
        LOG(FATAL) << "<imol>cannot create id file: " << id_path;
    }
    return unique_id;
}

const std::string uid = generateID();

ModuleCreator& moduleCreator()
{
    static ModuleCreator c("imol_creator_module");
    return c;
}

VersionController& ver()
{
    static VersionController mgr("imol_creator_version");
    return mgr;
}

int ver(const std::string& key)
{
    return ver().exec(key);
}

LoopObject* loop_main = nullptr;
LoopObject* loop_rt = nullptr;
LoopObject* loop_calc = nullptr;

namespace rt {

imol::TDataObject<long long>* g_clock = data::from<long long>(-1, "imol.rt.clock");

long long clock() { return g_clock->get(); }

bool hasTask(Module* context)
{
    return g_clock->hasConnection(DataObject::CHANGED, context);
}

void createTask(Module* context, const Task& task, bool unique_check)
{
    if (!context || (unique_check && hasTask(context))) return;
    LOG(INFO) << "<imol::rt>" << "create a RT task by: " << context->name();
    g_clock->connect(DataObject::CHANGED, context, task);
}

void removeTasks(Module* context)
{
    LOG(INFO) << "<imol::rt>" << "remove all RT tasks from: " << context->name();
    g_clock->disconnect(context);
}

}

namespace entry {

Manager g_modules("imol_mgr_module");
std::string g_last_module_name;

Module* module(const std::string& key) { return g_modules.get<Module>(key); }

bool createModule(const std::string& key, bool fix)
{
    if (module(key)) return false;
    g_last_module_name = key;
    Module* m = moduleCreator().exec(key);
    if (!m) return false;
    g_modules[key] = m;
    if (fix) g_modules.fixObjectLinks();
    return true;
}

std::string creatingModuleName()
{
    return g_last_module_name;
}

void losgehts()
{
    if (!g_modules.empty()) return;

    DLOG(INFO) << "<imol>++++++ start ++++++";

    // identification
    LOG(INFO) << "<imol::info>" << "version [" << GREEN << IMOL_VERSION << RESET << "] uuid [" << PURPLE << uid  << RESET << "]";

    // prepare loops
    (loop_main = loop::create("_imol_entry_main", LOOP_EVPP))->start();
    (loop_rt = loop::create("_imol_entry_rt", LOOP_RTT))->start();
    (loop_calc = loop::create("_imol_entry_calc", LOOP_POOL))->start();

    // rt support
    rt::g_clock->connect(DataObject::CHANGED, rt::g_clock, [=] {
        loop_rt->addTask([=] { rt::g_clock->update(rt::clock() + 1); });
    });
    rt::g_clock->update(0);

    // create modules
    for (const auto& key : moduleCreator().keys()) {
        LOG(INFO) << "<imol>module " << BLUE << "create" << RESET << " : " << GREEN << key << RESET << " (version: " << ver(key) << ")";
        if (!createModule(key, false)) {
            LOG(ERROR) << "<imol> create module: " << key << " failed!";
        }
    }
    g_modules.fixObjectLinks();
    g_last_module_name = IMOL_UNDEFINED_OBJECT_NAME;

    // init all modules
    for (auto it : g_modules.values()) {
        LOG(INFO) << "<imol>module " << PURPLE << "init" << RESET << " : " << GREEN << it->name() << RESET;
        dynamic_cast<Module*>(it)->startToInit();
    }

    // idle all modules
    for (auto it : g_modules.values()) {
        LOG(INFO) << "<imol>module " << YELLOW << "idle" << RESET << " : " << GREEN << it->name() << RESET;
        dynamic_cast<Module*>(it)->startToIdle();
    }

    DLOG(INFO) << "<imol>------ start ------";
}

void tschuess()
{
    DLOG(INFO) << "<imol>++++++ stop ++++++";

    // deinit all modules
    for (auto it : g_modules.values()) {
        LOG(INFO) << "<imol>module " << PURPLE << "deinit" << RESET << " : " << GREEN << it->name() << RESET;
        dynamic_cast<Module*>(it)->startToDeinit();
    }
    
    // delete modules
    for (auto& it : g_modules.values()) {
        LOG(INFO) << "<imol>module " << BLUE << "delete" << RESET << " : " << GREEN << it->name() << RESET;
        delete it;
    }

    DLOG(INFO) << "<imol> ------ stop ------";
}

}

}

namespace ic {

bool mergeJson(Json::Value& dst, const Json::Value& src, bool strict)
{
    if (dst.isObject()) {
        if (!src.isObject()) return false;
        // object merge to object
        for (const std::string& key : dst.getMemberNames()) {
            if (src.isMember(key)) {
                if (!mergeJson(dst[key], src[key]) && strict) return false;
            }
        }
    } else if (dst.isArray()) {
        if (strict && !src.isArray()) return false;
        dst = src;
    } else {
        if (strict && !src.isConvertibleTo(dst.type())) return false;
        dst = src;
    }
    return true;
}

std::string jsonSerialize(const Json::Value& value)
{
    std::string raw = Json::FastWriter().write(value);
    raw[raw.length() - 1] = '\r'; // ending control, instead of \n
    return raw;
}

Json::Value jsonDeserialize(const std::string& str, std::string* err_str)
{
    Json::Value value;
    Json::Reader reader;
    if (reader.parse(str, value)) return value;
    if (err_str) *err_str = reader.getFormattedErrorMessages();
    return value;
}

Json::Value readJson(const std::string& path)
{
    Json::Value value;
    utils::json::LoadFromFile(value, path);
    if (value.isNull()) {
        LOG(WARNING) << "<ic>read invalid json file: " << path;
    }
    return value;
}

bool writeJson(const std::string& path, const Json::Value& value)
{
    if (!utils::json::SaveToFile(value, path)) return false;
    boost::filesystem::permissions(path, boost::filesystem::all_all);
    return true;
}

std::string pathOf(imol::Module* module, const std::string& file_name, const std::string& suffix, const std::string& prefix)
{
    std::string path = prefix + "/" + antlrcpp::replaceString(module ? module->name() : ".", "::", "/");
    utils::fs::CreateFolderIfNotExist(path);
    boost::filesystem::permissions(path, boost::filesystem::all_all);
    if (!file_name.empty()) path += "/" + file_name;
    return suffix.empty() ? path : path + "." + suffix;
}

std::string pathOf(imol::Module* module, const imol::Object* obj, const std::string& suffix, const std::string& prefix)
{
    return pathOf(module, obj ? obj->name() : "", suffix, prefix);
}

std::string bool2String(bool b)
{
    return b ? "true" : "false";
}

imol::CommandObject* registerCommand(imol::Module* context, const std::string& key, const Proc& proc)
{
    imol::CommandObject* cobj = new imol::CommandObject(key);
    return imol::command::registerCobj(context, key, cobj->appendProc(proc), true) ? cobj : nullptr;
}

}