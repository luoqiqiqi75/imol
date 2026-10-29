/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file evppclientnetobject.h
 * @brief evpp based client net object implementation
 * @author LuoQi
 * @date 2021-06-29
 */

#ifndef IMOL_INTERNAL_EVPP_CLIENT_NET_OBJECT_H
#define IMOL_INTERNAL_EVPP_CLIENT_NET_OBJECT_H

#include <evpp/tcp_client.h>
#include <evpp/tcp_conn.h>

#include "evppmsghandler.h"
#include "imol/netmanager.h"

namespace imol {

class EvppClientNetObject : public ClientNetObject
{
public:
    explicit EvppClientNetObject(const std::string& name, EvppMsgHandler* msg_handler);
    virtual ~EvppClientNetObject();

    bool connectToHost(const std::string& ip, int port) override;
    bool disconnectFromHost() override;

    bool send(const std::string& content) const override;

private:
    void onConnectionChanged(const evpp::TCPConnPtr& conn);
    void onMessageComing(const evpp::TCPConnPtr& conn, evpp::Buffer* buf);
    bool sendMessage(const evpp::TCPConnPtr& conn, const std::string& bytes) const;

private:
    EvppMsgHandler* m_msg_handler;
    evpp::TCPClient* m_client;
    evpp::TCPConnPtr m_conn;
    bool m_is_connected;
};

}

#endif // 