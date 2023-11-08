#ifndef THREADS_JOINER
#define THREADS_JOINER

#include <vector>
#include <thread>

namespace gutter
{
class threads_joiner
{
private:
    std::vector<std::thread>& threads;
public:
    threads_joiner(std::vector<std::thread>& __threads) : threads(__threads) {}
    threads_joiner() = delete;

    ~threads_joiner() {
        unsigned int size = threads.size();
        for (unsigned int i = 0; i < size; ++i) {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

};

}
#endif