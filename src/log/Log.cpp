
#include "log/Log.h"

#include <fstream>

namespace lite_http {
    const char* AsyncLogger::m_log_path = nullptr;
    unsigned int AsyncLogger::m_concurrency = 1;

    void AsyncLogger::LogInfo(const char *fmt, ...) {
        struct timespec ts{};
        timespec_get(&ts, TIME_UTC);

        va_list args;
        va_start(args, fmt);
        char buf[4096];
        vsprintf(buf, fmt, args);
        va_end(args);
        GetLogger().m_logs.push({ts, buf, INFO_LEVEL});
        GetLogger().m_cv.notify_one();
    }

    void AsyncLogger::LogWarn(const char *fmt, ...) {
        struct timespec ts{};
        timespec_get(&ts, TIME_UTC);

        va_list args;
        va_start(args, fmt);
        char buf[4096];
        vsprintf(buf, fmt, args);
        va_end(args);
        GetLogger().m_logs.push({ts, buf, WARN_LEVEL});
        GetLogger().m_cv.notify_one();
    }

    void AsyncLogger::LogFatal(const char *fmt, ...) {
        struct timespec ts{};
        timespec_get(&ts, TIME_UTC);

        va_list args;
        va_start(args, fmt);
        char buf[4096];
        vsprintf(buf, fmt, args);
        va_end(args);

        GetLogger().m_logs.push({ts, buf, FATAL_LEVEL});
        GetLogger().m_cv.notify_one();
    }

    void AsyncLogger::Config(const char *log_path, unsigned int concurrency) {
        m_log_path = log_path;
        m_concurrency = concurrency;
        GetLogger().Start();
    }
}