#ifndef _LITE_LOG_H
#define _LITE_LOG_H

#include <cstdio>
#include <cstring>
#include <cstdarg>

#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define INFO "INFO"
#define WARN "WARN"
#define FATAL "FATAL"

namespace lite_http {

#define LOG_INFO(fmt, args...) log_debug(INFO_LEVEL, fmt,##args)
#define LOG_WARN(fmt, args...) log_debug(WARN_LEVEL, fmt,##args)
#define LOG_FATAL(fmt, args...) log_debug(FATAL_LEVEL, fmt,##args)

    enum LOG_LEVEL { INFO_LEVEL, WARN_LEVEL, FATAL_LEVEL };
    inline void log_debug(const LOG_LEVEL& level, const char* fmt, ...) {
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

        switch (level)
        {
        case INFO_LEVEL:
            printf("%s%s [%s]%s: %s\n", WHITE, time_buf, INFO, RESET, buf);
            break;
        case FATAL_LEVEL:
            printf("%s%s [%s]%s: %s %s exit(%d).\n", RED, time_buf, FATAL, RESET, buf, strerror(errno), errno);
            exit(errno);
        default:
            printf("%s%s [%s]%s: %s\n", YELLOW, time_buf, WARN, RESET, buf);
            break;
        }
    }
}

#endif