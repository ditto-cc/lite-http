#ifndef _BUFFER_H
#define _BUFFER_H

#include "socket/Common.h"

namespace lite_http {

    class Buffer {
    private:
        unique_ptr<char> data;
        size_t m_size { 0 };
        long m_wrt_idx{ 0 }, m_rd_idx{ 0 };
    public:
        explicit Buffer(long size = MAX_LINE) {
            data.reset(new char[size]);
        }
        ~Buffer() = default;

        void* get_buf_in() const { return data.get(); }
        void set_size(size_t size) {
            m_size = size;
            data.get()[size] = '\0';
        }
        size_t size() const { return m_size; }
        int getReadIndex() const { return m_rd_idx; }
        int getWriteIndex() const { return m_wrt_idx; }
    };
}


#endif