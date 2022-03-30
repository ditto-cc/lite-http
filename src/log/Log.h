#ifndef _LITE_LOG_H
#define _LITE_LOG_H

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <ctime>

#include <fstream>
#include <iostream>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>

#define WHITE "\033[37m"
#define YELLOW "\033[33m"
#define RED "\033[31m"
#define RESET "\033[0m"

#define INFO "INFO"
#define WARN "WARN"
#define FATAL "FATAL"

namespace lite_http {

#define LOG_INFO(fmt, args...) AsyncLogger::LogInfo(fmt,##args)
#define LOG_WARN(fmt, args...) AsyncLogger::LogWarn(fmt,##args)
#define LOG_FATAL(fmt, args...) AsyncLogger::LogFatal(fmt,##args)

enum LOG_LEVEL {
    INFO_LEVEL, WARN_LEVEL, FATAL_LEVEL
};

inline void log_debug(const LOG_LEVEL &level, const char *fmt, ...) {
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

    switch (level) {
        case INFO_LEVEL:
            printf("%s%s [%s]%s: %s\n", WHITE, time_buf, INFO, RESET, buf);
            break;
        case FATAL_LEVEL:
            printf("%s%s [%s]%s: %s %s. exit(%d).\n", RED, time_buf, FATAL, RESET, buf, strerror(errno), errno);
            break;
        default:
            printf("%s%s [%s]%s: %s\n", YELLOW, time_buf, WARN, RESET, buf);
            break;
    }
}

struct Log {
    struct timespec ts;
    std::string msg;
    LOG_LEVEL level;
};

class AsyncLogger {
private:
    std::queue<Log> m_logs;
    std::vector<std::thread> m_threads;
    std::condition_variable m_cv;
    std::mutex m_mtx;
    std::once_flag m_started;
    bool m_runnning { true };
    static const char *m_log_path;
    static unsigned int m_concurrency;

    AsyncLogger() = default;

    ~AsyncLogger() {
        Stop();
        for (auto &th: m_threads) {
            th.join();
        }
    }

public:
    const std::string INFO_STR = " [INFO]: ";
    const std::string WARN_STR = " [WARN]: ";
    const std::string FATAL_STR = " [FATAL]: ";

    static void LogInfo(const char *, ...);
    static void LogWarn(const char *, ...);
    static void LogFatal(const char *, ...);
    static void Config(const char *log_path = nullptr, unsigned int concurrency = 1);

    static AsyncLogger &GetLogger() {
        static AsyncLogger logger;
        return logger;
    }

    void Stop() {
        std::lock_guard<std::mutex> guard(m_mtx);
        m_runnning = false;
        m_cv.notify_all();
    }

private:
    void Start() {
        // start log consumer
        for (size_t sz = m_concurrency; sz > 0; --sz)
            m_threads.emplace_back(std::thread(&AsyncLogger::Consume, this));
    }

    void Consume() {
        std::ofstream of;
        std::streambuf *sb;
        if (m_log_path == nullptr) {
            sb = std::cout.rdbuf();
        } else {
            of.open(m_log_path, std::ios::app);
            sb = of.rdbuf();
        }
        std::ostream os(sb);
        while (true) {
            Log log;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                while (m_logs.empty()) {
                    if (!m_runnning) break;
                    m_cv.wait(lock);
                }

                if (!m_runnning) break;

                std::swap(log, m_logs.front());
                m_logs.pop();
            }
            process_log(os, log);
        }

        std::lock_guard<std::mutex> guard(m_mtx);
        while (!m_logs.empty()) {
            process_log(os, m_logs.front());
            m_logs.pop();
        }
    }

    void process_log(std::ostream& os, const Log& log) {
        char time_buf[100] = "";
        size_t rc = strftime(time_buf, sizeof time_buf, "%D %T", gmtime(&log.ts.tv_sec));
        snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ld UTC", log.ts.tv_nsec / 1000);
        std::string buf(time_buf);
        switch (log.level) {
            case INFO_LEVEL:
                os << buf + INFO_STR + log.msg << std::endl;
                break;
            case FATAL_LEVEL:
                os << buf + FATAL_STR + log.msg << std::endl;
                exit(errno);
                break;
            default:
                os << buf + WARN_STR + log.msg << std::endl;
                break;
        }
        os.flush();
    }
};

}

#endif
