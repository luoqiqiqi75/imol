/**
 * Copyright(C) 2019 Rokae Technology Co., Ltd.
 * All Rights Reserved.
 *
 * Information in this file is the intellectual property of Rokae Technology Co., Ltd,
 * And may contains trade secrets that must be stored and viewed confidentially.
 *
 * @file netmanager.h
 * @brief global network interface
 * @author LuoQi
 * @date 2021-05-24
 */

#ifndef IMOL_NET_MANAGER_H
#define IMOL_NET_MANAGER_H

#include "base.h"

namespace imol {

typedef std::function<int(const std::string&, const std::string&)> NetHandler;
typedef std::function<void(const std::string&, int)> NetErrorHandler;

class LoopObject;

class ServerInterface
{
public:
    virtual bool startListening(int port, const std::string& ip = "0.0.0.0");
    virtual bool stopListening();

    virtual bool send(const std::string& addr, const std::string& content) const;
    virtual bool broadcast(const std::string& content) const;

    virtual LoopObject* loopOfClient(const std::string& addr) const;
};

class ClientInterface
{
public:
    virtual bool connectToHost(const std::string& ip, int port);
    virtual bool disconnectFromHost();

    virtual bool send(const std::string& content) const;
};

class NetObject : public Object
{
public:
    explicit NetObject(const std::string& name = "");
    virtual ~NetObject();

    LoopObject* lobj() const;

    NetHandler handler() const;
    void setHandler(const NetHandler& handler);

    NetErrorHandler errorHandler() const;
    void setErrorHandler(const NetErrorHandler& err_handler);

protected:
    LoopObject* m_loop;
    NetHandler m_handler;
    NetErrorHandler m_err_handler;
};

class ServerNetObject : public NetObject, public ServerInterface
{ public: using NetObject::NetObject; };

class ClientNetObject : public NetObject, public ClientInterface
{ public: using NetObject::NetObject; };

enum NetType {
    NET_RAW,
    NET_CACHE,
    NET_BACKSLASH_R
};

namespace net {
Manager& mgr();
NetObject* get(const std::string& name);

ServerNetObject* createServer(const std::string& name, NetType type);
ServerNetObject* server(const std::string& name);

ClientNetObject* createClient(const std::string& name, NetType type);
ClientNetObject* client(const std::string& name);
}

}

#endif //IMOL_NET_MANAGER_H