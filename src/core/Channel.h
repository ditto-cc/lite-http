#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <utility>
#include <memory>
#include <cassert>

#include "util/nocopyable.h"

namespace lite_http {

class EventLoop;
typedef std::function<void()> Func;

const int EVENT_NONE = 0x00;
const int EVENT_READ = 0x02;
const int EVENT_WRITE = 0x04;

class Channel : private nocopyable {
public:
    Channel(EventLoop* event_loop, int fd)
        : m_fd(fd),
        m_loop(event_loop),
        m_event(EVENT_NONE),
        m_revent(EVENT_NONE),
        m_add_to_loop(false) {}
    ~Channel() = default;

    void set_write_callback(const Func& callback) { m_write_callback = callback; }
    void set_read_callback(const Func& callback) { m_read_callback = callback; }

    int fd() const { return m_fd; }
    int event() const { return m_event; }
    int revent() const { return m_revent; }
    void set_revent(int revent) { m_revent = revent; }

    void handle_event() {
        std::shared_ptr<void> guard;
        if (m_tied) {
            guard = m_tie.lock();
            if (guard)
                do_handle_event();
        } else {
            do_handle_event();
        }
    }

    void enable_write() { m_event |= EVENT_WRITE; update(); }
    void disable_write() { m_event &= ~EVENT_WRITE; update(); }
    void enable_read() { m_event |= EVENT_READ; update(); }
    void disable_read() { m_event &= ~EVENT_READ; update(); }
    void disable_all() { m_event = EVENT_NONE; update(); }

    bool readable() const { return m_event & EVENT_READ; }
    bool writable() const { return m_event & EVENT_WRITE; }
    bool is_none() const { return m_event == EVENT_NONE; }

    void tie(const std::shared_ptr<void>& ptr) { m_tie = ptr; m_tied = true; }
    void remove();

private:
    EventLoop* m_loop;
    int m_fd, m_event, m_revent;
    bool m_add_to_loop, m_tied;
    std::weak_ptr<void> m_tie;
    Func m_read_callback, m_write_callback;

    void update();
    void do_handle_event() {
        if (m_revent & EVENT_READ)
            if (m_read_callback)
                m_read_callback();
        if (m_revent & EVENT_WRITE) 
            if (m_write_callback)
                m_write_callback();
    }
};
} // namespace

#endif
