/**
 *
 */
#include "socket/Socket.h"
#include "socket/Common.h"
#include "log/Log.h"


namespace lite_http {

    int MakeSocket(int domain, int type) {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            LOG_WARN("create socket error");
        }
        return fd;
    }

    int DoBind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len) {
        int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
        if (ret < 0) {
            LOG_FATAL("bind failed.");
        }
        else {
            LOG_INFO("bind success.");
        }
        return ret;
    }

    int DoListen(int fd, int backlog) {
        int ret = listen(fd, backlog);
        if (ret < 0) {
            LOG_FATAL("listen failed.");
        }
        else {
            LOG_INFO("listen success.");
        }
        return ret;
    }

    int DoAccept(int fd, struct sockaddr_in* client, socklen_t* client_len) {
        int connfd = accept(fd, (struct sockaddr*)client, client_len);
        if (connfd < 0) {
            LOG_WARN("accept failed.");
        }
        else {
            LOG_INFO("accept success.");
        }
        return connfd;
    }

    int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len) {
        int ret = connect(fd, (struct sockaddr*)client, client_len);
        if (ret < 0) {
            LOG_WARN("connect failed.");
        }
        return ret;
    }

    ssize_t Read(int fd, void* buf, size_t size) {
        ssize_t result = read(fd, buf, size);
        if (result < 0) {
            LOG_WARN("Read failed.");
        }
        return result;
    }

    ssize_t Send(int connfd, void* buf, size_t size) {
        ssize_t ret;
        if ((ret = send(connfd, buf, size, 0)) < 0) {
            LOG_WARN("Send failed.");
        }
        return ret;
    }


}