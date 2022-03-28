#ifndef _EVENT_DISPATCHER_H
#define _EVENT_DISPATCHER_H

#include <unordered_map>
#include <vector>

#include "Channel.h"
#include "ChannelMap.h"


namespace lite_http {

class EventLoop;

class EventDispatcher {
protected:
    const char* name;
    std::map<int, Channel*> m_map;
    EventLoop* m_event_loop;
public:
    explicit EventDispatcher(EventLoop* event_loop, const char* name): m_event_loop(event_loop), name(name) {}
    virtual ~EventDispatcher() = default;
    virtual void init() = 0;
    virtual void add(Channel* channel) = 0;
    virtual void del(Channel* channel) = 0;
    virtual void update(Channel* channel) = 0;
    virtual void dispatch(struct timeval* timeval, std::vector<Channel*>* activate_channels) = 0;
    virtual void clear() = 0;
};
}

#endif
