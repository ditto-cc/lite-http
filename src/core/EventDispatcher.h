#ifndef _EVENT_DISPATCHER_H
#define _EVENT_DISPATCHER_H

#include "Channel.h"

namespace lite_http {
    class EventDispatcher {
    private:
        const char* name;
    public:
        explicit EventDispatcher(const char* name): name(name) {}
        virtual ~EventDispatcher() = default;
        virtual void init() = 0;
        virtual void add(const Channel& channel) = 0;
        virtual void del(const Channel& channel) = 0;
        virtual void update(const Channel& channel) = 0;
        virtual void dispatch(struct timeval* timeval) = 0;
        virtual void clear() = 0;
    };
}

#endif