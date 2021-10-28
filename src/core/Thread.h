#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>

namespace lite_http {
    class Thread {
    private:
        pthread_t th_id;
        pthread_mutex_t mtx;
        pthread_cond_t cond;
    public:
        Thread() {}
        ~Thread() {}
        Thread(const Thread&) = delete;

        pthread_t get_thread_id() const {
            return th_id;
        }
    };
}

#endif