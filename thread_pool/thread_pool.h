#ifndef THREAD_POOL
#define THREAD_POOL

#include <iostream>

#include <atomic>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <memory>
#include "concurrency_deque.h"
#include "concurrency_queue.h"
#include "threads_joiner.h"

namespace gutter
{
class thread_pool
{
private:

    using task_type = std::function<void()>;

    std::atomic_bool done;

    gutter::concurrency_queue<task_type> m_global_task_queue;
    
    std::vector<std::unique_ptr<gutter::concurrency_deque<task_type>>> m_local_task_queues;
    
    std::vector<std::thread> threads;
    
    gutter::threads_joiner joiner;

    unsigned int pool_capacity;
    
    static thread_local unsigned int m_index;

    static thread_local gutter::concurrency_deque<task_type> *local_task_queue;

    gutter::spinlock_mutex spinlock;

private:
    void worker_thread(unsigned __index);

    bool try_transfer(gutter::concurrency_deque<task_type> *tasksPtr, task_type& func);

    bool try_transfer_from_orthers(task_type&);

public:
    explicit thread_pool(unsigned int nums = std::thread::hardware_concurrency()) : done(false), joiner(threads), pool_capacity(nums) {
        try {
            for (unsigned int i =0; i < pool_capacity; ++i) {
                m_local_task_queues.emplace_back(
                    std::unique_ptr<gutter::concurrency_deque<task_type>> { new gutter::concurrency_deque<task_type>(spinlock) }
                    );
            }


            for (unsigned int i = 0; i < pool_capacity; ++i) {
                threads.emplace_back(std::thread(&thread_pool::worker_thread,this,i));
            }
        }catch(...) {
            done = true;
            throw;
        }

    }
    ~thread_pool() {
        done = true;
    }

public:
    template<typename Func, typename...Args>
    auto submit(Func&& f, Args&&... args) -> std::future<decltype(f(args...))>;

    void run_pending_task();

};
 
thread_local unsigned int thread_pool::m_index {0}; 
thread_local gutter::concurrency_deque<std::function<void()>> *thread_pool::local_task_queue {nullptr};

inline
void thread_pool::worker_thread(unsigned int __index) {
    m_index = __index;
    local_task_queue = m_local_task_queues[m_index].get();
    while(!done) {
        run_pending_task();
    }
}

//the submit function is a little tricky,the main problem is we need std::packaged_task 
//to obtain return values of the functions which passed to thread objects,but Unfortunately
//std::packaged_task object can not be copied, that's why we need a wrapper to pass our task_type
//to a concurrency_queue<std::function<void()>> object. another way is we design a container
//like std::function but with capability to deal objects can't be copied.

template<typename Func, typename...Args>
auto thread_pool::submit(Func&& f, Args&&... args) -> std::future<decltype(f(args...))> {

    std::function<decltype(f(args...))()> 
    func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);

    auto task_ptr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    std::function<void()> wrapper = [task_ptr]() {
        (*task_ptr)();
    };

    if (local_task_queue) {
        local_task_queue -> emplace_front(std::move(wrapper));
    } 
    else {
        m_global_task_queue.emplace(std::move(wrapper));
    }

    return task_ptr -> get_future();
}

inline
void thread_pool::run_pending_task() {
    task_type task;
    if ((local_task_queue && local_task_queue -> try_pop_front(task)) ||
         m_global_task_queue.try_pop(task)                            ||
         try_transfer_from_orthers(task)
    ) {
        task();
    } else {
        std::this_thread::yield();
    }
}

inline
bool thread_pool::try_transfer(gutter::concurrency_deque<task_type> *tasksPtr, task_type& func) {
    return tasksPtr -> try_pop_back(func);
}

inline
bool thread_pool::try_transfer_from_orthers(task_type& task) {
    for (unsigned int i = 0; i < m_local_task_queues.size(); ++i) {
        const unsigned index = (m_index + i + 1) % m_local_task_queues.size();
        if (try_transfer(m_local_task_queues[index].get(),task)) {
            return true;
        }
    }
    return false;
}

}
#endif