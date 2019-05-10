
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

        MemoryPool(uint32_t size = 100) : next_size_m(size), size_m(size), pool(size), free_list(size), index(size - 1), next_m(NULL) {
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

    

    template <typename T>
    class pool_allocator : public std::allocator<T> {
        static MemoryPool<T> pool;
    public:
        typedef size_t size_type;
        typedef T* pointer;
        typedef const T* const_pointer;

        template<typename _Tp1>
        struct rebind {
            typedef pool_allocator<_Tp1> other;
        };

        pointer allocate(size_type n, const void *hint = 0) {
            return (pointer)pool_allocator::pool.malloc();
        }

        void deallocate(pointer p, size_type n) {
            return pool_allocator::pool.free((pointer)p);
        }

        pool_allocator() throw () : std::allocator<T>() {
        }

        pool_allocator(const pool_allocator &a) throw () : std::allocator<T>(a) {
        }

        template <class U>
        pool_allocator(const pool_allocator<U> &a) throw () : std::allocator<T>(a) {
        }

        ~pool_allocator() throw () {
        }
    };

    template <class T>
    MemoryPool<T> pool_allocator<T>::pool;

}
#endif