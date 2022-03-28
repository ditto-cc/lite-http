#include "socket/Common.h"


namespace lite_http {
typedef unsigned long long size_t;

uint32_t Host2Network32(uint32_t x) {
    return htobe32(x);
}

uint16_t Host2Network16(uint16_t x) {
    return htobe16(x);
}

uint32_t Network2Host32(uint32_t x) {
    return be32toh(x);
}

uint16_t Network2Host16(uint16_t x) {
    return be16toh(x);
}

void make_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

int MakeSocket(int domain, int type, bool blocking) {
    int fd = socket(domain, type, 0);
    if (fd < 0) {
        AsyncLogger::LogWarn("create socket error");
    }
    if (!blocking) {
        make_nonblocking(fd);
    }
    return fd;
}

int DoBind(int fd, const struct sockaddr_in* serv_addr, socklen_t serv_len) {
    int ret = bind(fd, (struct sockaddr*)serv_addr, serv_len);
    if (ret < 0) {
        AsyncLogger::LogFatal("bind failed.");
    } else {
        AsyncLogger::LogInfo("bind success.");
    }
    return ret;
}

int DoListen(int fd, int backlog) {
    int ret = listen(fd, backlog);
    if (ret < 0) {
        AsyncLogger::LogFatal("listen failed.");
    } else {
        AsyncLogger::LogInfo("listen success.");
    }
    return ret;
}

int DoAccept(int fd, struct sockaddr_in* client) {
    socklen_t len = sizeof *client;
    int connfd = accept(fd, (struct sockaddr*)client, &len);
    if (connfd < 0) {
        AsyncLogger::LogWarn("accept failed.");
    } else {
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

ssize_t DoSend(int connfd, const char* buf, size_t size) {
    ssize_t ret;
    if ((ret = send(connfd, buf, size, 0)) < 0) {
        AsyncLogger::LogWarn("Send failed.");
    }
    return ret;
}

void DoShutdownWrite(int fd) {
    if (shutdown(fd, SHUT_WR) < 0) {
        AsyncLogger::LogWarn("shutdown write failed, fd = %d", fd);
    }
}

void sockaddr2c_str(char* buf, size_t len, const struct sockaddr_in* addr) {
    assert(len > INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &addr->sin_addr, buf, (socklen_t) len);
    size_t end = strlen(buf);

    uint16_t port = Network2Host16(addr->sin_port);
    assert(len > end);
    snprintf(buf + end, len - end, ":%u", port);
}

std::string Sockaddr2str(const struct sockaddr_in* addr) {
    char buf[64] = "";
    sockaddr2c_str(buf, sizeof buf, addr);
    return buf;
}
} // namespace

#
