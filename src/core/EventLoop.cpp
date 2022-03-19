
#include <thread>
#include <queue>

#include "core/EventLoop.h"
#include "core/ChannelMap.h"
#include "core/SelectDispatcher.h"
#include "log/Log.h"
#include "ChannelElement.h"


namespace lite_http {
    class EventLoop::Impl {
    public:
        Impl() : m_dispatcher(new SelectDispatcher(128)) {}
        ~Impl() = default;
    public:
        bool is_stop {false};
        std::unique_ptr<EventDispatcher> m_dispatcher;
        std::unordered_map<int, std::shared_ptr<Channel>> m_channel_map;
        std::queue<ChannelElement> m_queue;
        std::thread m_thread;
        std::mutex m_mtx;

        void do_channel_event(int fd, Channel& channel, int type) {
            m_queue.push(ChannelElement(channel, type));
            // TODO is same thread
            handle_pending_channel();
        }

        void handle_pending_channel() {
            std::lock_guard<std::mutex> lock(m_mtx);
            while (!m_queue.empty()) {
                ChannelElement& ce = m_queue.front();
                if (ce.type() == 1) {
                    handle_pending_add(ce.get_channel());
                } else if (ce.type() == 2) {
                    handle_pending_delete(ce.get_channel());
                } else if (ce.type() == 3) {
                    handle_pending_update(ce.get_channel());
                }
                m_queue.pop();
            }
        }

        void handle_pending_add(Channel& ch) {
            int fd = ch.get_fd();
            if (fd < 0) return;

            auto find = m_channel_map.find(fd);
            if (find == m_channel_map.end()) {
                m_channel_map.insert({fd, std::shared_ptr<Channel>(&ch)});
                m_dispatcher->add(ch);
            }
        }

        void handle_pending_delete(Channel& ch) {
            // TODO call dispatcher delete
        }

        void handle_pending_update(Channel& ch) {
            // TODO call dispatcher update
        }
    };

    EventLoop::EventLoop() : m_pimpl(new Impl()) {}

    EventLoop::~EventLoop() = default;

    void EventLoop::run_eventloop() {
        while (!m_pimpl->is_stop) {
            AsyncLogger::LogInfo("RUN IN EVENTLOOP.");

            m_pimpl->m_dispatcher->dispatch(0);
        }
    }

    void EventLoop::add_channel_event(int fd, Channel &channel) {
        m_pimpl->do_channel_event(fd, channel, 1);
    }
}
