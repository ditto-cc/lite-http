#include "core/SelectDispatcher.h"


namespace lite_http {
    void SelectDispatcher::init() {
        FD_ZERO(&readset);
        FD_ZERO(&writeset);
        FD_ZERO(&exset);
    }

    void SelectDispatcher::dispatch() {
        init();
        FD_SET(listenfd, &readset);
        int maxfd = get_max_fd();
    }

    void SelectDispatcher::add() {}
    void SelectDispatcher::update() {}
    void SelectDispatcher::del() {}
    void SelectDispatcher::clear() {}
}