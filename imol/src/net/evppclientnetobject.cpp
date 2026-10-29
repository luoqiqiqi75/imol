#include "evppclientnetobject.h"

#include <iomanip>

#include "logger/logger.hpp"
#include "utils/time.hpp"

#include "imol/common.h"
#include "imol/src/loop/evpploopobject.h"

using namespace imol;

EvppClientNetObject::EvppClientNetObject(const std::string& name, EvppMsgHandler* msg_handler) : ClientNetObject(name),
    m_msg_handler(msg_handler),
    m_client(nullptr),
    m_conn(NULL),
    m_is_connected(false)
{

}

EvppClientNetObject::~EvppClientNetObject()
{
    delete m_client;
    delete m_msg_handler;
}

bool EvppClientNetObject::connectToHost(const std::string& ip, int port)
{
    if (m_client && m_conn && m_conn->IsConnected()) return false;

    EvppLoopObject* elobj = dynamic_cast<EvppLoopObject*>(m_loop);
    if (!elobj) {
        // todo
        return false;
    }

    m_client = new evpp::TCPClient(elobj->m_loop, ip + ":" + std::to_string(port), name() + "_client");
    m_client->SetConnectionCallback(IC_MEMBER_1(&EvppClientNetObject::onConnectionChanged));
    m_client->SetMessageCallback(IC_MEMBER_2(&EvppClientNetObject::onMessageComing));
    m_client->set_auto_reconnect(false);

    m_loop->start();
    m_client->Connect();
    return true;
}

bool EvppClientNetObject::disconnectFromHost()
{
    if (!m_client || !m_conn || !m_conn->IsConnected()) return false;

    m_client->Disconnect();
    m_loop->stop();

    delete m_client;
    m_client = nullptr;
    m_conn.reset();

    return true;
}

bool EvppClientNetObject::send(const std::string& content) const
{
    if (!m_client || !m_conn || !m_conn->IsConnected()) return false;
    return sendMessage(m_conn, content);
}

void EvppClientNetObject::onConnectionChanged(const evpp::TCPConnPtr& conn)
{
    if (conn->IsConnected() != m_is_connected) {
        LOG(INFO) << "<imol::net>" << "client to " << conn->name() << " " << (conn->IsConnected() ? "connected" : "disconnected");
        m_is_connected = conn->IsConnected();
    }
    m_conn = conn->IsConnected() ? conn : NULL;
}

void EvppClientNetObject::onMessageComing(const evpp::TCPConnPtr& conn, evpp::Buffer* buf)
{
    long long dt = utils::time::GetCurrentTimeNs();

    int ret = m_msg_handler->handle(this, m_conn->name(), buf);
    if (ret < 0) m_err_handler(m_conn->name(), ret);

    if ((dt = utils::time::GetCurrentTimeNs() - dt) > 10000000) {
        LOG(WARNING) << "<imol::net::evpp>msg handle slower than 10ms: " << std::fixed << std::setprecision(3) << (dt / 1000000.0) << "ms";
    }
}

bool EvppClientNetObject::sendMessage(const evpp::TCPConnPtr& conn, const std::string& bytes) const
{
    if (!conn->IsConnected()) return false;
    conn->Send(bytes);
    return true;
}