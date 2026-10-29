#include "../netmanager.h"

#include <evpp/tcp_server.h>
#include <evpp/tcp_conn.h>

#include "../common.h"
#include "net/evppservernetobject.h"
#include "net/evppclientnetobject.h"

#define NO_IMPL(_FUNC) LOG(WARNING) << "<imol::net>" << this << " - no implementation: " << #_FUNC; return NULL

namespace imol {

bool ServerInterface::startListening(int, const std::string&) { NO_IMPL(startListening); }
bool ServerInterface::stopListening() { NO_IMPL(stopListening); }
bool ServerInterface::send(const std::string&, const std::string&) const { NO_IMPL(send); }
bool ServerInterface::broadcast(const std::string&) const { NO_IMPL(broadcast); }
LoopObject* ServerInterface::loopOfClient(const std::string &addr) const { NO_IMPL(loopOfClient); }

bool ClientInterface::connectToHost(const std::string& ip, int port) { NO_IMPL(connectToHost); }
bool ClientInterface::disconnectFromHost() { NO_IMPL(disconnectFromHost); }
bool ClientInterface::send(const std::string& content) const { NO_IMPL(send); }

NetObject::NetObject(const std::string& name) : Object(name)
{
    m_handler = [] (const std::string&, const std::string&) { return 0; };
    m_err_handler = [] (const std::string&, int) {};

    m_loop = loop::create("_imol_net_" + name, LOOP_EVPP);
}

NetObject::~NetObject()
{
    loop::mgr().remove(m_loop);
}

LoopObject* NetObject::lobj() const
{
    return m_loop;
}

NetHandler NetObject::handler() const
{
    return m_handler;
}

void NetObject::setHandler(const NetHandler& handler)
{
    m_handler = handler;
}

NetErrorHandler NetObject::errorHandler() const
{
    return m_err_handler;
}

void NetObject::setErrorHandler(const NetErrorHandler& err_handler)
{
    m_err_handler = err_handler;
}

namespace net {

Manager& mgr()
{
    static Manager m("imol_net_mgr");
    return m;
}

NetObject* get(const std::string& name)
{
    NetObject* obj = mgr().get<NetObject>(name);
    return obj;
}

class RawMsgHandler : public EvppMsgHandler
{
public:
    virtual int handle(NetObject* nobj, const std::string& addr, evpp::Buffer* buf) override
    {
        return nobj->handler()(addr, buf->NextAllString());
    }
};

class CacheMsgHandler : public EvppMsgHandler
{
public:
    virtual int handle(NetObject* nobj, const std::string& addr, evpp::Buffer* buf) override
    {
        int ret = 0;
        std::string bytes = buf->NextAllString();
        if (m_bytes.size() == 0) {
            ret = nobj->handler()(addr, bytes);
            if (ret > 0 && ret < static_cast<int>(bytes.size())) m_bytes.append(bytes.substr(ret, bytes.size() - ret));
        } else {
            m_bytes.append(bytes);
            ret = nobj->handler()(addr, m_bytes);
            if (ret > 0) m_bytes.erase(0, std::min(ret, static_cast<int>(m_bytes.size())));
        }

        if (m_bytes.size() > 256 * 1024 * 1024) {
            // todo
            m_bytes.clear();
        }

        return ret;
    }

private:
    std::string m_bytes;
};

class BackslashRMsgHandler : public EvppMsgHandler
{
protected:
    virtual int handle(NetObject* nobj, const std::string& addr, evpp::Buffer* buf) override
    {
        std::string bytes = m_bytes + buf->NextAllString();
        int pre_i = 0, next_i = 0;
        while ((next_i = bytes.find('\r', pre_i)) >= 0) {
            int ret = nobj->handler()(addr, bytes.substr(pre_i, next_i - pre_i));
            if (ret < 0 && nobj->errorHandler()) nobj->errorHandler()(addr, ret);

            pre_i = next_i + 1;
        }

        if (bytes.size() > 0 && bytes[bytes.size() - 1] == '\r') {
            m_bytes.clear();
        } else {
            m_bytes = (pre_i == 0 ? bytes : bytes.substr(pre_i, bytes.size() - pre_i));
        }
        return 0;
    }

private:
    std::string m_bytes;
};

ServerNetObject* createServer(const std::string& name, NetType type)
{
    ServerNetObject* obj = nullptr;
    switch (type) {
    case NET_RAW: obj = new EvppServerNetObject(name, new RawMsgHandler()); break;
    case NET_CACHE: obj = new EvppServerNetObject(name, new CacheMsgHandler()); break;
    case NET_BACKSLASH_R: obj = new EvppServerNetObject(name, new BackslashRMsgHandler()); break;
    default: break;
    }
    if (obj) mgr().add(obj);
    return obj;
}

ServerNetObject* server(const std::string& name) { return dynamic_cast<ServerNetObject*>(get(name)); }

ClientNetObject* createClient(const std::string& name, NetType type)
{
    ClientNetObject* obj = nullptr;
    switch (type) {
    case NET_RAW: obj = new EvppClientNetObject(name, new RawMsgHandler()); break;
    case NET_CACHE: obj = new EvppClientNetObject(name, new CacheMsgHandler()); break;
    case NET_BACKSLASH_R: obj = new EvppClientNetObject(name, new BackslashRMsgHandler()); break;
    default: break;
    }
    if (obj) mgr().add(obj);
    return obj;
}

ClientNetObject* client(const std::string& name) { return dynamic_cast<ClientNetObject*>(get(name)); }

}

}
