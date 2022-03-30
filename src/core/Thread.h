#ifndef _THREAD_H
#define _THREAD_H

#include <list>
#include <vector>

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>

#include "util/Nocopyable.h"

namespace lite_http {

class ThreadPool : private Nocopyable {
 public:
  typedef std::function<void()> TaskFunc;
  explicit ThreadPool(int thread_num);
  ~ThreadPool();

  void AddTask(TaskFunc taskFunc);
  void RemoveAllThreads();

 private:
  void ThreadFunc();

 private:
  int m_th_num;
  bool m_running;
  std::list<TaskFunc> m_tasks;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  std::vector<std::unique_ptr<std::thread>> m_threads;
};
} // namespace

#endif
