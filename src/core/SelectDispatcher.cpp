#include <cerrno>
#include <cstdlib>
#include <cassert>

#include "core/SelectDispatcher.h"
#include "core/Channel.h"
#include "log/Log.h"

namespace lite_http {

void SelectDispatcher::init() {
    FD_ZERO(&m_readset);
    FD_ZERO(&m_writeset);
    FD_ZERO(&m_exset);
}

void SelectDispatcher::add(Channel* channel) {
    int fd = channel->fd();
    LOG_INFO("channel add %d", fd);
    m_map[fd] = channel;

    if (channel->readable())
        FD_SET(fd, &m_readset);
    if (channel->writable())
        FD_SET(fd, &m_writeset);
}

void SelectDispatcher::update(Channel* channel) {
    if (m_map.count(channel->fd()) == 0) {
        add(channel);
    } else {
        int fd = channel->fd();
        LOG_INFO("channel update %d", fd);
        if (channel->readable())
            FD_SET(fd, &m_readset);
        else
            FD_CLR(fd, &m_readset);

        if (channel->writable())
            FD_SET(fd, &m_writeset);
        else
            FD_CLR(fd, &m_writeset);
    }
}

void SelectDispatcher::del(Channel* channel) {
    assert(channel->is_none());
    int fd = channel->fd();
    FD_CLR(fd, &m_writeset);
    FD_CLR(fd, &m_readset);
    FD_CLR(fd, &m_exset);
    LOG_INFO("channel del %d", fd);
    m_map.erase(fd);
}

void SelectDispatcher::dispatch(struct timeval* timeval, std::vector<Channel*>& channels) {
    if (m_map.empty()) {
        LOG_WARN("no fd to select.");
        return;
    }
    
    m_readmask = m_readset;
    m_writemask = m_writeset;
    m_exmask = m_exset;
    LOG_INFO("max fd = %d, start select.", m_map.rbegin()->first);
    if (select(m_map.rbegin()->first + 1, &m_readmask, &m_writemask, &m_exmask, timeval) < 0) {
        LOG_WARN("select error.");
        return;
    }

    for (auto it = m_map.begin(); it != m_map.end(); ++it) {
        int fd = it->first;
        Channel* ch = it->second;
        assert(ch->fd() == fd);
        int revent = EVENT_NONE;
        if (FD_ISSET(fd, &m_readmask))
            revent |= EVENT_READ;

        if (FD_ISSET(fd, &m_writemask))
            revent |= EVENT_WRITE;
        ch->set_revent(revent);
        if (revent == EVENT_NONE) continue;
        channels.push_back(ch);
    }
}

void SelectDispatcher::clear() {
    m_map.clear();
    init();
}

} // namespace
