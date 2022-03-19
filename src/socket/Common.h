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
#include <fcntl.h>

#include <memory>


namespace lite_http {
    typedef unsigned long long size_t;

    const long MAX_LINE = 4096;
    const int LISTENQ = 1024;
}



#endif
