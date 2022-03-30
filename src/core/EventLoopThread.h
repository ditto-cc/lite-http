#ifndef _EVENTLOOPOTHREAD_H
#define _EVENTLOOPOTHREAD_H

#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <functional>
#include "util/Nocopyable.h"

namespace lite_http {

class EventLoop;

class EventLoopThread : private Nocopyable {
 public:
  typedef std::function<void(EventLoop *)> ThreadInitCallback;
  EventLoopThread(
      std::string name,
      ThreadInitCallback cb)
      : loop_(nullptr),
        name_(std::move(name)),
        init_callback_(std::move(cb)) {}
  ~EventLoopThread();

  EventLoop *Start();

 private:
  EventLoop *loop_;
  ThreadInitCallback init_callback_;
  std::string name_;
  mutable std::mutex mtx_;
  mutable std::condition_variable cv_;
  std::thread thread_;

  void ThreadFunc();
};
} // namespace


#endif // _EVENTLOOPOTHREAD_H
