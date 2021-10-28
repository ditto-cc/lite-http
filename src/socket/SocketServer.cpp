#include "socket/Socket.h"

namespace lite_http {

    class SocketServer::Impl {
    public:
        Impl() {}
        ~Impl() {}
    public:
        struct sockaddr_in m_server;
        socklen_t m_serv_len;
        int m_fd;
        u_int16_t m_port;
        long m_heartbeat_interval;
        void LoadConfig() {
            // TODO: Read Config From File
            u_int16_t port = 12345;
            // SocketServer Config
            m_server.sin_family = AF_INET;
            m_server.sin_addr.s_addr = htonl(INADDR_ANY);
            m_server.sin_port = htons(12345);
            m_serv_len = sizeof(m_pimpl->m_server);

            m_heartbeat_interval = 30;
            m_port = port;
        }
    };

    SocketServer::SocketServer() : m_pimpl(new Impl()) {
        m_pimpl->LoadConfig();
        m_pimpl->m_fd = MakeSocket(m_pimpl->m_server.sin_family, SOCK_STREAM);
    }

    SocketServer::~SocketServer() {
        if (m_pimpl->m_fd >= 0) {
            close(m_pimpl->m_fd);
        }
    }

    int SocketServer::do_bind() {
        return DoBind(m_pimpl->m_fd, &m_pimpl->m_server, m_pimpl->m_serv_len);
    }

    int SocketServer::do_listen() {
        return DoListen(m_pimpl->m_fd, LISTENQ);
    }

    SocketClient SocketServer::do_accept() {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int connfd = DoAccept(m_pimpl->m_fd, &client, &client_len);

        return SocketClient(connfd, client, client_len);
    }

    int SocketServer::read(const SocketClient& sc, Buffer& buf) {
        int ret = Read(sc.get_fd(), buf.get_buf_in(), MAX_LINE);
        
    }
}