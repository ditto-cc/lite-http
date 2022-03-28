
#include <cassert>

#include "Channel.h"
#include "EventLoop.h"

namespace lite_http {
void Channel::update() {
    m_add_to_loop = true;
    m_loop->update_channel(this);
}
void Channel::remove() {
    assert(m_event == EVENT_NONE);
    m_loop->delete_channel(this);
    m_add_to_loop = false;
}


}
