#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <utility>
#include <memory>


namespace lite_http {
    const int EVENT_READ = 0x02;
    const int EVENT_WRITE = 0x04;

    class Channel {
    private:
        typedef std::function<void()> Func;
        int m_fd, m_event_type;
        Func m_read_callback {}, m_write_callback {};

    public:
        Channel(int fd, int event_type) : m_fd(fd), m_event_type(event_type) {}
        ~Channel() = default;

        void set_write_callback(Func callback) {
            m_write_callback = std::move(callback);
        }

        void set_read_callback(Func callback) {
            m_read_callback = std::move(callback);
        }

        void read_callback() {
            m_read_callback();
        }

        void write_callback() {
            m_read_callback();
        }

        int get_fd() const {
            return m_fd;
        }

        int get_event_type() const {
            return m_event_type;
        }

    };
}

#endif
