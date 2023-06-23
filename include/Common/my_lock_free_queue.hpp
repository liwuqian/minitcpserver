#pragma once
#include "boost/lockfree/queue.hpp"
#include "boost/noncopyable.hpp"
#include "boost/thread/mutex.hpp"
#include <list>
#include <Windows.h>
#include <mutex>
#include <memory>
#include <functional>
#include <condition_variable>

namespace myspace {

    // method 1: fixed-size queue, but low efficiency 
    template<class T, uint64_t MAXSIZE = 1024>
    class MyLockfreeQueue : private boost::noncopyable
    {
    public:
        MyLockfreeQueue() : m_iMaxLength(MAXSIZE) {}
        virtual ~MyLockfreeQueue() {}

    public:
        bool IsEmpty()
        {
            boost::mutex::scoped_lock lock(m_mutex);
            return m_list.empty();
        }
        int Length()
        {
            boost::mutex::scoped_lock lock(m_mutex);
            return m_list.size();
        }
        bool pop(T& data)
        {
            boost::mutex::scoped_lock lock(m_mutex);
            if (!m_list.empty())
            {
                data = m_list.front();
                m_list.pop_front();
                return true;
            }
            return false;

        }
        bool push(const T& data)
        {
            boost::mutex::scoped_lock lock(m_mutex);
            if (m_list.size() > m_iMaxLength)
            {
                return false;
            }
            m_list.push_back(data);
            return true;
        }

    private:
        boost::mutex m_mutex;
        uint64_t m_iMaxLength;
        std::list<T> m_list;
    };

    // method 2: not support std::string
    template<typename T_type, uint64_t size = 1024>
    class FreeQueue : private boost::noncopyable
    {
    public:
        static bool push(const T_type& data)
        {
            return m_queue.push(data);
        }

        static bool pop(T_type& data)
        {
            return m_queue.pop(data);
        }
    private:
        boost::lockfree::queue<T_type, boost::lockfree::capacity<size>> m_queue;
    };

    // method 3: high efficiency, but cause memory fragment
    template <typename T>
    class threadsafe_queue
    {
    public:
        threadsafe_queue()
            : head(new node)
            , tail(head.get())
        {}
        threadsafe_queue(const threadsafe_queue&) = delete;
        threadsafe_queue operator=(const threadsafe_queue&) = delete;
        ~threadsafe_queue() = default;

        std::shared_ptr<T> try_pop()
        {
            std::unique_lock<std::mutex> ulkh(head_mut);
            if (head.get() == get_tail())
                return nullptr;
            auto old_head = std::move(head);
            head = std::move(old_head->next);
            return old_head->data;
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock<std::mutex> ulkh(head_mut);
            {
                std::unique_lock<std::mutex> ulkt(tail_mut);
                data_cond.wait(ulkt, [&]()
                    {
                        return head.get() != tail;
                    });
            }
            auto old_head = std::move(head);
            head = std::move(old_head->next);
            return old_head->data;
        }

        void push(T&& t)
        {
            std::shared_ptr<T> new_data(std::make_shared<T>(std::forward<T>(t)));
            std::unique_ptr<node> new_tail(new node);
            node* p = new_tail->get();
            {
                std::unique_lock<std::mutex> ulkt(tail_mut);
                tail->data = new_data;
                tail->next = std::move(new_tail);
                tail = p;
            }
            data_cond.notify_one();
        }

    private:
        struct node
        {
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
        };

        std::mutex head_mut;
        std::mutex tail_mut;
        std::unique_ptr<node> head;
        node* tail;
        std::condition_variable data_cond;

    private:
        node* get_tail()
        {
            std::unique_lock<std::mutex> ulkt(tail_mut);
            return tail;
        }
    };
}