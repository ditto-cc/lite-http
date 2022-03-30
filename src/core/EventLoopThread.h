#ifndef _EVENTLOOPOTHREAD_H
#define _EVENTLOOPOTHREAD_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <functional>
#include "util/nocopyable.h"

namespace lite_http {

class EventLoop;

class EventLoopThread : private nocopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    EventLoopThread(
        std::string name,
        ThreadInitCallback cb)
        : m_loop(nullptr),
        m_name(std::move(name)),
        m_init_cb(std::move(cb)) {}
    ~EventLoopThread();

    EventLoop* start();

private:
    EventLoop* m_loop;
    ThreadInitCallback m_init_cb;
    std::string m_name;
    mutable std::mutex m_mtx;
    mutable std::condition_variable m_cv;
    std::thread m_thread;

    void thread_func();
};
} // namespace


#endif // _EVENTLOOPOTHREAD_H
