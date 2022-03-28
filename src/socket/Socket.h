#ifndef _SOCKET_H
#define _SOCKET_H

#include "socket/INetAddress.h"
#include "core/Buffer.h"

namespace lite_http {
class Socket {
public:
    explicit Socket(int fd) : m_fd(fd) {}
    ~Socket() = default;

    int fd() const { return m_fd; }
    void set_reuse_addr(bool on) {
        int opt = on ? 1 : 0;
        setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }

    void set_reuse_port(bool on) {
        int opt = on ? 1 : 0;
        setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    }

    void set_keepalive(bool on) {
        int opt = on ? 1 : 0;
        setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    }

    void set_nodelay(bool on) {
        int opt = on ? 1 : 0;
        setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    }

    void bind(const INetAddress& addr) {
        const sockaddr_in* saddr = addr.get_sockaddr();
        DoBind(m_fd, saddr, sizeof(*saddr));
    }
    
    void listen() {
        DoListen(m_fd, SOMAXCONN);
    }

    int accept(INetAddress& peeraddr) {
        sockaddr_in saddr {};
        ::bzero(&saddr, sizeof saddr);
        int connfd = DoAccept(m_fd, &saddr);
        if (connfd >= 0) {
            peeraddr.set_sockaddr(saddr);
        }
        make_nonblocking(connfd);

        return connfd;
    }

    void shutdown_write() {
        DoShutdownWrite(m_fd);
    }
private:
    int m_fd;
};

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
    int Accept(INetAddress& addr);
    int getListenfd();
    bool is_blocking();
private:
    class Impl;
    std::unique_ptr<Impl> m_pimpl;
};
}

#endif
