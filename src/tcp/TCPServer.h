#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <utility>

#include "socket/Socket.h"
#include "core/EventLoop.h"
#include "tcp/TCPConnection.h"
#include "tcp/TCPAcceptor.h"


namespace lite_http {
    class TCPServer {
    private:
        SocketServer m_server;
        std::shared_ptr<EventLoop> m_eventloop;
        int m_thread_num {1};

        Callback connectionCompleteCallback;
        Callback connectionCloseCallback;
        Callback writeCompleteCallback;
        Callback messageCallback;
    public:
        explicit TCPServer(u_int16_t port, bool blocking, std::shared_ptr<EventLoop> eventloop)
        : m_server(port, blocking), m_eventloop(std::move(eventloop)) {}
        ~TCPServer() = default;


        void start();
    };
}

#endif
