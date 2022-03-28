#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <queue>
#include <thread>

#include "core/Channel.h"
#include "core/ChannelMap.h"
#include "core/ChannelElement.h"
#include "core/EventDispatcher.h"
#include "util/Queue.h"
#include "util/nocopyable.h"

namespace lite_http {
class EventLoop : nocopyable {
public:
    typedef std::function<void()> Functor;

    EventLoop(std::string name);
    ~EventLoop();

    void run_loop();

    void run_in_loop(Functor cb);
    void queue_in_loop(Functor cb);

    void add_channel(Channel* ch);
    void update_channel(Channel* ch);
    void delete_channel(Channel* ch);

    void channel_event_activate(int fd, int event);
private:
    void handle_pending_functors();

    bool is_in_thread() const { return std::this_thread::get_id() == m_thread_id; }

private:
    std::string m_name;
    bool is_stop {false}, m_loop{false};
    bool m_calling_pending_functors {false}, m_event_handling {false};
    unique_ptr<EventDispatcher> m_dispatcher;
    std::vector<Channel*> m_activate_channels;
    std::vector<Functor> m_pending_functors;
    std::thread::id m_thread_id;
    std::mutex m_mtx;
};
}

#endif
