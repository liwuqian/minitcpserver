#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace myspace {
    class ThreadPool {

    public:
        ThreadPool(size_t);                          
        template<class F, class... Args>           
        auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;
        ~ThreadPool();                             

    private:
        std::vector< std::thread > workers;            // thread queue
        std::queue< std::function<void()> > tasks;     // task queue  

        std::mutex queue_mutex;                        
        std::condition_variable condition;             
        bool stop;                                    
    };

    inline ThreadPool::ThreadPool(size_t threads) : stop(false) {

        for (size_t i = 0; i < threads; ++i)
            workers.emplace_back(
                [this]
                {
                    for (;;)
                    {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->queue_mutex);

                            //��1����������������falseʱ�������̣߳�����ʱ�Զ��ͷ�����
                            //��2����������������true���ܵ�֪ͨʱ��������Ȼ�������
                            this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });

                            if (this->stop && this->tasks.empty())
                                return;
                            // Get a task
                            task = std::move(this->tasks.front());
                            this->tasks.pop();
                        }                            // auto unlock
                        task();                      // execute
                    }
                }
                );
    }

    // push task into queue
    template<class F, class... Args>
    auto ThreadPool::enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>
    {
        // get return type   
        using return_type = typename std::result_of<F(Args...)>::type;

        // make shared pointer to task
        auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);  
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });         // push task
        }                                                   // auto unlock
        condition.notify_one();                             // wake up suspend thread
        return res;
    }

    inline ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }
} // myspace