#ifndef _EVENTLOOPTHREADPOOL_H
#define _EVENTLOOPTHREADPOOL_H

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace lite_http {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {
 public:
  typedef std::function<void(EventLoop *)> ThreadInitCallback;
  EventLoopThreadPool(EventLoop *base, std::string name, int thread_num)
      : baseloop_(base),
        name_(std::move(name)),
        started_(false),
        thread_num_(thread_num) {
    loops_.reserve(thread_num);
    threads_.reserve(thread_num);
  }
  ~EventLoopThreadPool() = default;

  void Start();
  EventLoop *NextLoop();
  EventLoop *NextLoopHash();
  void SetThreadInitCallback(ThreadInitCallback cb) { thread_init_cb_ = std::move(cb); }
  bool Started() const { return started_; }
  std::vector<EventLoop *> GetAllLoops() const { return loops_; }
  const std::string &Name() const { return name_; }

 private:
  EventLoop *baseloop_;
  std::string name_;
  std::vector<std::unique_ptr<EventLoopThread>> threads_;
  std::vector<EventLoop *> loops_;
  int thread_num_;
  bool started_;
  std::vector<EventLoop *>::const_iterator iterator_;
  ThreadInitCallback thread_init_cb_;
};

} // namespace

#endif // _EVENTLOOPTHREADPOOL_H
