#ifndef _CHANNELELEMENT_H
#define _CHANNELELEMENT_H

#include <utility>

#include "core/Channel.h"

namespace lite_http {

enum ChannelElementType {
    CHANNEL_ADD, CHANNEL_UPDATE, CHANNEL_DELETE
};

struct ChannelElement {
public:
    std::shared_ptr<Channel> channel;
    int type;
    ChannelElement(std::shared_ptr<Channel> ch, int type): channel(std::move(ch)), type(type) {}
};
}

#endif
