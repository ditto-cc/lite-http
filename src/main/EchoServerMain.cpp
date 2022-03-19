
#include <cstring>
#include <iostream>
#include <string>

#include "socket/Common.h"
#include "socket/Socket.h"
#include "log/Log.h"

int main() {
    using namespace lite_http;
    AsyncLogger::Config(nullptr, 1);
    SocketServer ss;

    ss.Bind();
    ss.Listen();

    char temp[65535];
    for (;;) {
        SocketClient sc = ss.Accept();
        if (sc.get_fd() < 0)
            continue;
        for (;;) {
            Buffer buf(10);
            ssize_t ret = sc.Read(buf);
            if (ret == 0) {
                AsyncLogger::LogInfo("Connection closed.");
                break;
            }
            ssize_t readable = buf.get_readable_size();
            const char* ptr = buf.get_read_ptr();
            for (int i = 0; i < readable; ++i)
                temp[i] = *(ptr + i);
            temp[readable] = 0;
            AsyncLogger::LogInfo("%ld bytes received from client. %s", readable, temp);
            ssize_t send_len = sc.Send(buf);
            AsyncLogger::LogInfo("%ld bytes sent to client.", send_len);
        }
    }
    return 0;
}

