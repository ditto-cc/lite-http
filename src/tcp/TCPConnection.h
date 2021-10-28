#ifndef _TCP_CONNECTION_H
#define _TCP_CONNECTION_H

namespace lite_http {
    class TCPConnection {
    private:
        int connfd{ -1 };
    public:
        TCPConnection(int connfd) : connfd(connfd) {}
        ~TCPConnection() {}
    };
}

#endif
