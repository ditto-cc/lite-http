#ifndef _COMMON_H
#define _COMMON_H
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <endian.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <memory>

#include "log/Log.h"

namespace lite_http {
typedef unsigned long long size_t;

const long MAX_LINE = 4096;
const int LISTENQ = 1024;


inline uint32_t Host2Network32(uint32_t x) {
    return htobe32(x);
}

inline uint16_t Host2Network16(uint16_t x) {
    return htobe16(x);
}

inline uint32_t Network2Host32(uint32_t x) {
    return be32toh(x);
}

inline uint16_t Network2Host16(uint16_t x) {
    return be16toh(x);
}

inline void make_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

inline int MakeSocket(int domain, int type, bool blocking) {
    int fd = socket(domain, type, 0);
    if (fd < 0) {
        AsyncLogger::LogWarn("create socket error");
    }
    if (!blocking) {
        make_nonblocking(fd);
    }
    return fd;
}

inline int DoBind(int fd, const struct sockaddr_in* serv_addr, socklen_t serv_len) {
    int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
    if (ret < 0) {
        AsyncLogger::LogFatal("bind failed.");
    } else {
        AsyncLogger::LogInfo("bind success.");
    }
    return ret;
}

inline int DoListen(int fd, int backlog) {
    int ret = listen(fd, backlog);
    if (ret < 0) {
        AsyncLogger::LogFatal("listen failed.");
    } else {
        AsyncLogger::LogInfo("listen success.");
    }
    return ret;
}

inline int DoAccept(int fd, struct sockaddr_in* client) {
    socklen_t len = sizeof *client;
    int connfd = accept(fd, (struct sockaddr*)client, &len);
    if (connfd < 0) {
        AsyncLogger::LogWarn("accept failed.");
    } else {
        AsyncLogger::LogInfo("accept success.");
    }
    return connfd;
}

inline int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len) {
    int ret = connect(fd, (struct sockaddr*)client, client_len);
    if (ret < 0) {
        AsyncLogger::LogWarn("connect failed.");
    }
    return ret;
}

inline ssize_t DoRead(int fd, void* buf, size_t size) {
    return read(fd, buf, size);
}

inline ssize_t DoSend(int connfd, const char* buf, size_t size) {
    ssize_t ret;
    if ((ret = send(connfd, buf, size, 0)) < 0) {
        AsyncLogger::LogWarn("Send failed.");
    }
    return ret;
}

inline void DoShutdownWrite(int fd) {
    if (shutdown(fd, SHUT_WR) < 0) {
        AsyncLogger::LogWarn("shutdown write failed, fd = %d", fd);
    }
}
} // namespace

#endif
