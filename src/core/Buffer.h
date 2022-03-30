#ifndef _BUFFER_H
#define _BUFFER_H

#include <cstring>
#include <cassert>
#include <vector>

#include <sys/uio.h>

#include "socket/Common.h"

namespace lite_http {

static const size_t kInitBufferSize = 255;
class Buffer {
 public:
  explicit Buffer(size_t cap = kInitBufferSize)
      : cap_(cap),
        data_(cap),
        read_idx_(0),
        write_idx_(0) {}
  ~Buffer() = default;

  const char *ReadPtr() const { return data_.data() + read_idx_; }

  std::string GetReadableAsStr() { return GetReadableAsStr(ReadableSize()); }

  std::string GetReadableAsStr(size_t sz) {
    assert(sz <= ReadableSize());
    const char *p = ReadPtr();
    Retrieve(sz);
    return {p, p + sz};
  }

  size_t Capacity() const { return cap_; }
  size_t ReadableSize() const { return write_idx_ - read_idx_; }
  size_t WritableSize() const { return data_.size() - write_idx_; }
  size_t PrependSize() const { return read_idx_; }

  void Append(const char *data, size_t sz) {
    EnsureWritable(sz);
    std::copy(data, data + sz, BeginWrite());
    HasWritten(sz);
  }

  ssize_t ReadFromFd(int fd) {
    char temp[kInitBufferSize];
    struct iovec vec[2];
    size_t max_writable = WritableSize();
    vec[0].iov_base = BeginWrite();
    vec[0].iov_len = max_writable;
    vec[1].iov_base = temp;
    vec[1].iov_len = kInitBufferSize;
    ssize_t ret = readv(fd, vec, 2);

    if (ret < 0) {
      return errno;
    } else if (ret <= max_writable) {
      write_idx_ += ret;
    } else {
      write_idx_ = cap_;
      Append(temp, ret - max_writable);
    }
    return ret;
  }

  void Retrieve(size_t n) {
    assert(n <= ReadableSize());
    if (n < ReadableSize())
      read_idx_ += n;
    else
      read_idx_ = write_idx_ = 0;
  }
 private:
  std::vector<char> data_;
  size_t cap_, write_idx_, read_idx_;

  char *Begin() { return &(*data_.begin()); }
  char *BeginRead() { return Begin() + read_idx_; }
  char *BeginWrite() { return Begin() + write_idx_; }

  void MakeSpace(size_t sz) {
    size_t readable = ReadableSize();
    std::copy(BeginRead(), BeginWrite(), Begin());
    read_idx_ = 0;
    write_idx_ = read_idx_ + readable;
    if (WritableSize() <= sz) {
      data_.resize(write_idx_ + sz);
    }
    assert(ReadableSize() == readable);
  }

  void EnsureWritable(size_t sz) {
    if (WritableSize() < sz)
      MakeSpace(sz);
    assert(WritableSize() >= sz);
  }

  void HasWritten(size_t sz) {
    assert(sz <= WritableSize());
    write_idx_ += sz;
  }
};
}

#endif
