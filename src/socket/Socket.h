#ifndef _SOCKET_H
#define _SOCKET_H

#include "socket/Common.h"
#include "core/Buffer.h"

namespace lite_http {
    class SocketClient {
    public:
        SocketClient();
        SocketClient(int fd, const struct sockaddr_in& client, socklen_t client_len);
        SocketClient(const SocketClient&);
        SocketClient(SocketClient&&);
        ~SocketClient();

        int get_fd() const;
        int do_connect();
    private:
        class Impl;
        unique_ptr<Impl> m_pimpl;
    };

    class SocketServer {
    public:
        SocketServer();
        ~SocketServer();

        int do_bind();
        int do_listen();
        SocketClient do_accept();

        int read(const SocketClient&, Buffer&);
    private:
        class Impl;
        unique_ptr<Impl> m_pimpl;
    };

    int MakeSocket(int domain, int type);
    int DoBind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len);
    int DoListen(int fd, int backlog);
    int DoAccept(int fd, struct sockaddr_in* client, socklen_t* client_len);
    int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len);

    ssize_t Read(int connfd, void* buffer, size_t size);
    ssize_t Send(int connfd, void* buffer, size_t size);
}

#endif