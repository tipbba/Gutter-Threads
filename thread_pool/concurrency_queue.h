#ifndef CONCURRENCY_QUEUE
#define CONCURRENCY_QUEUE

#include <queue>
#include "spinlock.h"

namespace gutter
{
    
template<typename T>
class concurrency_queue
{
private:
    std::queue<T> m_queue;
    spinlock_mutex spinlock;
public:
    concurrency_queue() = default;
    concurrency_queue(std::queue<T>& __que) : m_queue(__que) {};
    ~concurrency_queue() {}

    auto front();
    
    auto back();
    
    bool empty();

    void pop();

    bool try_pop(T& value);

    void push(T&&);

    void emplace(T&&);

    auto size();

};

template<typename T>
inline auto concurrency_queue<T>::front() {
    spinlock.lock();
    return m_queue.front();
    spinlock.unlock();
}

template<typename T>
inline auto concurrency_queue<T>::back() {
    spinlock.lock();
    return m_queue.back();
    spinlock.unlock();
}

template<typename T>
inline bool concurrency_queue<T>::empty() {
    spinlock.lock();
    return empty();
    spinlock.unlock();
}

template<typename T>
inline void concurrency_queue<T>::pop() {
    spinlock.lock();
    m_queue.pop();
    spinlock.unlock();
}

template<typename T>
inline bool concurrency_queue<T>::try_pop(T& value) {
    spinlock.lock();
    if(m_queue.empty()) {
        spinlock.unlock();
        return false;
    }
    value = m_queue.front();
    m_queue.pop();
    spinlock.unlock();
    return true;
}

template<typename T>
inline void concurrency_queue<T>::push(T&& value) {
    spinlock.lock();
    m_queue.push(value);
    spinlock.unlock();
}

template<typename T>
inline void concurrency_queue<T>::emplace(T&& value) {
    spinlock.lock();
    m_queue.emplace(value);
    spinlock.unlock();
}

template<typename T>
inline auto concurrency_queue<T>::size() {
    spinlock.lock();
    m_queue.size();
    spinlock.unlock();
}

}
#endif
