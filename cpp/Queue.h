#ifndef QUEUE_H
#define QUEUE_H

#include <queue>

template<typename T>
class BaseQueue
{
public:

    virtual void push(const T &val) = 0;
    virtual void pop() = 0;
    virtual const T& front() = 0;
    virtual const T& back() = 0;
    
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
};

template<typename T>
class Queue : public BaseQueue<T>
{
public:
    Queue()
        : mQueue(std::queue<T>()) {}
    ~Queue() {};

    void push(const T &val) { mQueue.push(val); };
    void pop() { mQueue.pop(); };
    const T& front() { return mQueue.front(); };
    const T& back() { return mQueue.back(); };

    bool empty() const { return mQueue.empty(); };
    size_t size() const { return mQueue.size(); };
private:
    std::queue<T> mQueue;
};



#endif
