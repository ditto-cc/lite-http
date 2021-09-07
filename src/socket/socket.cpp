#include "common.h"
#include "log/lite_log.h"


namespace lite_http {

    int Socket() {
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if (fd < 0) {
            log_fatal("create socket error");
        }
        return fd;
    }

    int Bind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len) {
        int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
        if (ret < 0) {
            log_fatal("bind failed.");
        }
        return ret;
    }

    int Listen(int fd, int backlog) {
        int ret = listen(fd, backlog);
        if (ret < 0) {
            log_fatal("listen failed.");
        }
        return ret;
    }

    int Accept(int fd, struct sockaddr_in* client, socklen_t* client_len) {
        int connfd = accept(fd, (struct sockaddr*)client, client_len);
        if (connfd < 0) {
            log_fatal("accept failed.");
        }
        return connfd;
    }

    int Connect(int fd, struct sockaddr_in* client, socklen_t client_len) {
        int ret = connect(fd, (struct sockaddr*)client, client_len);
        if (ret < 0) {
            log_fatal("connect failed.");
        }
        return ret;
    }

    ssize_t Read(int fd, void* buf, size_t size) {
        ssize_t result = read(fd, buf, size);
        if (result < 0) {
            log_fatal("Read failed.");
        }
        return result;
    }

    ssize_t Send(int connfd, void* buf, size_t size) {
        ssize_t ret;
        if ((ret = send(connfd, buf, size, 0)) < 0) {
            log_fatal("Send failed.");
        }
        return ret;
    }
}