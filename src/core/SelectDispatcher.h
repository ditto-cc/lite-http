#ifndef _SELECT_DISPATCHER_H
#define _SELECT_DISPATCHER_H

#include <sys/select.h>

#include "core/EventDispatcher.h"

namespace lite_http {
class SelectDispatcher : public EventDispatcher {
 public:
  explicit SelectDispatcher(
      EventLoop *event_loop,
      const char *name = "select dispatcher")
      : EventDispatcher(event_loop, name) {
    Init();
  }
  ~SelectDispatcher() override = default;

  void Init() override;
  void Add(Channel *channel) override;
  void Del(Channel *channel) override;
  void Update(Channel *channel) override;
  void Dispatch(struct timeval *timeval, std::vector<Channel *> &activate_channels) override;
  void Clear() override;

 private:
  fd_set readset_ {}, writeset_ {}, exset_ {};
  fd_set readmask_ {};, writemask_ {}, exmask_ {};
};
}

#endif
