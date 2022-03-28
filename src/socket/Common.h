#ifndef _COMMON_H
#define _COMMON_H

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
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

uint32_t Host2Network32(uint32_t x);
uint16_t Host2Network16(uint16_t x);
uint32_t Network2Host32(uint32_t x);
uint16_t Network2Host16(uint16_t x);

void make_nonblocking(int fd);
int MakeSocket(int domain, int type, bool blocking);

int DoBind(int fd, const struct sockaddr_in* serv_addr, socklen_t serv_len);
int DoListen(int fd, int backlog);
int DoAccept(int fd, struct sockaddr_in* client);
int DoConnect(int fd, struct sockaddr_in* client, socklen_t client_len);

ssize_t DoRead(int fd, void* buf, size_t size);
ssize_t DoSend(int connfd, const char* buf, size_t size);

void DoShutdownWrite(int fd);

void sockaddr2c_str(char* buf, size_t len, const struct sockaddr_in* addr);
std::string Sockaddr2str(const struct sockaddr_in* addr);

} // namespace

#endif
