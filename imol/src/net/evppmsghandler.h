/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file evppmsghandler.h
 * @brief evpp based message handler
 * @author LuoQi
 * @date 2021-06-29
 */

#ifndef IMOL_INTERNAL_EVPP_MSG_HANDLER_H
#define IMOL_INTERNAL_EVPP_MSG_HANDLER_H

namespace evpp {
class Buffer;
}

namespace imol {
class NetObject;
class EvppMsgHandler
{
public:
    EvppMsgHandler() {}
    virtual ~EvppMsgHandler() {}

    virtual int handle(NetObject* nobj, const std::string& addr, evpp::Buffer* buf) {return -1;}
};
}

#endif // IMOL_INTERNAL_EVPP_MSG_HANDLER_H