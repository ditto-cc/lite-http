
#include "tcp/Acceptor.h"
#include "log/Log.h"

namespace lite_http {
void Acceptor::listen() {
    m_server.listen();
    m_accept_ch.enable_read();
} 

void Acceptor::handle_read() {
    INetAddress peer;
    int connfd = m_server.accept(peer);
    LOG_INFO("accept %d", connfd);
    if (connfd > 0) {
        if (new_conn_cb)
            new_conn_cb(connfd, peer);
        else
            ::close(connfd);
    } else {
        LOG_FATAL("accept fd < 0.");
    }
}
}
