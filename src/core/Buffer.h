#ifndef _BUFFER_H
#define _BUFFER_H

#include <cstring>
#include <cassert>
#include <vector>

#include <sys/uio.h>

#include "socket/Common.h"

namespace lite_http {

class Buffer {
public:
    static const size_t INIT_BUFFER_SIZE = 65535;
    explicit Buffer(size_t cap = INIT_BUFFER_SIZE)
        : m_cap(cap),
        m_data(cap),
        m_read_idx(0),
        m_write_idx(0) {}
    ~Buffer() {}

    const char* read_ptr() const { return m_data.data() + m_read_idx; }

    std::string get_readable_as_str() { return get_readable_as_str(readable_size());}

    std::string get_readable_as_str(size_t sz) {
        assert(sz <= readable_size());
        const char* p = read_ptr();
        reterive(sz);
        return std::string(p, p + sz);
    }

    size_t capacity() const { return m_cap; }
    size_t readable_size() const { return m_write_idx - m_read_idx; }
    size_t writable_size() const { return m_data.size() - m_write_idx; }
    size_t prepend_size() const { return m_read_idx; }

    void append(const char* data, size_t sz) {
        ensure_writable(sz);
        std::copy(data, data + sz, begin_write());
        has_written(sz);
    }

    ssize_t write_to_fd(int fd) {
        ssize_t ret = write(fd, begin_read(), readable_size());
        if (ret > 0)
            m_read_idx += ret;
        return ret;
    }

    ssize_t read_from_fd(int fd) {
        char temp[INIT_BUFFER_SIZE];
        struct iovec vec[2];
        size_t max_writable = writable_size();
        vec[0].iov_base = begin_write();
        vec[0].iov_len = max_writable;
        vec[1].iov_base = temp;
        vec[1].iov_len = INIT_BUFFER_SIZE;
        ssize_t ret = readv(fd, vec, 2);

        if (ret < 0) {
            return errno;
        } else if (ret <= max_writable) {
            m_write_idx += ret;
        } else {
            m_write_idx = m_cap;
            append(temp, ret - max_writable);
        }
        return ret;
    }

    void reterive(size_t n) {
        assert(n <= readable_size());
        if (n < readable_size())
            m_read_idx += n;
        else
            m_read_idx = m_write_idx = 0;
    }
private:
    std::vector<char> m_data;
    size_t m_cap { MAX_LINE }, m_write_idx { 0 }, m_read_idx { 0 };

    char* begin() { return &(*m_data.begin()); }
    char* begin_read() { return begin() + m_read_idx; }
    char* begin_write() { return begin() + m_write_idx; }

    void make_space(size_t sz) {
        size_t readable = readable_size();
        std::copy(begin_read(), begin_write(), begin());
        m_read_idx = 0;
        m_write_idx = m_read_idx + readable;
        if (writable_size() <= sz) {
            m_data.resize(m_write_idx + sz);
        }
        assert(readable_size() == readable);
    }

    void ensure_writable(size_t sz) {
        if (writable_size() < sz)
            make_space(sz);
        assert(writable_size() >= sz);
    }

    void has_written(size_t sz) {
        assert(sz <= writable_size());
        m_write_idx += sz;
    }
};
}


#endif
