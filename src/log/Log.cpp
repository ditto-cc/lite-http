
#include "log/Log.h"

#include <fstream>

namespace lite_http {
const char* AsyncLogger::m_log_path = nullptr;
unsigned int AsyncLogger::m_concurrency = 1;
// std::mutex AsyncLogger::m_mtx;
// std::condition_variable AsyncLogger::m_cv;

void AsyncLogger::LogInfo(const char *fmt, ...) {
    AsyncLogger& logger = GetLogger();
    std::call_once(logger.m_started, &AsyncLogger::Start, logger);
    struct timespec ts{};
    timespec_get(&ts, TIME_UTC);

    va_list args;
    va_start(args, fmt);
    char buf[4096];
    vsprintf(buf, fmt, args);
    va_end(args);
    std::lock_guard<std::mutex> guarad(logger.m_mtx);
    logger.m_logs.push({ts, buf, INFO_LEVEL});
    logger.m_cv.notify_one();
}

void AsyncLogger::LogWarn(const char *fmt, ...) {
    AsyncLogger& logger = GetLogger();
    std::call_once(logger.m_started, &AsyncLogger::Start, logger);
    struct timespec ts{};
    timespec_get(&ts, TIME_UTC);

    va_list args;
    va_start(args, fmt);
    char buf[4096];
    vsprintf(buf, fmt, args);
    va_end(args);
    std::lock_guard<std::mutex> guarad(logger.m_mtx);
    logger.m_logs.push({ts, buf, WARN_LEVEL});
    logger.m_cv.notify_one();
}

void AsyncLogger::LogFatal(const char *fmt, ...) {
    AsyncLogger& logger = GetLogger();
    std::call_once(logger.m_started, &AsyncLogger::Start, logger);
    struct timespec ts{};
    timespec_get(&ts, TIME_UTC);

    va_list args;
    va_start(args, fmt);
    char buf[4096];
    vsprintf(buf, fmt, args);
    va_end(args);

    std::lock_guard<std::mutex> guarad(logger.m_mtx);
    logger.m_logs.push({ts, buf, FATAL_LEVEL});
    logger.m_cv.notify_one();
}

void AsyncLogger::Config(const char *log_path, unsigned int concurrency) {
    m_log_path = log_path;
    m_concurrency = concurrency;
    AsyncLogger& logger = GetLogger();
    std::call_once(logger.m_started, &AsyncLogger::Start, logger);
}
}
