
#include <thread>
#include <queue>
#include <string>
#include <cassert>

#include "core/EventLoop.h"
#include "core/SelectDispatcher.h"
#include "log/Log.h"

namespace lite_http {

EventLoop::EventLoop(std::string name)
    : name_(std::move(name)),
      dispatcher_(new SelectDispatcher(this)),
      thread_id_(std::this_thread::get_id()) {}

EventLoop::~EventLoop() = default;

void EventLoop::RunLoop() {
  loop_ = true;
  is_stop_ = false;
  LOG_INFO("EventLoop(%s) Start looping.", name_.c_str());
  while (!is_stop_) {
    activate_channels_.clear();
    LOG_INFO("Dispatch.");
    dispatcher_->Dispatch(0, activate_channels_);

    LOG_INFO("handle channels(%d).", activate_channels_.size());
    event_handling_ = true;
    for (Channel *ch : activate_channels_) {
      ch->HandleEvent();
    }
    event_handling_ = false;

    HandlePendingFunctors();
  }
  LOG_INFO("EventLoop(%s) stop looping.", name_.c_str());
  loop_ = false;
}

void EventLoop::Quit() {
  is_stop_ = true;
  // TODO Wakeup if not in thread
  if (!IsInThread())
    Wakeup();
}

void EventLoop::HandlePendingFunctors() {
  calling_pending_functors_ = true;
  std::vector<Functor> functors;
  {
    std::lock_guard<std::mutex> guard(mtx_);
    functors.swap(pending_functors_);
  }

  LOG_INFO("handle callbacks(%d).", functors.size());
  for (const Functor &func : functors) {
    func();
  }
  calling_pending_functors_ = false;
}

void EventLoop::RunInLoop(Functor cb) {
  if (IsInThread())
    cb();
  else
    QueueInLoop(std::move(cb));
}

void EventLoop::QueueInLoop(Functor cb) {
  {
    std::lock_guard<std::mutex> guard(mtx_);
    pending_functors_.push_back(std::move(cb));
  }
  // TODO Wakeup
  if (!IsInThread() || calling_pending_functors_)
    Wakeup();
}

void EventLoop::Wakeup() {
  // TODO
}

void EventLoop::HandleRead() {
  // TODO
}

void EventLoop::AddChannel(Channel *ch) {
  dispatcher_->Add(ch);
}

void EventLoop::UpdateChannel(Channel *ch) {
  dispatcher_->Update(ch);
}

void EventLoop::DeleteChannel(Channel *ch) {
  dispatcher_->Del(ch);
}

} // namespace
