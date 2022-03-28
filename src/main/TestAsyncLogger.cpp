//
// Created by cheng on 2021/12/2.
//

#include "log/Log.h"

int main() {
    using namespace lite_http;
    AsyncLogger::Config(nullptr, 1);

    for (int i = 0; ; ++i) {
        AsyncLogger::LogInfo("info log", lite_http::INFO_LEVEL);
        AsyncLogger::LogWarn("warn log", lite_http::WARN_LEVEL);
        AsyncLogger::LogFatal("fatal log", lite_http::FATAL_LEVEL);
    }

    return 0;
}
