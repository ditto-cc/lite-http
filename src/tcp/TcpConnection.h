#ifndef _TCP_CONNECTION_H
#define _TCP_CONNECTION_H

#include <string>

#include "core/EventLoop.h"
#include "core/Buffer.h"
#include "socket/Socket.h"
#include "util/Nocopyable.h"

namespace lite_http {

class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr &)> ConnCallback;

class TcpConnection : private Nocopyable, public std::enable_shared_from_this<TcpConnection> {
 public:
  TcpConnection(
      EventLoop *loop,
      int connfd,
      std::string name)
      : loop_(loop),
        state_(CONNECTING),
        name_(std::move(name)),
        socket_(connfd),
        ch_(new Channel(loop, connfd)) {
    LOG_INFO("TcpConnection ctor %d", ch_->Fd());
    ch_->SetReadCallback(std::bind(&TcpConnection::HandleRead, this));
    ch_->SetWriteCallback(std::bind(&TcpConnection::HandleWrite, this));
  }

  ~TcpConnection() {
    LOG_INFO("TcpConnection dtor %d", ch_->Fd());
    assert(state_ == DISCONNECTED);
  }

  EventLoop *GetLoop() { return loop_; }

  Buffer &GetBufIn() { return buf_in_; }
  Buffer &GetBufOut() { return buf_out_; }
  const char *ConnName() const { return name_.c_str(); }
  bool Connected() const { return state_ == CONNECTED; }
  bool Disconnected() const { return state_ == DISCONNECTED; }

  void Shutdown();
  void ForceClose();
  void StartRead();
  void StopRead();

  void SetConnCb(ConnCallback cb) { conn_cb_ = std::move(cb); }
  void SetConnCloseCb(const ConnCallback &cb) { conn_close_cb_ = cb; }
  void SetWriteComCb(const ConnCallback &cb) { write_com_cb_ = cb; }
  void SetMessageCb(const ConnCallback &cb) { message_cb_ = cb; }

  void Establish();
  void Destroy();

  void Send(const char *data, size_t size);
 private:
  enum State { CONNECTING, CONNECTED, DISCONNECTING, DISCONNECTED };
  ConnCallback conn_cb_, conn_close_cb_, write_com_cb_, message_cb_;

  EventLoop *loop_;
  std::string name_;
  Buffer buf_in_, buf_out_;
  std::unique_ptr<Channel> ch_;
  Socket socket_;
  int state_;

  void HandleRead();
  void HandleWrite();
  void HandleClose();
  void HandleError();

  void SendInLoop(const char *data, size_t len);
};
}

#endif
