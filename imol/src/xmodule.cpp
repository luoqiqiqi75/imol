//
// Created by LuoQi on 2021/12/22.
//

#include "../xmodule.h"

namespace ix {

Module::Module() : imol::Module() {}
Module::~Module() {}

Module* Module::instanceOf(const std::string &module_name) { return dynamic_cast<Module*>(imol::entry::module(module_name)); }

}

