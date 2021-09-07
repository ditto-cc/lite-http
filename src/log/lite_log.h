#ifndef _LITE_LOG_H
#define _LITE_LOG_H

#include <iostream>

#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"
#define LOG_INFO "INFO"
#define LOG_WARN "WARN"
#define LOG_FATAL "FATAL"

#define LOG_INFO_PREFIX "[\033[37mINFO\033[0m]"
#define LOG_WARN_PREFIX "[\033[33mWARN\033[0m]"
#define LOG_FATAL_PREFIX "[\033[31mFATAL\033[0m]"

namespace lite_http {
    inline void log_info(const char* msg) {
        printf("%s %s\n", LOG_INFO_PREFIX, msg);
    }
    inline void log_warn(const char* msg) {
        printf("%s %s\n", LOG_WARN_PREFIX, msg);
    }
    inline void log_fatal(const char* msg) {
        printf("%s %s\n", LOG_FATAL_PREFIX, msg);
        exit(errno);
    }
}

#endif