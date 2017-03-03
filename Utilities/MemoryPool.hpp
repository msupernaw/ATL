
#ifndef MEMORYPOOL_HPP
#define MEMORYPOOL_HPP
#include <vector>
#include <stdint.h>
#include <iostream>
#include <atomic>
#include <mutex>



namespace util {

    template<class T>
    class MemoryPool {
        std::vector<T> pool; //actual heap of objects
        std::vector<T* > free_list; //available objects
        std::atomic<size_t> index;
        std::mutex lock;
        size_t size_m, next_size_m;
        MemoryPool<T >* next_m;

        inline void* extend() {
#ifdef ATL_THREAD_SAFE
            lock.lock();
#endif
            if (next_m == NULL) {
                std::cout << "extending memory pool..." << std::endl;
                this->next_m = new MemoryPool<T> (this->next_size_m);
            }
#ifdef ATL_THREAD_SAFE
            lock.unlock();
#endif
            return next_m->malloc();
        }

    public:

        MemoryPool(uint32_t size) : next_size_m(size), size_m(size), pool(size), free_list(size), index(size - 1), next_m(NULL) {
            //            pool = new T[size];
            for (int i = 0; i < size_m; i++) {
                free_list[i] = (&pool[i]);
            }
        }

        ~MemoryPool() {
            free_list.clear();

            if (next_m != NULL) {
                delete next_m;
            }
            //            delete[] pool;
        }

        void SetResize(uint32_t resize) {
            this->next_size_m = resize;
        }

        inline void* malloc() {
            if (index == 0) {

                return this->extend();
            }
            return free_list[index--];
        }

        inline void free(void* ptr) {

            if (ptr < &pool[0] || ptr > &pool[size_m - 1]) {
                if (next_m != NULL) {
                    next_m->free(ptr);
                } else {
                    std::cout << "pointer not freed...\n";
                }

            } else {
                if (ptr)
                    free_list[++index] = (T*) ptr;
            }

        }

    };


}
#endif