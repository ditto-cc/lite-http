#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <unordered_map>
#include <list>
#include <memory>

#include "core/Channel.h"
#include "core/Thread.h"
#include "util/Queue.h"
#include "util/nocopyable.h"

namespace lite_http {
    class EventLoop : nocopyable {
    public:
        EventLoop();
        ~EventLoop();

        void run_eventloop();

        void add_channel_event(int fd, Channel& channel);
        void delete_channel_event(int fd, Channel& channel);
        void update_channel_event(int fd, Channel& channel);

    private:
        class Impl;
        std::unique_ptr<Impl> m_pimpl;
    };
}

#endif
