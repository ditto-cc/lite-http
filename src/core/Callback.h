#ifndef _CALLBACK_H
#define _CALL_BACK_H

namespace lite_http {
    class Callback {
    public:
        virtual void readCallback() = 0;
        virtual void writeCallback() = 0;
    };
}

#endif