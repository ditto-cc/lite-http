#ifndef _EVENTLOOPTHREADPOOL_H
#define _EVENTLOOPTHREADPOOL_H

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace lite_http {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    EventLoopThreadPool(EventLoop* base, std::string name, int thread_num)
        : m_baseloop(base),
        m_name(std::move(name)),
        m_thread_num(thread_num) {
        m_loops.reserve(thread_num);
        m_threads.reserve(thread_num);
    }
    ~EventLoopThreadPool() = default;

    void start();
    EventLoop* next_loop();
    EventLoop* next_loop_hash();
    void set_thread_init_callback(ThreadInitCallback cb) { m_thread_init_cb = std::move(cb); }
    bool started() const { return m_started; }
    std::vector<EventLoop*> get_all_loops() const { return m_loops; }
    const std::string& name() const { return m_name; }

private:
    EventLoop* m_baseloop;
    std::string m_name;
    std::vector<std::unique_ptr<EventLoopThread>> m_threads;
    std::vector<EventLoop*> m_loops;
    int m_thread_num;
    bool m_started;
    std::vector<EventLoop*>::const_iterator m_itr;
    ThreadInitCallback m_thread_init_cb;
};

} // namespace

#endif // !_EVENTLOOPTHREADPOOL_H
