#ifndef _TCP_SERVER_H
#define _TCP_SERVER_H

#include <utility>
#include <iostream>
#include <string>

#include "core/EventLoop.h"
#include "core/EventLoopThreadPool.h"
#include "socket/Socket.h"
#include "tcp/TcpConnection.h"
#include "tcp/Acceptor.h"
#include "util/Nocopyable.h"

namespace lite_http {
class TcpServer : private Nocopyable {
 public:
  TcpServer(
      EventLoop *event_loop,
      const INetAddress &addr,
      const std::string &name,
      int thread_num = 0,
      bool blocking = true)
      : loop_(event_loop),
        name_(name),
        ip_port_(std::move(addr.IpPortStr())),
        thread_num_(thread_num),
        pools_(new EventLoopThreadPool(event_loop, name, thread_num)),
        acceptor_(new Acceptor(event_loop, addr, blocking)) {
    acceptor_->SetNewConnCb(
        std::bind(&TcpServer::EstablishConn, this, std::placeholders::_1, std::placeholders::_2));
  }
  ~TcpServer() = default;

  void Start();
  void SetConnCb(ConnCallback cb) { conn_cb_ = std::move(cb); }
  void SetWriteComCb(ConnCallback cb) { write_com_cb_ = std::move(cb); }
  void SetMessageCb(ConnCallback cb) { message_cb_ = std::move(cb); }

 private:
  EventLoop *loop_;
  std::unique_ptr<EventLoopThreadPool> pools_;
  std::unique_ptr<Acceptor> acceptor_;
  int thread_num_;
  const std::string name_, ip_port_;
  std::unordered_map<std::string, TcpConnectionPtr> conn_map_;
  ConnCallback conn_cb_, conn_close_cb_, write_com_cb_, message_cb_;

  void EstablishConn(int fd, const INetAddress &addr);
  void RemoveConnection(const TcpConnectionPtr &conn_sp);
};

} // namespace

#endif
