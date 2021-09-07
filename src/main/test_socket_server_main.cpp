#include "socket/common.h"
#include <iostream>

#include "log/lite_log.h"

int main() {
    using namespace lite_http;
    int fd = Socket();
    struct sockaddr_in serv;
    u_int16_t port = 12345;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int ret = Bind(fd, &serv, sizeof(serv));

    Listen(fd, LISTENQ);

    char buf_in[MAX_LINE];
    char buf_out[MAX_LINE];
    char log_str[MAX_LINE];
    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t client_len;
        int connfd = Accept(fd, &client_addr, &client_len);

        for (;;) {
            ssize_t read_size = Read(connfd, buf_in, MAX_LINE);
            sprintf(log_str, "%ld bytes read from client.", read_size);
            log_info(log_str);
            if (read_size == 0) {
                close(connfd);
                log_info("Connection closed.");
                break;
            }
            strncpy(buf_out, buf_in, read_size);
            buf_out[read_size] = buf_in[read_size] = '\0';
            printf("%s", buf_in);
            ssize_t send_len = Send(connfd, buf_out, strlen(buf_out));
            sprintf(log_str, "%ld bytes sent to client.", send_len);
            log_info(log_str);
        }
    }

    return 0;
}

