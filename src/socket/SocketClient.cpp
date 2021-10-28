#include "socket/Socket.h"

namespace lite_http {

    class SocketClient::Impl {
    public:
        Impl() {}
        Impl(int fd, const struct sockaddr_in& client, socklen_t client_len)
            : m_fd(fd), m_client(client), m_cli_len(client_len) {}
        ~Impl() {}
    public:
        int m_fd;
        struct sockaddr_in m_client;
        socklen_t m_cli_len;
    };

    SocketClient::SocketClient() : m_pimpl(new SocketClient::Impl()) {}

    SocketClient::SocketClient(int fd, const struct sockaddr_in& client, socklen_t client_len)
        : m_pimpl(new SocketClient::Impl(fd, client, client_len)) {}

    SocketClient::~SocketClient() {
        if (m_pimpl->m_fd >= 0)
            close(m_pimpl->m_fd);
    }

    int SocketClient::get_fd() const {
        return m_pimpl->m_fd;
    }
}