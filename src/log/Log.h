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

enum LogLevel {
  INFO_LEVEL, WARN_LEVEL, FATAL_LEVEL
};

inline void log_debug(const LogLevel &level, const char *fmt, ...) {
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
    case INFO_LEVEL:printf("%s%s [%s]%s: %s\n", WHITE, time_buf, INFO, RESET, buf);
      break;
    case FATAL_LEVEL:
      printf("%s%s [%s]%s: %s %s. exit(%d).\n",
             RED,
             time_buf,
             FATAL,
             RESET,
             buf,
             strerror(errno),
             errno);
      break;
    default:printf("%s%s [%s]%s: %s\n", YELLOW, time_buf, WARN, RESET, buf);
      break;
  }
}

struct Log {
  struct timespec ts;
  std::string msg;
  LogLevel level;
};

static const char* INFO_STR = " [INFO]: ";
static const char* WARN_STR = " [WARN]: ";
static const char* FATAL_STR = " [FATAL]: ";

inline void ProcessLog(std::ostream &os, const Log &log) {
  char time_buf[100] = "";
  size_t rc = strftime(time_buf, sizeof time_buf, "%D %T", gmtime(&log.ts.tv_sec));
  snprintf(time_buf + rc, sizeof time_buf - rc, ".%06ld UTC", log.ts.tv_nsec / 1000);
  std::string buf(time_buf);
  switch (log.level) {
    case INFO_LEVEL:os << buf + INFO_STR + log.msg << std::endl;
      break;
    case FATAL_LEVEL:os << buf + FATAL_STR + log.msg << std::endl;
      exit(errno);
      break;
    default:os << buf + WARN_STR + log.msg << std::endl;
      break;
  }
  os.flush();
}

class AsyncLogger {
 private:
  std::queue<Log> logs_;
  std::vector<std::thread> threads_;
  std::condition_variable cv_;
  std::mutex mtx_;
  std::once_flag started_;
  bool running_ {true};
  static const char *log_path_;
  static unsigned int concurrency_;

  AsyncLogger() = default;

  ~AsyncLogger() {
    Stop();
    for (auto &th : threads_) {
      th.join();
    }
  }

 public:
  static void LogInfo(const char *, ...);
  static void LogWarn(const char *, ...);
  static void LogFatal(const char *, ...);
  static void Config(const char *log_path = nullptr, unsigned int concurrency = 1);

  static AsyncLogger &GetLogger() {
    static AsyncLogger logger;
    return logger;
  }

  void Stop() {
    std::lock_guard<std::mutex> guard(mtx_);
    running_ = false;
    cv_.notify_all();
  }

 private:
  void Start() {
    // Start log consumer
    for (size_t sz = concurrency_; sz > 0; --sz)
      threads_.emplace_back(std::thread(&AsyncLogger::Consume, this));
  }

  void Consume() {
    std::ofstream of;
    std::streambuf *sb;
    if (log_path_ == nullptr) {
      sb = std::cout.rdbuf();
    } else {
      of.open(log_path_, std::ios::app);
      sb = of.rdbuf();
    }
    std::ostream os(sb);
    while (true) {
      Log log;
      {
        std::unique_lock<std::mutex> lock(mtx_);
        while (logs_.empty()) {
          if (!running_) break;
          cv_.wait(lock);
        }

        if (!running_) break;

        std::swap(log, logs_.front());
        logs_.pop();
      }
      ProcessLog(os, log);
    }

    std::lock_guard<std::mutex> guard(mtx_);
    while (!logs_.empty()) {
      ProcessLog(os, logs_.front());
      logs_.pop();
    }
  }
};

}

#endif
