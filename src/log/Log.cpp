
#include "log/Log.h"

#include <fstream>

namespace lite_http {
const char *AsyncLogger::log_path_ = nullptr;
unsigned int AsyncLogger::concurrency_ = 1;
// std::mutex AsyncLogger::mtx_;
// std::condition_variable AsyncLogger::cv_;

void AsyncLogger::LogInfo(const char *fmt, ...) {
  AsyncLogger &logger = GetLogger();
  std::call_once(logger.started_, &AsyncLogger::Start, logger);
  struct timespec ts{};
  timespec_get(&ts, TIME_UTC);

  va_list args;
  va_start(args, fmt);
  char buf[4096];
  vsprintf(buf, fmt, args);
  va_end(args);
  std::lock_guard<std::mutex> guarad(logger.mtx_);
  logger.logs_.push({ts, buf, INFO_LEVEL});
  logger.cv_.notify_one();
}

void AsyncLogger::LogWarn(const char *fmt, ...) {
  AsyncLogger &logger = GetLogger();
  std::call_once(logger.started_, &AsyncLogger::Start, logger);
  struct timespec ts{};
  timespec_get(&ts, TIME_UTC);

  va_list args;
  va_start(args, fmt);
  char buf[4096];
  vsprintf(buf, fmt, args);
  va_end(args);
  std::lock_guard<std::mutex> guarad(logger.mtx_);
  logger.logs_.push({ts, buf, WARN_LEVEL});
  logger.cv_.notify_one();
}

void AsyncLogger::LogFatal(const char *fmt, ...) {
  AsyncLogger &logger = GetLogger();
  std::call_once(logger.started_, &AsyncLogger::Start, logger);
  struct timespec ts{};
  timespec_get(&ts, TIME_UTC);

  va_list args;
  va_start(args, fmt);
  char buf[4096];
  vsprintf(buf, fmt, args);
  va_end(args);

  std::lock_guard<std::mutex> guarad(logger.mtx_);
  logger.logs_.push({ts, buf, FATAL_LEVEL});
  logger.cv_.notify_one();
}

void AsyncLogger::Config(const char *log_path, unsigned int concurrency) {
  log_path_ = log_path;
  concurrency_ = concurrency;
  AsyncLogger &logger = GetLogger();
  std::call_once(logger.started_, &AsyncLogger::Start, logger);
}
}
