
#include "log/Log.h"
#include "core/Channel.h"
#include "tcp/TcpServer.h"

namespace lite_http {

void TcpServer::Start() {
  assert(!acceptor_->Listening());
  loop_->RunInLoop(std::bind(&Acceptor::Listen, acceptor_.get()));
  pools_->Start();
}

void TcpServer::EstablishConn(int fd, const INetAddress &addr) {
  char buf[128];
  snprintf(buf, sizeof buf, "-%s#%d", ip_port_.c_str(), fd);
  // TODO thread pool io loop
  EventLoop *io_loop = pools_->NextLoop();

  std::string key = name_ + buf;
  TcpConnectionPtr conn_sp(new TcpConnection(io_loop, fd, key));

  LOG_INFO("establish new connection(%s)", key.c_str());
  conn_map_[key] = conn_sp;

  conn_sp->SetConnCb(conn_cb_);
  conn_sp->SetWriteComCb(write_com_cb_);
  conn_sp->SetMessageCb(message_cb_);
  conn_sp->SetConnCloseCb(std::bind(&TcpServer::RemoveConnection, this, std::placeholders::_1));
  io_loop->RunInLoop(std::bind(&TcpConnection::Establish, conn_sp));
}

void TcpServer::RemoveConnection(const TcpConnectionPtr &conn_sp) {
  // TODO thread pool io loop
  LOG_INFO("remove connection %s", conn_sp->ConnName());
  conn_map_.erase(conn_sp->ConnName());
  EventLoop *loop = conn_sp->GetLoop();
  loop->QueueInLoop(std::bind(&TcpConnection::Destroy, conn_sp));
}

} // namespace
