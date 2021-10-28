#ifndef _CHANNELELEMENT_H
#define _CHANNELELEMENT_H

#include "core/Channel.h"

namespace lite_http {
    class ChannelElement {
    private:
        Channel channel;
        int type;
    public:
        ChannelElement() {}
        ~ChannelElement() {}
    };
}

#endif