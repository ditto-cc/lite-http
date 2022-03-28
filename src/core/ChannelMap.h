
#ifndef _CHANNEL_MAP_H
#define _CHANNEL_MAP_H

#include "core/Channel.h"
#include "util/nocopyable.h"

#include <map>
#include <memory>

namespace lite_http {
class ChannelMap : private nocopyable {
private:
    std::map<int, Channel*> m_data;
public:
    ChannelMap() = default;
    ~ChannelMap() = default;

    Channel* get_channel(int fd) {
        return m_data.at(fd);
    }

    void add_channel(int fd, Channel* ch) {
        m_data.insert({fd, ch});
    }

    void remove_channel(int fd) {
        m_data.erase(fd);
    }

    bool contains_channel(int fd) const {
        return m_data.count(fd) == 1;
    }

    bool empty() const {
        return m_data.empty();
    }
    
    std::map<int, Channel*>::iterator begin() {
        return m_data.begin();
    }
    
    std::map<int, Channel*>::iterator end() {
        return m_data.end();
    }
    int max_fd() const {
        if (m_data.empty()) return -1;
        return m_data.rbegin()->first;
    }
};
}

#endif
