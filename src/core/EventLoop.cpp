
#include <thread>
#include <queue>
#include <string>
#include <cassert>

#include "core/EventLoop.h"
#include "core/ChannelMap.h"
#include "core/SelectDispatcher.h"
#include "log/Log.h"
#include "ChannelElement.h"


namespace lite_http {

EventLoop::EventLoop(std::string name) 
    : m_dispatcher(new SelectDispatcher(this, name.c_str())),
    m_thread_id(std::this_thread::get_id()) {}

EventLoop::~EventLoop() = default;

void EventLoop::run_loop() {
    m_loop = true;
    is_stop = false;
    AsyncLogger::LogInfo("EventLoop start looping.");
    while (!is_stop) {
        m_activate_channels.clear();
        AsyncLogger::LogInfo("dispatch.");
        m_dispatcher->dispatch(0, &m_activate_channels);

        AsyncLogger::LogInfo("handle channels (%d)", m_activate_channels.size());
        m_event_handling = true;
        for (Channel* ch: m_activate_channels) {
            ch->handle_event();
        }
        m_event_handling = false;

        AsyncLogger::LogInfo("handle callbacks.");
        handle_pending_functors();
    }
    AsyncLogger::LogInfo("EventLoop stop looping.");
    m_loop = false;
}

void EventLoop::handle_pending_functors() {
    m_calling_pending_functors = true;
    std::vector<Functor> functors;
    {
        std::lock_guard<std::mutex> guard(m_mtx);
        functors.swap(m_pending_functors);
    }

    for (const Functor& func: functors)
    func();
    m_calling_pending_functors = false;
}

void EventLoop::run_in_loop(Functor func) {
    if (is_in_thread())
        func();
    else
        queue_in_loop(std::move(func));
}

void EventLoop::queue_in_loop(Functor func) {
    {
        std::lock_guard<std::mutex> guard(m_mtx);
        m_pending_functors.push_back(std::move(func));
    }
    // TODO wakeup
}

void EventLoop::add_channel(Channel* ch) {
    m_dispatcher->add(ch);
}

void EventLoop::update_channel(Channel* ch) {
    m_dispatcher->update(ch);
}

void EventLoop::delete_channel(Channel* ch) {
    m_dispatcher->del(ch);
}


} // namespace
