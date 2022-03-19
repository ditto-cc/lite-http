
#ifndef _CHANNEL_MAP_H
#define _CHANNEL_MAP_H

#include "core/Channel.h"
#include "util/nocopyable.h"

#include <unordered_map>
#include <memory>

namespace lite_http {
    class ChannelMap : private nocopyable {
    private:
        std::unordered_map<int, std::shared_ptr<Channel>> m_data;
    public:
        ChannelMap() = default;
        ~ChannelMap() = default;

        Channel& get_channel(int fd) {
            return *m_data.at(fd);
        }

        void add_channel(int fd, const Channel& ch) {
            std::shared_ptr<Channel> p(new Channel(ch));
            m_data[fd] = std::move(p);
        }

    };
}

#endif