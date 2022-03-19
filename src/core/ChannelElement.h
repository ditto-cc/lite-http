#ifndef _CHANNELELEMENT_H
#define _CHANNELELEMENT_H

#include <utility>

#include "core/Channel.h"

namespace lite_http {
    class ChannelElement {
    private:
        Channel m_data;
        int m_type;
    public:
        ChannelElement(Channel& ptr, int type): m_data(ptr), m_type(type) {}
        ~ChannelElement() = default;

        int type() {
            return m_type;
        }

        Channel& get_channel() {
            return m_data;
        }
    };
}

#endif