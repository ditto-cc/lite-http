
#include "core/EventLoop.h"
#include "log/Log.h"


namespace lite_http {
    class EventLoop::Impl {
    public:
        Impl() : dispatcher(new SelectDispatcher(128)) {}
        ~Impl() {}
    public:
        bool is_stop;
        std::unique_ptr<EventDispatcher> dispatcher;
        std::unordered_map<int, Channel> channelMap;
        Thread thread;
        Queue<Channel> queue;
    };

    EventLoop::EventLoop() : m_pimpl(new Impl()) {}

    EventLoop::~EventLoop() {}

    void EventLoop::run_eventloop() {

        while (!m_pimpl->is_stop) {
            log_info("RUN IN EVENTLOOP.");
            m_pimpl->is_stop = true;

            m_pimpl->dispatcher->dispatch();
        }
    }
}
