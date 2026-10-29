#include "evppservernetobject.h"

#include <iomanip>

#include "logger/logger.hpp"
#include "utils/time.hpp"

#include "imol/common.h"
#include "imol/src/loop/evpploopobject.h"

using namespace imol;

EvppServerNetObject::EvppServerNetObject(const std::string& name, EvppMsgHandler* msg_handler) : ServerNetObject(name),
    m_msg_handler(msg_handler),
    m_server(nullptr)
{

}

EvppServerNetObject::~EvppServerNetObject()
{
    stopListening();
    delete m_msg_handler;
}

bool EvppServerNetObject::startListening(int port, const std::string& ip)
{
    stopListening();

    EvppLoopObject* elobj = dynamic_cast<EvppLoopObject*>(m_loop);
    if (!elobj) {
        // todo
        return false;
    }

    m_server = new evpp::TCPServer(elobj->m_loop, ip + ":" + std::to_string(port), name() + "_server", 1);
    m_server->SetConnectionCallback(IC_MEMBER_1(&EvppServerNetObject::onConnectionChanged));
    m_server->SetMessageCallback(IC_MEMBER_2(&EvppServerNetObject::onMessageComing));
    m_server->Init();

    if (!m_server->Start()) return false;

    m_loop->start();
    return true;
}

bool EvppServerNetObject::stopListening()
{
    if (!m_server) return false;

    m_server->Stop();
    while (!m_server->IsStopped()) { usleep(1); }
    delete m_server;
    m_server = nullptr;

    m_loop->stop();
    return true;
}

std::string addr2LoopName(const std::string& addr)
{
    return "_imol_net_server_" + addr;
}

bool EvppServerNetObject::send(const std::string& addr, const std::string& bytes) const
{
    auto it = m_conns.find(addr);
    return it == m_conns.end() ? false : sendMessage(it->second, bytes);
}

bool EvppServerNetObject::broadcast(const std::string& bytes) const
{
    for (const auto& it : m_conns) {
        sendMessage(it.second, bytes);
    }
    return !m_conns.empty();
}

LoopObject* EvppServerNetObject::loopOfClient(const std::string &addr) const
{
    return imol::loop::get(addr2LoopName(addr));
}

void EvppServerNetObject::onConnectionChanged(const evpp::TCPConnPtr& conn)
{
    LOG(INFO) << "<imol::net>" << "server from " << conn->name() << " " << (conn->IsConnected() ? "connected" : "disconnected");
    if (conn->IsConnected()) {
        m_conns[conn->name()] = conn;
        imol::loop::mgr().add(new EvppLoopObject(addr2LoopName(conn->name()), conn->loop()));
    } else {
        imol::loop::mgr().remove(addr2LoopName(conn->name()));
        m_conns.erase(conn->name());
    }
}

void EvppServerNetObject::onMessageComing(const evpp::TCPConnPtr& conn, evpp::Buffer* buf)
{
    long long dt = utils::time::GetCurrentTimeNs();

    int ret = m_msg_handler->handle(this, conn->name(), buf);
    if (ret < 0) m_err_handler(conn->name(), ret);

    if ((dt = utils::time::GetCurrentTimeNs() - dt) > 10000000) {
        LOG(WARNING) << "<imol::net::evpp>msg handle slower than 10ms: " << std::fixed << std::setprecision(3) << (dt / 1000000.0) << "ms";
    }
}

bool EvppServerNetObject::sendMessage(const evpp::TCPConnPtr& conn, const std::string& bytes) const
{
    if (!conn->IsConnected()) return false;
    conn->Send(bytes);
    return true;
}
