
#include "core/EventLoop.h"
#include "core/EventLoopThread.h"
#include "core/EventLoopThreadPool.h"

namespace lite_http {

void EventLoopThreadPool::start() {
    assert(!m_started);
    m_started = true;

    for (int i = 0; i < m_thread_num; ++i) {
        EventLoopThread* th = new EventLoopThread(m_name + "#" + std::to_string(i), m_thread_init_cb);
        m_threads.push_back(std::unique_ptr<EventLoopThread>(th));
        m_loops.push_back(th->start());
    }

    if (m_thread_num > 0) {
        m_itr = m_loops.cbegin();
    }

    if (m_thread_init_cb && m_thread_num == 0) {
        m_thread_init_cb(m_baseloop);
    }
}


EventLoop* EventLoopThreadPool::next_loop() {
    if (m_thread_num == 0) return m_baseloop;
    EventLoop* loop = *m_itr;
    if (m_itr == m_loops.end())
        m_itr = m_loops.begin();
    return loop;
}

} // namespace
