#ifndef _COMMON_H
#define _COMMON_H
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



namespace lite_http {
    const ssize_t MAX_LINE = 4096;
    const int LISTENQ = 1024;

    int Socket();
    int Bind(int fd, struct sockaddr_in* serv_addr, socklen_t serv_len);
    int Listen(int fd, int backlog);
    int Accept(int fd, struct sockaddr_in* client, socklen_t* client_len);
    int Connect(int fd, struct sockaddr_in* client, socklen_t client_len);

    ssize_t Read(int connfd, void* buffer, size_t size);
    ssize_t Send(int connfd, void* buffer, size_t size);
}


#endif