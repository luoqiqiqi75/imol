/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file evppservernetobject.h
 * @brief evpp based server net object implementation
 * @author LuoQi
 * @date 2021-06-01
 */

#ifndef IMOL_INTERNAL_EVPP_SERVER_NET_OBJECT_H
#define IMOL_INTERNAL_EVPP_SERVER_NET_OBJECT_H

#include <evpp/tcp_server.h>
#include <evpp/tcp_conn.h>

#include "evppmsghandler.h"
#include "imol/netmanager.h"

namespace imol {

class EvppServerNetObject : public ServerNetObject
{
public:
    explicit EvppServerNetObject(const std::string& name, EvppMsgHandler* msg_handler);
    virtual ~EvppServerNetObject();

    bool startListening(int port, const std::string& ip = "0.0.0.0") override;
    bool stopListening() override;

    bool send(const std::string& addr, const std::string& bytes) const override;
    bool broadcast(const std::string& bytes) const override;

    LoopObject* loopOfClient(const std::string& addr) const override;

private:
    void onConnectionChanged(const evpp::TCPConnPtr& conn);
    void onMessageComing(const evpp::TCPConnPtr& conn, evpp::Buffer* buf);
    bool sendMessage(const evpp::TCPConnPtr& conn, const std::string& bytes) const;

private:
    EvppMsgHandler* m_msg_handler;
    evpp::TCPServer* m_server;
    imol::Map<std::string, evpp::TCPConnPtr> m_conns;
};

}


#endif // IMOL_INTERNAL_EVPP_SERVER_NET_OBJECT_H