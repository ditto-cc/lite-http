#ifndef _BUFFER_H
#define _BUFFER_H

#include <cstring>
#include <sys/uio.h>
#include "socket/Common.h"

namespace lite_http {
    const size_t INIT_BUFFER_SIZE = 65535;
    class Buffer {
    private:
        char* m_data { nullptr };
        size_t m_cap { MAX_LINE }, m_write_idx { 0 }, m_read_idx { 0 };
        void do_adjust(size_t new_size) {
            if (get_writable_size() >= new_size)
                return;
            size_t readable = get_readable_size();
            if (m_read_idx + get_writable_size() >= new_size) {
                memmove(m_data, m_data + m_read_idx, readable);
                m_read_idx = 0;
                m_write_idx = readable;
            } else {
                size_t newCap = m_cap + new_size;
                char* newData = new char[newCap];
                memmove(newData, m_data, readable);
                delete[] m_data;
                m_data = newData;
                m_read_idx = 0;
                m_write_idx = readable;
                m_cap = newCap;
            }
        }

        void do_append(char* data, size_t sz) {
            if (data != nullptr) {
                do_adjust(sz);
                memcpy(m_data + m_write_idx, data, sz);
                m_write_idx += sz;
            }
        }
    public:
        explicit Buffer(size_t cap = INIT_BUFFER_SIZE): m_cap(cap), m_data(new char[cap]) {}
        ~Buffer() {
            delete[] m_data;
        }

        const char* get_read_ptr() const { return m_data + m_read_idx; }
        const char* get_write_ptr() const { return m_data + m_write_idx; }

        size_t capacity() const { return m_cap; }
        size_t get_readable_size() const { return m_write_idx - m_read_idx; }
        size_t get_read_index() const { return m_read_idx; }
        size_t get_writable_size() const { return m_cap - m_write_idx; }
        size_t get_write_index() const { return m_write_idx; }

        ssize_t write_to_fd(int fd) {
            ssize_t ret = write(fd, m_data + m_read_idx, get_readable_size());
            if (ret > 0)
                m_read_idx += ret;
            return ret;
        }

        ssize_t read_from_fd(int fd) {
            char temp[INIT_BUFFER_SIZE];
            struct iovec vec[2];
            size_t max_writable = get_writable_size();
            vec[0].iov_base = m_data + m_write_idx;
            vec[0].iov_len = max_writable;
            vec[1].iov_base = temp;
            vec[1].iov_len = INIT_BUFFER_SIZE;
            ssize_t ret = readv(fd, vec, 2);
            if (ret < 0) {
                return -1;
            } else if (ret <= max_writable) {
                m_write_idx += ret;
            } else {
                m_write_idx = m_cap;
                do_append(temp, ret - max_writable);
            }
            return ret;
        }
    };
}


#endif
