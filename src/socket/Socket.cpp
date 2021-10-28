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
            log_warn("create socket error");
        }
        return fd;
    }

    int DoBind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len) {
        int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
        if (ret < 0) {
            log_warn("bind failed.");
        } else {
            log_info("bind success.");
        }
        return ret;
    }

    int DoListen(int fd, int backlog) {
        int ret = listen(fd, backlog);
        if (ret < 0) {
            log_warn("listen failed.");
        } else {
            log_info("listen success.");
        }
        return ret;
    }

    int DoAccept(int fd, struct sockaddr_in* client, socklen_t* client_len) {
        int connfd = accept(fd, (struct sockaddr*)client, client_len);
        if (connfd < 0) {
            log_warn("accept failed.");
        } else {
            log_info("accept success.");
        }
        return connfd;
    }

    int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len) {
        int ret = connect(fd, (struct sockaddr*)client, client_len);
        if (ret < 0) {
            log_warn("connect failed.");
        }
        return ret;
    }

    ssize_t Read(int fd, void* buf, size_t size) {
        ssize_t result = read(fd, buf, size);
        if (result < 0) {
            log_warn("Read failed.");
        }
        return result;
    }

    ssize_t Send(int connfd, void* buf, size_t size) {
        ssize_t ret;
        if ((ret = send(connfd, buf, size, 0)) < 0) {
            log_warn("Send failed.");
        }
        return ret;
    }


}