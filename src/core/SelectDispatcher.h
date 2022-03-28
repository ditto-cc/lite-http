#ifndef _SELECT_DISPATCHER_H
#define _SELECT_DISPATCHER_H

#include <sys/select.h>

#include "core/EventDispatcher.h"

namespace lite_http {
class SelectDispatcher : public EventDispatcher {
public:
    explicit SelectDispatcher(EventLoop* event_loop, const char* name = "select dispatcher") : EventDispatcher(event_loop, name) {}
    ~SelectDispatcher() override = default;

    void init() override;
    void add(Channel* channel) override;
    void del(Channel* channel) override;
    void update(Channel* channel) override;
    void dispatch(struct timeval* timeval, std::vector<Channel*>* activate_channels) override;
    void clear() override;

private:
    fd_set m_readset{}, m_writeset{}, m_exset{};

    void fd_zero();
    void set_channel(Channel* ch);
};
}

#endif
