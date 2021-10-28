
#include "core/EventLoop.h"


int main(int argc, char** argv) {
    using namespace lite_http;
    EventLoop eventloop;
    eventloop.run_eventloop();
    
    return 0;
}