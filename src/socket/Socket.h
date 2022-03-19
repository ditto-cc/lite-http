#ifndef _SOCKET_H
#define _SOCKET_H

#include "socket/Common.h"
#include "core/Buffer.h"

namespace lite_http {
    class SocketClient {
    public:
        SocketClient(const char* server, int port);
        SocketClient(int fd, const struct sockaddr_in& client, socklen_t client_len);
        SocketClient(const SocketClient&);
        SocketClient(SocketClient&&);
        ~SocketClient();

        int get_fd() const;
        int Connect();
        ssize_t Read(Buffer& buf);
        ssize_t Send(Buffer& buf);

    private:
        class Impl;
        std::unique_ptr<Impl> m_pimpl;
    };

    class SocketServer {
    public:
        SocketServer();
        explicit SocketServer(int port, bool blocking);
        ~SocketServer();

        int Bind();
        int Listen();
        SocketClient Accept();
        int getListenfd();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pimpl;
    };

    int MakeSocket(int domain, int type, bool reuse);
    int DoBind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len);
    int DoListen(int fd, int backlog);
    int DoAccept(int fd, struct sockaddr_in* client, socklen_t* client_len);
    int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len);

    void make_nonblocking(int fd);

    ssize_t DoRead(int connfd, void* buffer, size_t size);
    ssize_t DoSend(int connfd, void* buffer, size_t size);
}

#endif
