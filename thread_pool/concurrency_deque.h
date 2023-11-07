#ifndef CONCURRENCY_deque
#define CONCURRENCY_deque

#include <deque>
#include <utility>
#include "spinlock.h"

namespace gutter
{
template<typename T>
class concurrency_deque
{
private:
    std::deque<T> m_deque;
    spinlock_mutex spinlock;
public:
    concurrency_deque() = default;
    concurrency_deque(std::deque<T>& __que) : m_deque(__que) {};
    ~concurrency_deque() {}

    auto front();
    
    auto back();
    
    bool empty();

    void pop_front();
    
    void pop_back();

    bool try_pop_front(T& value);
    
    bool try_pop_back(T& value);

    void push_front(T&&);
    
    void push_back(T&&);

    void emplace_front(T&&);

    void emplace_back(T&&);
        
    auto size();

};

template<typename T>
inline auto concurrency_deque<T>::front() {
    spinlock.lock();
    return m_deque.front();
    spinlock.unlock();
}

template<typename T>
inline auto concurrency_deque<T>::back() {
    spinlock.lock();
    return m_deque.back();
    spinlock.unlock();
}

template<typename T>
inline bool concurrency_deque<T>::empty() {
    spinlock.lock();
    return empty();
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::pop_front() {
    spinlock.lock();
    m_deque.pop_front();
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::pop_back() {
    spinlock.lock();
    m_deque.pop_back();
    spinlock.unlock();
}

template<typename T>
inline bool concurrency_deque<T>::try_pop_front(T& value) {
    spinlock.lock();
    if(m_deque.empty()) 
        return false;
    value = std::move(m_deque.front());
    m_deque.pop_front();
    return true;
    spinlock.unlock();
}

template<typename T>
inline bool concurrency_deque<T>::try_pop_back(T& value) {
    spinlock.lock();
    if(m_deque.empty()) 
        return false;
    value = std::move(m_deque.back());
    m_deque.pop_back();
    return true;
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::push_front(T&& value) {
    spinlock.lock();
    m_deque.push_front(value);
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::push_back(T&& value) {
    spinlock.lock();
    m_deque.push_back(value);
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::emplace_front(T&& value) {
    spinlock.lock();
    m_deque.emplace_front(value);
    spinlock.unlock();
}

template<typename T>
inline void concurrency_deque<T>::emplace_back(T&& value) {
    spinlock.lock();
    m_deque.emplace_back(value);
    spinlock.unlock();
}

template<typename T>
inline auto concurrency_deque<T>::size() {
    spinlock.lock();
    m_deque.size();
    spinlock.unlock();
}

}
#endif
