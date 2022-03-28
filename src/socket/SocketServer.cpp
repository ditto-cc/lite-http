#include "socket/Socket.h"
#include "log/Log.h"

namespace lite_http {

class SocketServer::Impl {
public:
    explicit Impl(int port = 12345, bool blocking = true) : m_port(port), m_blocking(blocking), m_heartbeat_interval(30) {
        // SocketServer Config
        m_serv_len = sizeof(m_pimpl->m_server);
        bzero(&m_server, sizeof(m_server));
        m_server.sin_family = AF_INET;
        m_server.sin_addr.s_addr = htonl(INADDR_ANY);
        m_server.sin_port = htons(m_port);
        m_fd = MakeSocket(m_server.sin_family, SOCK_STREAM, true);

        int on = 1;
        setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);

        if (!blocking)
            make_nonblocking(m_fd);
    }
    ~Impl() = default;
public:
    struct sockaddr_in m_server {};
    socklen_t m_serv_len {};
    int m_fd;
    u_int16_t m_port;
    bool m_blocking;
    long m_heartbeat_interval {30};
};

SocketServer::SocketServer() : m_pimpl(new Impl()) {}

SocketServer::SocketServer(int port,
                           bool blocking) 
: m_pimpl(new Impl(port, blocking)) {

}

SocketServer::~SocketServer() {
    if (m_pimpl->m_fd >= 0) {
        close(m_pimpl->m_fd);
        AsyncLogger::LogWarn("server socket fd close.");
    }
}

int SocketServer::Bind() {

    return DoBind(m_pimpl->m_fd, &m_pimpl->m_server, m_pimpl->m_serv_len);
}

int SocketServer::Listen() {
    return DoListen(m_pimpl->m_fd, LISTENQ);
}

int SocketServer::Accept(INetAddress& addr) {
    struct sockaddr_in client{};
    int connfd = DoAccept(m_pimpl->m_fd, &client);
    addr.set_sockaddr(client);
    return connfd;
}

int SocketServer::getListenfd() {
    return m_pimpl->m_fd;
}

bool SocketServer::is_blocking() {
    return m_pimpl->m_blocking;
}

}
