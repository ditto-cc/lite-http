#ifndef _NOCOPYABLE_H
#define _NOCOPYABLE_H

namespace lite_http {
/**
  禁用拷贝复制
 */
class Nocopyable {
 public:
  Nocopyable() = default;
  ~Nocopyable() = default;
  Nocopyable(const Nocopyable &) = delete;
  Nocopyable &operator=(const Nocopyable &) = delete;
};
}

#endif