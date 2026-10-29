//
// Created by LuoQi on 2021/8/17.
//

#ifndef XCORE_SYS_SOCKET_DEF_H
#define XCORE_SYS_SOCKET_DEF_H

#include <string>

namespace ix {

struct SysSocketParam
{
    bool enable = false;
    std::string ip = "192.168.40.1";
    int port = 9898;
};

}


#endif //XCORE_SYS_SOCKET_DEF_H
