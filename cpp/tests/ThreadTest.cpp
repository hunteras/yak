#include "../ThreadPool.h"
#include "gtest/gtest.h"

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

using namespace yak;

void foo()
{
    for(int i = 0; i < 10; i++)
        std::cout << std::this_thread::get_id() << " : " << i << std::endl;
}

void bar(int x)
{
    for(int i = 0; i < 10; i++)
        std::cout << "X: " << x << " " << std::this_thread::get_id() << " : " << i << std::endl;
}

TEST(Thread, ThreadUsage)
{
    std::thread first(foo);     // spawn new thread that calls foo()
    std::thread second(bar, 0);  // spawn new thread that calls bar(0)

    std::cout << "main, foo and bar now execute concurrently...\n";

    // synchronize threads:
    first.join();                // pauses until first finishes
    second.join();               // pauses until second finishes

    std::cout << "foo and bar completed.\n";
}

TEST(ThreadPool, ThreadPoolUsage)
{
    ThreadPool pool(4);
    std::vector< std::future<int> > results;

    for(int i = 0; i < 8; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                    std::cout << "hello " << i << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    std::cout << "world " << i << std::endl;
                    return i*i;
                })
            );
    }

    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
}
