
#include "TCPServer.h"


namespace lite_http {

    void TCPServer::start() {
        int listen_fd = m_server.getListenfd();
        Channel ch(listen_fd, EVENT_READ);
        ch.set_read_callback([&]() {
            struct sockaddr_in client_sock;
            socklen_t client_len = sizeof(client_sock);
            int conn_fd = accept(listen_fd, (sockaddr*) &client_sock, &client_len);
            make_nonblocking(conn_fd);

            TCPConnection conn(conn_fd, connectionCompleteCallback,
                               connectionCloseCallback, writeCompleteCallback, messageCallback);
        });
        m_eventloop->add_channel_event(ch.get_fd(), ch);
    }
}