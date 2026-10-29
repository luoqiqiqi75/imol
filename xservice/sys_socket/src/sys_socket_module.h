//
// Created by LuoQi on 2021/8/17.
//

#ifndef XCORE_SYS_SOCKET_MODULE_H
#define XCORE_SYS_SOCKET_MODULE_H

#include "imol/xmodule.h"

#include "../sys_socket_def.h"

namespace imol {
class ClientNetObject;
}

namespace ix {

class SysSocketModule : public Module
{
public:
    explicit SysSocketModule();

protected:
    void init() override;
    void idle() override;

private:
    int handleMsg(const std::string&, const std::string& content) const;

    typedef std::function<std::string()> QueryFunc;

    void execCmd(const std::string& cmd) const;
    void execLater(const QueryFunc& func) const;

private:
    SysSocketParam* m_param;
    imol::ClientNetObject* m_net;
    imol::Map<std::string, std::string> m_cmd_map;
    imol::Map<std::string, QueryFunc> m_query_map;
};

}

#endif //XCORE_SYS_SOCKET_MODULE_H
