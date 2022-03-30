
#include "core/EventLoop.h"
#include "core/EventLoopThread.h"

namespace lite_http {

EventLoop *EventLoopThread::Start() {
  thread_ = std::thread(&EventLoopThread::ThreadFunc, this);

  std::unique_lock<std::mutex> lock(mtx_);
  while (!loop_) {
    cv_.wait(lock);
  }

  return loop_;
}

EventLoopThread::~EventLoopThread() {
  if (loop_) {
    loop_->Quit();
    thread_.join();
  }
}

void EventLoopThread::ThreadFunc() {
  EventLoop loop(name_);

  if (init_callback_) {
    init_callback_(&loop);
  }

  {
    std::lock_guard<std::mutex> guard(mtx_);
    loop_ = &loop;
    cv_.notify_one();
  }

  loop.RunLoop();
  std::lock_guard<std::mutex> guard(mtx_);
  loop_ = nullptr;
}

} // namespace
