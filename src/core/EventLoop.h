#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <unordered_map>
#include <list>
#include <memory>

#include "core/EventDispatcher.h"
#include "core/SelectDispatcher.h"
#include "core/Channel.h"
#include "core/Thread.h"
#include "util/Queue.h"

namespace lite_http {
    class EventLoop {
    public:
        EventLoop();
        ~EventLoop();

        void run_eventloop();
    private:
        class Impl;
        std::unique_ptr<Impl> m_pimpl;
    };
}

#endif
