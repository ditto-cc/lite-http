#ifndef _SOCKET_H
#define _SOCKET_H

#include "socket/INetAddress.h"
#include "core/Buffer.h"

namespace lite_http {
class Socket {
 public:
  explicit Socket(int fd) : fd_(fd) {}
  ~Socket() = default;

  int Fd() const { return fd_; }
  void SetReuseAddr(bool on) const {
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  }

  void SetReusePort(bool on) const {
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  }

  void SetKeepalive(bool on) const {
    int opt = on ? 1 : 0;
    setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
  }

  void SetNoDelay(bool on) const {
    int opt = on ? 1 : 0;
    setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
  }

  void Bind(const INetAddress &addr) const {
    const sockaddr_in *saddr = addr.GetSockaddr();
    DoBind(fd_, saddr, sizeof(*saddr));
  }

  void Listen() const {
    DoListen(fd_, SOMAXCONN);
  }

  int Accept(INetAddress &peeraddr) const {
    sockaddr_in saddr{};
    ::bzero(&saddr, sizeof saddr);
    int connfd = DoAccept(fd_, &saddr);
    if (connfd >= 0) {
      peeraddr.SetSockaddr(saddr);
    }
    MakeNonblocking(connfd);

    return connfd;
  }

  void ShutdownWrite() const {
    DoShutdownWrite(fd_);
  }

 private:
  int fd_;
};
}

#endif
