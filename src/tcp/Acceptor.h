
#ifndef _TCPACCEPTOR_H
#define _TCPACCEPTOR_H

#include "core/EventLoop.h"
#include "socket/INetAddress.h"
#include "socket/Socket.h"
#include "util/nocopyable.h"

namespace lite_http {

class Acceptor : private nocopyable {
public:
    Acceptor(EventLoop* event_loop,
             const INetAddress& listen_addr,
             bool is_blocking) 
        : m_event_loop(event_loop),
        m_server(MakeSocket(AF_INET, SOCK_STREAM, is_blocking)),
        m_accept_ch(event_loop, m_server.fd()), 
        m_listening(false) {
        m_server.bind(listen_addr);
        m_accept_ch.set_read_callback(std::bind(&Acceptor::handle_read, this));
    }

    ~Acceptor() = default;

    typedef std::function<void(int, const INetAddress&)> NewConnCallback;

    void listen();
    bool listening() const { return m_listening; }

    void set_new_conn_cb(const NewConnCallback& cb) {
        new_conn_cb = cb;
    }
private:
    void handle_read();
private:
    EventLoop* m_event_loop;
    Socket m_server;
    Channel m_accept_ch;
    bool m_listening;

    NewConnCallback new_conn_cb;
};
}

#endif //LITE_HTTP_TCPACCEPTOR_H
