
#include "core/EventLoop.h"
#include "tcp/TCPServer.h"
#include "log/Log.h"

int main(int argc, char** argv) {
    using namespace lite_http;
    AsyncLogger::Config(nullptr, 1);

    shared_ptr<EventLoop> eventloop(new EventLoop());

    eventloop->run_eventloop();
    
    return 0;
}
