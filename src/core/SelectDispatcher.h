#ifndef _SELECT_DISPATCHER
#define _SELECT_DISPATCHER

#include <sys/select.h>

#include <vector>

#include "core/EventDispatcher.h"

namespace lite_http {
    class SelectDispatcher : public EventDispatcher {
    private:
        fd_set m_readset{}, m_writeset{}, m_exset{};
        std::vector<int> m_fds;
        int get_max_fd() const {
            int maxfd = -1;
            for (const int& fd: m_fds)
                if (fd > maxfd)
                    maxfd = fd;
            return maxfd;
        }

    public:
        explicit SelectDispatcher(int init_size, const char* name = "select dispatcher") : EventDispatcher(name) {
            m_fds.reserve(init_size);
        }
        ~SelectDispatcher() override = default;

        void fd_zero();

        void init() override;
        void add(const Channel& channel) override;
        void del(const Channel& channel) override;
        void update(const Channel& channel) override;
        void dispatch(struct timeval* timeval) override;
        void clear() override;
    };
}

#endif