
#include <thread>
#include <queue>
#include <string>
#include <cassert>

#include "core/EventLoop.h"
#include "core/SelectDispatcher.h"
#include "log/Log.h"


namespace lite_http {

EventLoop::EventLoop(std::string name) 
    : m_name(std::move(name)),
    m_dispatcher(new SelectDispatcher(this)),
    m_thread_id(std::this_thread::get_id()) {}

EventLoop::~EventLoop() = default;

void EventLoop::run_loop() {
    m_loop = true;
    is_stop = false;
    LOG_INFO("EventLoop(%s) start looping.", m_name.c_str());
    while (!is_stop) {
        m_activate_channels.clear();
        LOG_INFO("dispatch.");
        m_dispatcher->dispatch(0, m_activate_channels);

        LOG_INFO("handle channels(%d).", m_activate_channels.size());
        m_event_handling = true;
        for (Channel* ch: m_activate_channels) {
            ch->handle_event();
        }
        m_event_handling = false;

        handle_pending_functors();
    }
    LOG_INFO("EventLoop(%s) stop looping.", m_name.c_str());
    m_loop = false;
}

void EventLoop::quit() {
    is_stop = true;
    // TODO wakeup if not in thread
    if (!is_in_thread())
        wakeup();
}

void EventLoop::handle_pending_functors() {
    m_calling_pending_functors = true;
    std::vector<Functor> functors;
    {
        std::lock_guard<std::mutex> guard(m_mtx);
        functors.swap(m_pending_functors);
    }

    LOG_INFO("handle callbacks(%d).", functors.size());
    for (const Functor& func: functors) {
        func();
    }
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
    if (is_in_thread() || m_calling_pending_functors)
        wakeup();
}

void EventLoop::wakeup() {
    // TODO
}

void EventLoop::handle_read() {
    // TODO
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
