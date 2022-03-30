
#include "tcp/Acceptor.h"
#include "log/Log.h"

namespace lite_http {
void Acceptor::Listen() {
  server_.Listen();
  accept_ch_.EnableRead();
}

void Acceptor::HandleRead() {
  INetAddress peer;
  int connfd = server_.Accept(peer);
  LOG_INFO("Accept %d", connfd);
  if (connfd > 0) {
    if (conn_callback_)
      conn_callback_(connfd, peer);
    else
      ::close(connfd);
  } else {
    LOG_FATAL("Accept Fd < 0.");
  }
}
}
