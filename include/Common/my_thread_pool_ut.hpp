#pragma once

#include <iostream>
#include "my_thread_pool.hpp"
#include <functional>

namespace myspace {
    void func()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //std::this_thread::sleep_for(std::chrono::seconds(100));
        std::cout << "worker thread ID:" << std::this_thread::get_id() << std::endl;
    }

    class ClassFunc
    {
    public:
        void operator()(int x, int y)
        {
            std::cout << "x y: " << x << y << std::endl;
        }

        void worker(int x)
        {
            int sum = 5;
            while (sum--)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::cout << "Thread ID: " << std::this_thread::get_id() << ", X: " << x << std::endl;
            }

        }
    };

    class ThreadPoolTest
    {
    public:
        void test1()
        {
            // create thread pool with 4 worker threads
            ThreadPool pool(4);

            // enqueue and store future
            auto result = pool.enqueue([](int answer) { return answer; }, 42);

            // get result from future, print 42
            std::cout << result.get() << std::endl;
        }

        void test2()
        {
            ThreadPool pool(4);
            int num = 10000;
            while (--num)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                pool.enqueue(func);
            }
        }

        
        void test3()
        {
            ThreadPool pool(4);
            int x = 3, y = 4;
            pool.enqueue(ClassFunc(), x, y);
        }

        void test4()
        {
            int sum = 0;
            ThreadPool pool(2);
            ClassFunc func;
            pool.enqueue(std::bind(&ClassFunc::worker, &func, 500));
            pool.enqueue(std::bind(&ClassFunc::worker, &func, 600));
            pool.enqueue(std::bind(&ClassFunc::worker, &func, 700));
        }
    };  
}