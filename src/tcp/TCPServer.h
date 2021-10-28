#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include "socket/Common.h"
#include "core/Callback.h"
#include "TCPConnection.h"

namespace lite_http {
    void make_nonblocking(int fd) {
        fcntl(fd, F_SETFL, O_NONBLOCK);
    }

    class TCPServer {
    private:
        int listenfd;
        u_int32_t port;
        bool blocking{false};
        struct sockaddr_in serv;
    public:
        explicit TCPServer(u_int16_t port, bool blocking = false);
        ~TCPServer() {}

        TCPConnection accept();

        void readCallback();

        void writeCallback();
    };
}

#endif