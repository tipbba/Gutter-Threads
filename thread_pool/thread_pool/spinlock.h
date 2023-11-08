#ifndef SPIN_LOCK
#define SPIN_LOCK

#include <atomic>
namespace gutter
{
class spinlock_mutex 
{
private:
    std::atomic_flag flag;

public:
    spinlock_mutex() : flag(ATOMIC_FLAG_INIT) {}

    void lock();

    void unlock();
};

inline void spinlock_mutex::lock() {
    while (flag.test_and_set(std::memory_order_acquire)); //acquire 对应test
}

inline void spinlock_mutex::unlock() {
    flag.clear(std::memory_order_release);
}

}
#endif
