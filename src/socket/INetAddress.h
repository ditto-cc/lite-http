
#ifndef _INETADDREDD_H
#define _INETADDREDD_H

#include "socket/Common.h"

namespace lite_http {

class INetAddress {
public:
    explicit INetAddress(uint16_t port = 0) {
        bzero(&m_addr, sizeof m_addr);
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = Host2Network16(port);
        m_addr.sin_addr.s_addr = Host2Network16(INADDR_ANY);
    }

    explicit INetAddress(const sockaddr_in& addr) : m_addr(addr) {}

    uint16_t port() const {
        return Network2Host16(m_addr.sin_port);
    }

    void set_sockaddr(const sockaddr_in& addr) { m_addr = addr; }
    const sockaddr_in* get_sockaddr() const { return &m_addr; }
private:
    sockaddr_in m_addr;
};

}

#endif
