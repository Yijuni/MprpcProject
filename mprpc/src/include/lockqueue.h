#pragma once
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
//日志队列，异步写日志

template<typename T>
class LockQueue
{
public: 
    void push(const T &data);
    T pop();
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvar;
};

//多个工作线程写日志到queue
template <typename T>
inline void LockQueue<T>::push(const T &data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_queue.push(data);
    m_condvar.notify_one();
}

//一个线程从queue读日志，然后写入文件保存
template <typename T>
inline T LockQueue<T>::pop()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    while(m_queue.empty()){
        //日志队列为空，线程进入wait状态
        m_condvar.wait(lock);
    }
    T data = m_queue.front();
    m_queue.pop();
    return data;
}
