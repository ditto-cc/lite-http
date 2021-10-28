#ifndef _EVENT_DISPATCHER_H
#define _EVENT_DISPATCHER_H

namespace lite_http {
    class EventDispatcher {
    private:
        const char* name;
    public:
        EventDispatcher(const char* name): name(name) {}
        virtual ~EventDispatcher() {}
        virtual void init() = 0;
        virtual void add() = 0;
        virtual void del() = 0;
        virtual void update() = 0;
        virtual void dispatch() = 0;
        virtual void clear() = 0;
    };
}

#endif