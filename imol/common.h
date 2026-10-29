/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file common.h
 * @brief imol interface
 * @author LuoQi
 * @date 2021-04-22
 */

#ifndef IMOL_COMMON_H
#define IMOL_COMMON_H

#include "datamanager.h"
#include "loopmanager.h"
#include "commandmanager.h"

#include "logger/logger.hpp"

namespace imol {

// global unique id
extern const std::string uid;

// module control
class Module;
typedef Creator<Module*()> ModuleCreator;
ModuleCreator& moduleCreator();

// version control
typedef Creator<int()> VersionController;

/**
 * @brief VersionController is extremely useful for finer defined versions, the version saved in
 * a \class CreatorManager instance, and can be obtained by means of CreatorManager methods
 * @return global version manager
 */
VersionController& ver();
int ver(const std::string& key);

// loop control
extern LoopObject* loop_main;
extern LoopObject* loop_rt;
extern LoopObject* loop_calc;

// rt support
namespace rt {
long long clock();
bool hasTask(Module* context);
void createTask(Module* context, const Task& task, bool unique_check = false);
void removeTasks(Module* context);
}

// main entrance
namespace entry {

Module* module(const std::string& name);
bool createModule(const std::string& key, bool fix = true);
std::string creatingModuleName();

/**
 * @brief Es geht los! = Ready? go!
 *
 * Main start entry of imol instance
 */
void losgehts();

/**
 * @brief Tschüß! = Good bye!
 *
 * Main stop entry of imol instance
 */
void tschuess();

}

}

// output macro
#define P_IMOL_INSERT_VER(_KEY, _VER) imol::ver().insert({#_KEY, [] (void) -> int { return _VER; }})
#define P_IMOL_INSERT_MODULE(_KEY, _CLASS_NAME) imol::moduleCreator().insert({#_KEY, [] () -> imol::Module* { return new _CLASS_NAME(); }})
#define IMOL_REGISTER_VERSION(_KEY, _VER) IMOL_AUTO_RUN(P_IMOL_INSERT_VER(_KEY, _VER);)
#define IMOL_REGISTER_MODULE(_KEY, _CLASS_NAME) IMOL_AUTO_RUN(_IMOL_INSERT_MODULE(_KEY, _CLASS_NAME);)
#define IMOL_MODULE(_KEY, _CLASS_NAME, _VER) IMOL_AUTO_RUN(P_IMOL_INSERT_MODULE(_KEY, _CLASS_NAME); P_IMOL_INSERT_VER(_KEY, _VER);)

// imol common defines
namespace ic {

// json utils
bool mergeJson(Json::Value& dst, const Json::Value& src, bool strict = false);

std::string jsonSerialize(const Json::Value& value);
Json::Value jsonDeserialize(const std::string& str, std::string* err_str = nullptr);

template<class Array>
Json::Value array2Json(const Array& array)
{
    Json::Value value;
    value.resize(array.size());
    int cnt = 0;
    for (const auto& it : array) {
        value[cnt++] = it;
    }
    return value;
}

template<class Map>
Json::Value map2Json(const Map& map)
{
    Json::Value value;
    for (const auto& kv : map) {
        value[kv.first] = kv.second;
    }
    return value;
}

Json::Value readJson(const std::string& path);
bool writeJson(const std::string& path, const Json::Value& value);

// path control
std::string pathOf(imol::Module* module, const std::string& file_name, const std::string& suffix, const std::string& prefix);
std::string pathOf(imol::Module* module, const imol::Object* obj, const std::string& suffix, const std::string& prefix = "./module");

// useful functions
std::string bool2String(bool b);

// manager utils
typedef imol::CommandObject::Procedure Proc;
imol::CommandObject* registerCommand(imol::Module* context, const std::string& key, const Proc& proc);

// super short standard simple usages
namespace reg {

template<typename F> imol::CommandObject* cmd(imol::Module* context, const std::string& key, F f)
{ return registerCommand(context, key, Proc::from(f)); }

template<typename F> imol::CommandObject* cmd(imol::Module* context, const std::string& key, F f, Proc::FuncIn<F> t)
{ return registerCommand(context, key, Proc::from(f, t)); }

template<typename F, typename... Args> imol::CommandObject* cmd(imol::Module* context, const std::string& key, F f, Args... args)
{ return registerCommand(context, key, Proc(std::bind(f, args...))); }

}

}

// imol common macro
#define IC_MEMBER_0(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this)
#define IC_MEMBER_1(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1)
#define IC_MEMBER_2(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1, std::placeholders::_2)
#define IC_MEMBER_3(_CLASS_MEMBER) std::bind(_CLASS_MEMBER, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#endif // IMOL_COMMON_H