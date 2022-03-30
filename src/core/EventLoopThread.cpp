
#include "core/EventLoop.h"
#include "core/EventLoopThread.h"


namespace lite_http {

EventLoop* EventLoopThread::start() {
    m_thread = std::thread(&EventLoopThread::thread_func, this);

    std::unique_lock<std::mutex> lock(m_mtx);
    while (!m_loop) {
        m_cv.wait(lock);
    }

    return m_loop;
}

EventLoopThread::~EventLoopThread() {
    if (m_loop) {
        m_loop->quit();
        m_thread.join();
    }
}


void EventLoopThread::thread_func() {
    EventLoop loop(m_name);

    if (m_init_cb) {
        m_init_cb(&loop);
    }

    {
        std::lock_guard<std::mutex> guard(m_mtx);
        m_loop = &loop;
        m_cv.notify_one();
    }

    loop.run_loop();
    std::lock_guard<std::mutex> guard(m_mtx);
    m_loop = nullptr;
}

} // namespace
