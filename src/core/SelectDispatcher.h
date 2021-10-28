#ifndef _SELECT_DISPATCHER
#define _SELECT_DISPATCHER

#include <sys/select.h>

#include <vector>

#include "core/EventDispatcher.h"

namespace lite_http {
    using std::vector;
    using std::max;
    class SelectDispatcher : public EventDispatcher {
    private:
        fd_set readset, writeset, exset;
        int fd_init_set, maxfd;
        int listenfd;
        int init_size;
        vector<int> connfds;
        int get_max_fd() const {
            int maxfd = listenfd;
            for (const int& fd: connfds)
                maxfd = max(maxfd, fd);
            return maxfd;
        }
    public:
        SelectDispatcher(int init_size, const char* name = "select dispatcher") : EventDispatcher(name), init_size(init_size), connfds(init_size, -1) {}
        ~SelectDispatcher() = default;

        void init() override;
        void add() override;
        void del() override;
        void update() override;
        void dispatch() override;
        void clear() override;
    };
}

#endif