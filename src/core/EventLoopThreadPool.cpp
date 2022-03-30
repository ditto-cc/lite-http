
#include "core/EventLoop.h"
#include "core/EventLoopThread.h"
#include "core/EventLoopThreadPool.h"

namespace lite_http {

void EventLoopThreadPool::Start() {
  assert(!started_);
  started_ = true;

  for (int i = 0; i < thread_num_; ++i) {
    EventLoopThread *th = new EventLoopThread(name_ + "#" + std::to_string(i), thread_init_cb_);
    threads_.push_back(std::unique_ptr<EventLoopThread>(th));
    loops_.push_back(th->Start());
  }

  if (thread_num_ > 0) {
    iterator_ = loops_.cbegin();
  }

  if (thread_init_cb_ && thread_num_ == 0) {
    thread_init_cb_(baseloop_);
  }
}

EventLoop *EventLoopThreadPool::NextLoop() {
  if (thread_num_ == 0) return baseloop_;
  EventLoop *loop = *iterator_;
  if (iterator_ == loops_.end())
    iterator_ = loops_.begin();
  return loop;
}

} // namespace
