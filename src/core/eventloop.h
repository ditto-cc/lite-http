#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

namespace lite_http {
    class EventLoop {
    private:
        bool is_stop;
    public:
        EventLoop() : is_stop(false) {}
        ~EventLoop() {}

        void run_eventloop();
    };
}

#endif
