
#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include "socket/Common.h"

namespace lite_http {

class INetAddress {
public:
    explicit INetAddress(uint16_t port = 0) {
        bzero(&m_addr, sizeof m_addr);
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = Host2Network16(port);
        m_addr.sin_addr.s_addr = Host2Network32(INADDR_ANY);
    }

    explicit INetAddress(const struct sockaddr_in& addr) : m_addr(addr) {}

    uint16_t port() const {
        return Network2Host16(m_addr.sin_port);
    }

    std::string ip_port_str() const {
        return Sockaddr2str(get_sockaddr());
    }

    void set_sockaddr(const struct sockaddr_in& addr) { m_addr = addr; }
    const struct sockaddr_in* get_sockaddr() const { return (const struct sockaddr_in*) (&m_addr); }
private:
    struct sockaddr_in m_addr;
};

}

#endif
