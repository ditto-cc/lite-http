#include <cerrno>
#include <cstdlib>
#include <cassert>

#include "core/SelectDispatcher.h"
#include "core/Channel.h"
#include "log/Log.h"

namespace lite_http {

void SelectDispatcher::fd_zero() {
    FD_ZERO(&m_readset);
    FD_ZERO(&m_writeset);
    FD_ZERO(&m_exset);
}

void SelectDispatcher::set_channel(Channel* ch) {
    if (ch->is_none()) return;
    int fd = ch->get_fd();
    if (ch->readable())
        FD_SET(fd, &m_readset);
    if (ch->writable())
        FD_SET(fd, &m_writeset);
}

void SelectDispatcher::init() {
    fd_zero();
    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        set_channel(it->second);
    }
}

void SelectDispatcher::add(Channel* channel) {
    AsyncLogger::LogInfo("channel add %d", channel->get_fd());
    m_map[channel->get_fd()] = channel;
}

void SelectDispatcher::update(Channel* channel) {
    AsyncLogger::LogInfo("channel update %d", channel->get_fd());
    if (m_map.count(channel->get_fd()) == 0) {
        add(channel);
    }
}

void SelectDispatcher::del(Channel* channel) {
    assert(channel->is_none());
    AsyncLogger::LogInfo("channel del %d", channel->get_fd());
    m_map.erase(channel->get_fd());
}

void SelectDispatcher::dispatch(struct timeval* timeval, std::vector<Channel*>* channels) {
    if (m_map.empty()) {
        AsyncLogger::LogWarn("no fd to select.");
        return;
    }
    
    init();
    AsyncLogger::LogInfo("max fd = %d, start select.", m_map.rbegin()->first);
    if (select(m_map.rbegin()->first + 1, &m_readset, &m_writeset, &m_exset, timeval) < 0) {
        AsyncLogger::LogWarn("select error.");
        return;
    }

    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        int fd = it->first;
        Channel* ch = it->second;
        assert(ch->get_fd() == fd);
        int revent = EVENT_NONE;
        if (FD_ISSET(fd, &m_readset))
            revent |= EVENT_READ;

        if (FD_ISSET(fd, &m_writeset))
            revent |= EVENT_WRITE;
        ch->set_revent(revent);
        if (revent == EVENT_NONE) continue;
        channels->push_back(ch);
    }
}

void SelectDispatcher::clear() {}
}
