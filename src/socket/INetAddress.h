#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include "socket/Common.h"

namespace lite_http {

class INetAddress {
 public:
  explicit INetAddress(uint16_t port = 0) {
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_port = Host2Network16(port);
    addr_.sin_addr.s_addr = Host2Network32(INADDR_ANY);
  }

  explicit INetAddress(const struct sockaddr_in &addr) : addr_(addr) {}

  uint16_t Port() const {
    return Network2Host16(addr_.sin_port);
  }

  std::string IpPortStr() const {
    return Sockaddr2Str(GetSockaddr());
  }

  void SetSockaddr(const struct sockaddr_in &addr) { addr_ = addr; }
  const struct sockaddr_in *GetSockaddr() const { return (const struct sockaddr_in *) (&addr_); }

 private:
  struct sockaddr_in addr_{};
};

}

#endif
