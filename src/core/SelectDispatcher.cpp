#include <cerrno>
#include <cstdlib>
#include "core/SelectDispatcher.h"
#include "Channel.h"
#include "log/Log.h"

namespace lite_http {
    void SelectDispatcher::fd_zero() {
        FD_ZERO(&m_readset);
        FD_ZERO(&m_writeset);
        FD_ZERO(&m_exset);
    }

    void SelectDispatcher::init() {
        fd_zero();
    }

    void SelectDispatcher::add(const Channel& channel) {
        if (channel.get_event_type() & EVENT_READ)
            FD_SET(channel.get_fd(), &m_readset);
        if (channel.get_event_type() & EVENT_WRITE)
            FD_SET(channel.get_fd(), &m_writeset);
    }

    void SelectDispatcher::update(const Channel& channel) {}

    void SelectDispatcher::del(const Channel& channel) {}

    void SelectDispatcher::dispatch(struct timeval* timeval) {
        int ret;
        if ((ret = get_max_fd()) == -1) {
            AsyncLogger::LogWarn("no fd to select.");
            return;
        }

        if (select(ret, &m_readset, &m_writeset, &m_exset, timeval) < 0) {
            AsyncLogger::LogWarn("select error.");
            return;
        }

        for (const int& fd: m_fds) {
            if (FD_ISSET(fd, &m_readset)) {
                // TODO channel activate

            }

            if (FD_ISSET(fd, &m_writeset)) {
                // TODO channel activate

            }
        }
    }

    void SelectDispatcher::clear() {}
}