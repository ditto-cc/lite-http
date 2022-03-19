/**
 *
 */
#include "socket/Socket.h"
#include "log/Log.h"


namespace lite_http {

    int MakeSocket(int domain, int type, bool reuse) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            AsyncLogger::LogWarn("create socket error");
        }
        if (reuse) {
            int opt = 1;
            if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt)) < 0)
                AsyncLogger::LogWarn("set socket reuseaddr error");
        }
        return fd;
    }

    int DoBind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len) {
        int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
        if (ret < 0) {
            AsyncLogger::LogFatal("bind failed.");
        }
        else {
            AsyncLogger::LogInfo("bind success.");
        }
        return ret;
    }

    int DoListen(int fd, int backlog) {
        int ret = listen(fd, backlog);
        if (ret < 0) {
            AsyncLogger::LogFatal("listen failed.");
        }
        else {
            AsyncLogger::LogInfo("listen success.");
        }
        return ret;
    }

    int DoAccept(int fd, struct sockaddr_in* client, socklen_t* client_len) {
        int connfd = accept(fd, (struct sockaddr*)client, client_len);
        if (connfd < 0) {
            AsyncLogger::LogWarn("accept failed.");
        }
        else {
            AsyncLogger::LogInfo("accept success.");
        }
        return connfd;
    }

    int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len) {
        int ret = connect(fd, (struct sockaddr*)client, client_len);
        if (ret < 0) {
            AsyncLogger::LogWarn("connect failed.");
        }
        return ret;
    }

    ssize_t DoRead(int fd, void* buf, size_t size) {
        return read(fd, buf, size);
    }

    ssize_t DoSend(int connfd, void* buf, size_t size) {
        ssize_t ret;
        if ((ret = send(connfd, buf, size, 0)) < 0) {
            AsyncLogger::LogWarn("Send failed.");
        }
        return ret;
    }


    void make_nonblocking(int fd) {
        fcntl(fd, F_SETFL, O_NONBLOCK);
    }
}