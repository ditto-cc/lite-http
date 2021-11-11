
#include <iostream>
#include <string>

#include "socket/Common.h"
#include "socket/Socket.h"
#include "log/Log.h"

int main() {
    using namespace lite_http;
    SocketServer ss;

    ss.do_bind();
    ss.do_listen();

    for (;;) {
        SocketClient sc = ss.do_accept();
        if (sc.get_fd() < 0)
            continue;
        for (;;) {
            Buffer buf;
            ssize_t ret = Read(sc.get_fd(), buf.get_buf_in(), MAX_LINE);
            if (ret == 0) {
                LOG_WARN("Connection closed.");
                break;
            }
            LOG_INFO("%ld bytes read from client.", ret);
            buf.set_size(ret);
            printf("%s", (char*)buf.get_buf_in());
            ssize_t send_len = Send(sc.get_fd(), buf.get_buf_in(), buf.size());
            LOG_INFO("%ld bytes sent to client.", send_len);
        }
    }
    return 0;
}

