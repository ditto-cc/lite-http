#ifndef _LITE_LOG_H
#define _LITE_LOG_H

#include <iostream>
#include <cstdio>
#include <cstdarg>

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
    inline void log_warn(const char* msg) {
        printf("%s %s\n", LOG_WARN_PREFIX, msg);
    }

    inline void log_fatal(const char* msg) {
        printf("%s %s\n", LOG_FATAL_PREFIX, msg);
        exit(errno);
    }

    inline void log_info(const char* fmt, ...) {
        struct timespec ts;
        timespec_get(&ts, TIME_UTC);
        char time_buf[100];
        size_t rc = strftime(time_buf, sizeof time_buf, "%D %T", gmtime(&ts.tv_sec));
        snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ld UTC", ts.tv_nsec / 1000);

        va_list args;
        va_start(args, fmt);
        char buf[4096];
        vsprintf(buf, fmt, args);
        va_end(args);

        printf("%s%s [%s]%s: %s\n", WHITE, time_buf, LOG_INFO, RESET, buf);
    }
}

#endif