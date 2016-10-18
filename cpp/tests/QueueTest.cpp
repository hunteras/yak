#include "../Queue.h"
#include "gtest/gtest.h"

#include <iostream>

TEST(Queue, QueueUsage)
{
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);

    while (!q.empty())
    {
        std::cout << q.front() << ":" << q.back() << std::endl;
        q.pop();
    }
    
}
