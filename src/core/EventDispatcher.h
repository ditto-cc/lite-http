#ifndef _EVENT_DISPATCHER_H
#define _EVENT_DISPATCHER_H

#include <map>
#include <vector>

#include "core/Channel.h"


namespace lite_http {

class EventLoop;

class EventDispatcher {
public:
    explicit EventDispatcher(EventLoop* event_loop, const char* name): m_event_loop(event_loop), m_name(name) {}
    virtual ~EventDispatcher() = default;
    virtual void init() = 0;
    virtual void add(Channel* channel) = 0;
    virtual void del(Channel* channel) = 0;
    virtual void update(Channel* channel) = 0;
    virtual void dispatch(struct timeval* timeval, std::vector<Channel*>& activate_channels) = 0;
    virtual void clear() = 0;
protected:
    const char* m_name;
    std::map<int, Channel*> m_map;
    EventLoop* m_event_loop;
};
}

#endif
