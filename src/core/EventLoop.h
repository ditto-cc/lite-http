#ifndef _EVENTLOOP_H
#define _EVENTLOOP_H

#include <unordered_map>
#include <list>
#include <memory>
#include <string>
#include <queue>
#include <thread>
#include <mutex>

#include "core/Channel.h"
#include "core/EventDispatcher.h"
#include "util/Nocopyable.h"

namespace lite_http {
class EventLoop : Nocopyable {
 public:
  typedef std::function<void()> Functor;

  explicit EventLoop(std::string name);
  ~EventLoop();

  void RunLoop();
  void Quit();

  void RunInLoop(Functor cb);
  void QueueInLoop(Functor cb);

  void AddChannel(Channel *ch);
  void UpdateChannel(Channel *ch);
  void DeleteChannel(Channel *ch);

 private:
  void HandlePendingFunctors();
  bool IsInThread() const { return std::this_thread::get_id() == thread_id_; }
  void Wakeup();
  void HandleRead();

 private:
  std::string name_;
  bool is_stop_{false}, loop_{false};
  bool calling_pending_functors_ {false}, event_handling_ {false};
  std::unique_ptr<EventDispatcher> dispatcher_;
  std::vector<Channel *> activate_channels_;
  std::vector<Functor> pending_functors_;
  std::thread::id thread_id_;
  mutable std::mutex mtx_;
};
} // namespace

#endif
