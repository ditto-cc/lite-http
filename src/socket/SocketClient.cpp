#include "socket/Socket.h"
#include "log/Log.h"

using std::unique_ptr;

namespace lite_http {

    class SocketClient::Impl {
    public:
        Impl(const char* server, int port) :m_fd(MakeSocket(AF_INET, SOCK_STREAM, false)) {
            LoadConfig(server, port);
        }

        ~Impl() = default;

        Impl(int fd, const struct sockaddr_in &client, socklen_t client_len)
                : m_fd(fd), m_client(client), m_cli_len(client_len) {}

        void LoadConfig(const char *server, int port) {
            // TODO: Read Configuration From Config File
            // SocketClient Config
            m_ser_len = sizeof(m_pimpl->m_server);
            bzero(&m_server, m_ser_len);
            m_server.sin_family = AF_INET;
            inet_pton(AF_INET, server, &m_server.sin_addr);
            m_server.sin_port = htons(port);
        }

    public:
        int m_fd;
        struct sockaddr_in m_client{}, m_server{};
        socklen_t m_cli_len{}, m_ser_len{};
    };


    SocketClient::SocketClient(const char *server, int port) : m_pimpl(new SocketClient::Impl(server, port)) {}

    SocketClient::SocketClient(int fd, const sockaddr_in &client, socklen_t client_len)
            : m_pimpl(new SocketClient::Impl(fd, client, client_len)) {}

    SocketClient::~SocketClient() {
        if (m_pimpl->m_fd >= 0) {
            close(m_pimpl->m_fd);
            AsyncLogger::LogWarn("client socket fd close.");
        }
    }

    int SocketClient::get_fd() const {
        return m_pimpl->m_fd;
    }

    int SocketClient::Connect() {
        return DoConnect(m_pimpl->m_fd, &m_pimpl->m_server, m_pimpl->m_ser_len);
    }

    ssize_t SocketClient::Read(Buffer& buf) {
        ssize_t ret = buf.read_from_fd(m_pimpl->m_fd);
        if (ret < 0) {
            AsyncLogger::LogWarn("client read error. errno(%d)", errno);
        } else if (ret == 0) {
            AsyncLogger::LogWarn("client close.");
        } else {
            AsyncLogger::LogInfo("client read %d bytes.", ret);
        }
        return ret;
    }

    ssize_t SocketClient::Send(Buffer &buf) {
        return buf.write_to_fd(m_pimpl->m_fd);
    }
}
