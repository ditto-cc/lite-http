#ifndef _QUEUE_H
#define _QUEUE_H

#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace lite_http {
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;

    using std::mutex;
    using std::condition_variable;

    template <typename T>
    class Queue {
    private:
        struct Node {
            T data;
            shared_ptr<Node> next;
            Node() {}
            Node(const T& data) : data(data) {}
            ~Node() {}
        };
        size_t m_size, m_cap;
        shared_ptr<Node> m_head, m_tail;
        mutex m_mtx;
        condition_variable m_cv;

    public:
        Queue() : m_size(0) { m_head = m_tail = shared_ptr<Node>(new Node()); }
        Queue(const Queue<T>&) = delete;
        ~Queue() = default;

        size_t size() { return m_size; }

        bool empty() { return m_size == 0; }

        T& front() {
            if (m_size == 0)
                throw "Empty Queue.";
            return m_head->next->data;
        }

        void pop() {
            auto lock = std::unique_lock<mutex>(m_mtx);
            m_cv.wait(lock, [&]() { return !empty(); });
            m_head = m_head->next;
            --m_size;
        }

        void push(const T& elem) {
            auto lock = std::unique_lock<mutex>(m_mtx);
            m_cv.wait(lock, [&]() { return m_size < m_cap; });
            shared_ptr<Node> new_tail(new Node(elem));
            m_tail->next = new_tail;
            m_tail = new_tail;
            ++m_size;
        }


    };
}

#endif