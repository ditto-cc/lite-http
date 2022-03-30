
#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include "core/EventLoop.h"
#include "socket/INetAddress.h"
#include "socket/Socket.h"
#include "util/Nocopyable.h"

namespace lite_http {

class Acceptor : private Nocopyable {
 public:
  Acceptor(
      EventLoop *loop,
      const INetAddress &listen_addr,
      bool is_blocking, bool reuse_port = true)
      : loop_(loop),
        server_(MakeSocket(AF_INET, SOCK_STREAM, is_blocking)),
        accept_ch_(loop, server_.Fd()),
        listening_(false) {
    server_.Bind(listen_addr);
    server_.SetReuseAddr(true);
    server_.SetReusePort(reuse_port);
    accept_ch_.SetReadCallback(std::bind(&Acceptor::HandleRead, this));
  }

  ~Acceptor() = default;

  typedef std::function<void(int, const INetAddress &)> NewConnCallback;

  void Listen();
  bool Listening() const { return listening_; }
  void SetNewConnCb(NewConnCallback cb) { conn_callback_ = std::move(cb); }

 private:
  EventLoop *loop_;
  Socket server_;
  Channel accept_ch_;
  bool listening_;
  NewConnCallback conn_callback_;

  void HandleRead();
};
} // namespace

#endif //_ACCEPTOR_H
