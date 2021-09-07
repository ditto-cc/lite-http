
#include "core/eventloop.h"


int main(int argc, char** argv) {

    lite_http::EventLoop eventloop;
    eventloop.run_eventloop();
    
    return 0;
}