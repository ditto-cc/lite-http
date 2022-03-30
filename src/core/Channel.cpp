
#include <cassert>

#include "core/Channel.h"
#include "core/EventLoop.h"

namespace lite_http {
void Channel::Update() {
  add_to_loop_ = true;
  loop_->UpdateChannel(this);
}
void Channel::Remove() {
  assert(event_ == kEventNone);
  loop_->DeleteChannel(this);
  add_to_loop_ = false;
}

}
