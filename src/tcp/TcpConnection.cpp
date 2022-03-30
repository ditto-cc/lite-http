
#include "socket/Common.h"
#include "tcp/TcpConnection.h"
#include "log/Log.h"

namespace lite_http {

void TcpConnection::HandleRead() {
  size_t n = buf_in_.ReadFromFd(ch_->Fd());
  if (n > 0) {
    LOG_INFO("handle read");
    message_cb_(shared_from_this());
  } else if (n == 0) {
    HandleClose();
  } else {
    HandleError();
  }
}

void TcpConnection::HandleWrite() {
  if (ch_->Writable()) {
    size_t n = DoSend(ch_->Fd(), buf_out_.ReadPtr(), buf_out_.ReadableSize());
    if (n > 0) {
      buf_out_.Retrieve(n);
      if (buf_out_.ReadableSize() == 0) {
        ch_->DisableWrite();
        if (write_com_cb_)
          loop_->QueueInLoop(std::bind(write_com_cb_, shared_from_this()));
        if (state_ == DISCONNECTING) {
          if (!ch_->Writable())
            socket_.ShutdownWrite();
        }
      }
    } else {
      // TODO nonblocking
    }
  } else {
    // TODO log
  }
}

void TcpConnection::HandleClose() {
  LOG_WARN("%s handle close.", name_.c_str());
  assert(state_ == DISCONNECTING || state_ == CONNECTED);
  state_ = DISCONNECTED;
  ch_->DisableAll();
  std::shared_ptr<TcpConnection> conn_sp(shared_from_this());
  conn_cb_(conn_sp);
  conn_close_cb_(conn_sp);
}

void TcpConnection::HandleError() {
  // TODO
  LOG_WARN("%s handle error.", name_.c_str());
}

void TcpConnection::Send(const char *data, size_t size) {
  if (state_ == CONNECTED) {
    SendInLoop(data, size);
  }
}

void TcpConnection::SendInLoop(const char *data, size_t len) {
  if (state_ == DISCONNECTED) {
    LOG_WARN("%s Disconnected, give up write.", name_.c_str());
    return;
  }

  size_t nwrite = 0, remain = len;
  bool fault = false;
  // TODO nonblocking Send directly
  if (!fault && remain > 0) {
    buf_out_.Append(data + nwrite, remain);
    if (!ch_->Writable())
      ch_->EnableWrite();
  }
}

void TcpConnection::Establish() {
  assert(state_ == CONNECTING);
  state_ = CONNECTED;
  // channel Tie this
  ch_->Tie(shared_from_this());
  ch_->EnableRead();
  conn_cb_(shared_from_this());
}

void TcpConnection::Destroy() {
  if (state_ == CONNECTED) {
    state_ = DISCONNECTED;
    ch_->DisableAll();
    conn_cb_(shared_from_this());
  }
  ch_->Remove();
}
}
