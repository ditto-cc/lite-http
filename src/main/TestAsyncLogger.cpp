//
// Created by cheng on 2021/12/2.
//

#include "log/Log.h"

int main() {
    using namespace lite_http;
    AsyncLogger::Config(nullptr, 1);

    for (int i = 0; ; ++i) {
        LOG_INFO("info log");
        LOG_WARN("warn log");
        LOG_FATAL("fatal log");
    }

    return 0;
}
