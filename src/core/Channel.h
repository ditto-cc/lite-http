#ifndef _CHANNEL_H
#define _CHANNEL_H

#include <functional>
#include <utility>
#include <memory>
#include <cassert>

#include "util/Nocopyable.h"

namespace lite_http {

class EventLoop;
typedef std::function<void()> Func;

const int kEventNone = 0x00;
const int kEventRead = 0x02;
const int kEventWrite = 0x04;

class Channel : private Nocopyable {
 public:
  Channel(EventLoop *event_loop, int fd)
      : fd_(fd),
        loop_(event_loop),
        event_(kEventNone),
        revent_(kEventNone),
        add_to_loop_(false),
        tied_(false) {}
  ~Channel() = default;

  void SetWriteCallback(const Func &callback) { write_callback_ = callback; }
  void SetReadCallback(const Func &callback) { read_callback_ = callback; }

  int Fd() const { return fd_; }
  int Event() const { return event_; }
  int Revent() const { return revent_; }
  void SetRevent(int revent) { revent_ = revent; }

  void HandleEvent() {
    std::shared_ptr<void> guard;
    if (tied_) {
      guard = tie_.lock();
      if (guard)
        DoHandleEvent();
    } else {
      DoHandleEvent();
    }
  }

  void EnableWrite() {
    event_ |= kEventWrite;
    Update();
  }

  void DisableWrite() {
    event_ &= ~kEventWrite;
    Update();
  }

  void EnableRead() {
    event_ |= kEventRead;
    Update();
  }

  void DisableRead() {
    event_ &= ~kEventRead;
    Update();
  }

  void DisableAll() {
    event_ = kEventNone;
    Update();
  }

  bool Readable() const { return event_ & kEventRead; }
  bool Writable() const { return event_ & kEventWrite; }
  bool IsNone() const { return event_ == kEventNone; }

  void Tie(const std::shared_ptr<void> &ptr) {
    tie_ = ptr;
    tied_ = true;
  }

  void Remove();

 private:
  EventLoop *loop_;
  int fd_, event_, revent_;
  bool add_to_loop_, tied_;
  std::weak_ptr<void> tie_;
  Func read_callback_, write_callback_;

  void Update();
  void DoHandleEvent() {
    if (revent_ & kEventRead)
      if (read_callback_)
        read_callback_();
    if (revent_ & kEventWrite)
      if (write_callback_)
        write_callback_();
  }
};
} // namespace

#endif
