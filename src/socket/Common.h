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

inline uint32_t Host2Network32(uint32_t x);
inline uint16_t Host2Network16(uint16_t x);
inline uint32_t Network2Host32(uint32_t x);
inline uint16_t Network2Host16(uint16_t x);

inline void MakeNonblocking(int fd);
inline int MakeSocket(int domain, int type, bool blocking);

inline int DoBind(int fd, const struct sockaddr_in *serv_addr, socklen_t serv_len);
inline int DoListen(int fd, int backlog);
inline int DoAccept(int fd, struct sockaddr_in *client);
inline int DoConnect(int fd, struct sockaddr_in *client, socklen_t client_len);

inline ssize_t DoRead(int fd, void *buf, size_t size);
inline ssize_t DoSend(int connfd, const char *buf, size_t size);

inline void DoShutdownWrite(int fd);

inline void Sockaddr2CStr(char *buf, size_t len, const struct sockaddr_in *addr);
inline std::string Sockaddr2Str(const struct sockaddr_in *addr);

} // namespace

#endif
