#ifndef _CHANNEL_H
#define _CHANNEL_H

namespace lite_http {
    class Channel {
    private:
        int fd, event_type;
    public:
        Channel() {}
        ~Channel() {}
    };
}

#endif