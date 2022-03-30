#ifndef _EVENT_DISPATCHER_H
#define _EVENT_DISPATCHER_H

#include <map>
#include <vector>

#include "core/Channel.h"

namespace lite_http {

class EventLoop;

class EventDispatcher {
 public:
  explicit EventDispatcher(EventLoop *event_loop, const char *name) : loop_(event_loop), name_(name) {}
  virtual ~EventDispatcher() = default;
  virtual void Init() = 0;
  virtual void Add(Channel *channel) = 0;
  virtual void Del(Channel *channel) = 0;
  virtual void Update(Channel *channel) = 0;
  virtual void Dispatch(struct timeval *timeval, std::vector<Channel *> &activate_channels) = 0;
  virtual void Clear() = 0;
 protected:
  const char *name_;
  std::map<int, Channel *> map_;
  EventLoop *loop_;
};
}

#endif
