#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <utility>
#include <iostream>
#include <string>

#include "socket/Socket.h"
#include "core/EventLoop.h"
#include "tcp/TCPConnection.h"
#include "tcp/Acceptor.h"
#include "util/nocopyable.h"


namespace lite_http {
class TCPServer : private nocopyable {
public:
    TCPServer(EventLoop* event_loop,
              const INetAddress& addr,
              const std::string& name,
              bool blocking = true)
        : m_name(name),
        m_ip_port(addr.ip_port_str()),
        m_loop(event_loop), 
        m_acceptor(new Acceptor(event_loop, addr, blocking)) {
        m_acceptor->set_new_conn_cb(
                std::bind(&TCPServer::establish_conn, this, std::placeholders::_1, std::placeholders::_2));
    }
    ~TCPServer() = default;

    void start();
    void set_conn_cb(ConnCallback cb) { conn_cb = std::move(cb); }
    void set_write_com_cb(ConnCallback cb) { write_com_cb = std::move(cb); }
    void set_message_cb(ConnCallback cb) { message_cb = std::move(cb); }

private:
    EventLoop* m_loop;
    std::unique_ptr<Acceptor> m_acceptor;

    int m_thread_num {1};
    const std::string m_name, m_ip_port;
    std::unordered_map<std::string, std::shared_ptr<TCPConnection>> m_conn_map;
    ConnCallback conn_cb, conn_close_cb, write_com_cb, message_cb;

    void establish_conn(int fd, const INetAddress& addr);
    void remove_connection(const std::shared_ptr<TCPConnection>& conn_sp);
};

}

#endif
