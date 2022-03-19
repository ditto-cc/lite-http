#include "core/Thread.h"
#include "log/Log.h"

namespace lite_http {
    ThreadPool::ThreadPool(int thread_num) : m_th_num(thread_num), m_running(true) {
        if (m_th_num <= 0) m_th_num = 5;
        m_threads.reserve(m_th_num);
        for (int i = 0; i < thread_num; ++i) {
            std::shared_ptr<std::thread> spThread(new std::thread(&ThreadPool::threadFunc, this));
            m_threads.push_back(spThread);
        }
    }

    void ThreadPool::threadFunc() {
        TaskFunc taskFunc;
        while (true) {
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                while (m_tasks.empty()) {
                    if (!m_running) break;
                    m_cv.wait(lock);
                }
                if (!m_running) break;
                taskFunc = m_tasks.front();
                m_tasks.pop_front();
            }

            if (!taskFunc) continue;

            taskFunc();
        }
        AsyncLogger::LogInfo("Thread Exit. %ld", std::this_thread::get_id());
    }

    void ThreadPool::addTask(const TaskFunc &taskFunc) {
        {
            std::lock_guard<std::mutex> guard(m_mtx);
            m_tasks.push_back(taskFunc);
        }
        m_cv.notify_one();
        AsyncLogger::LogInfo("Add TaskFunc.");
    }

    void ThreadPool::removeAllThreads() {
        {
            std::lock_guard<std::mutex> guard(m_mtx);
            m_tasks.clear();
        }
    }

    ThreadPool::~ThreadPool() {
        m_running = false;
        m_cv.notify_all();
        for (auto& th: m_threads)
            if (th->joinable())
                th->join();
    }

}
