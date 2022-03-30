
#include "log/Log.h"
#include "core/Channel.h"
#include "tcp/TCPServer.h"

namespace lite_http {

void TCPServer::start() {
    assert(!m_acceptor->listening());
    m_loop->run_in_loop(std::bind(&Acceptor::listen, m_acceptor.get()));
}


void TCPServer::establish_conn(int conn_fd, const INetAddress& addr) {
    char buf[128];
    snprintf(buf, sizeof buf, "-%s#%d", m_ip_port.c_str(), conn_fd);
    // TODO thread pool io loop
    std::string key = m_name + buf;
    TCPConnectionPtr conn_sp(new TCPConnection(m_loop, conn_fd, key));

    LOG_INFO("extablish new connection(%s)", key.c_str());
    m_conn_map[key] = conn_sp;

    conn_sp->set_conn_cb(conn_cb);
    conn_sp->set_write_com_cb(write_com_cb);
    conn_sp->set_message_cb(message_cb);
    conn_sp->set_conn_close_cb(std::bind(&TCPServer::remove_connection, this, std::placeholders::_1));
    m_loop->run_in_loop(std::bind(&TCPConnection::establish, conn_sp));
}

void TCPServer::remove_connection(const TCPConnectionPtr& conn_sp) {
    // TODO thread pool io loop
    LOG_INFO("remove connection %s", conn_sp->get_conn_name());
    m_conn_map.erase(conn_sp->get_conn_name());
    EventLoop* loop = conn_sp->get_loop();
    loop->queue_in_loop(std::bind(&TCPConnection::destory, conn_sp));
}

}
