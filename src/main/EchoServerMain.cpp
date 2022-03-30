
#include "core/EventLoop.h"
#include "tcp/TCPServer.h"
#include "log/Log.h"

using namespace lite_http;

void onMessage(const TCPConnectionPtr& conn_sp) {
    Buffer& buf_in = conn_sp->get_buf_in();
    std::string msg = buf_in.get_readable_as_str();
    LOG_INFO("receive msg from connection(%s): %s.", conn_sp->get_conn_name(), msg.substr(0, msg.find_first_of('\r')).c_str());
    conn_sp->send(msg.c_str(), msg.size());
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: Port\n");
        exit(0);
    }

    bool blocking = true;
    const char* server_name = "EchoServer";
    uint16_t port = atoi(argv[1]);

    EventLoop loop(std::string(server_name) + "_loop");
    INetAddress listen_addr(port);
    TCPServer server(&loop, listen_addr, server_name, blocking);
    server.set_conn_cb(
            [](const TCPConnectionPtr& conn_sp) {
                LOG_INFO("connection callback(%s)", conn_sp->get_conn_name());
            });
    server.set_message_cb(onMessage);
    server.set_write_com_cb(
            [](const TCPConnectionPtr& conn_sp){
                LOG_INFO("write callback(%s)", conn_sp->get_conn_name());
            });
    server.start();
    loop.run_loop();

    return 0;
}
