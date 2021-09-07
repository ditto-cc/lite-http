
#include "eventloop.h"
#include "log/lite_log.h"


namespace lite_http {
    void EventLoop::run_eventloop() {

        while (!is_stop) {
            ;
            is_stop = true;
        }
    }
}
