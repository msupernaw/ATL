/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ThreadPool.hpp
 * Author: matthewsupernaw
 *
 * Created on November 3, 2016, 1:58 PM
 */

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP


#include <thread>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <vector>
#include <future>
#include <cstdlib>
#include <functional>

#if defined(linux) || defined(__linux) || defined(__linux__)
#define NOAA_TP_LINUX
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
#define NOAA_TP_BSD
#elif defined(sun) || defined(__sun)
#define NOAA_TP_SOLARIS
#elif defined(__sgi)
#define NOAA_TP_IRIX
#elif defined(__hpux)
#define NOAA_TP_HPUX
#elif defined(__CYGWIN__)
#define NOAA_TP_CYGWIN
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#define NOAA_TP_WIN32
#elif defined(_WIN64) || defined(__WIN64__) || defined(WIN64)
#define NOAA_TP_WIN64
#elif defined(__BEOS__)
#define NOAA_TP_BEOS
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#define NOAA_TP_MACOS
#elif defined(__IBMCPP__) || defined(_AIX)
#define NOAA_TP_AIX
#elif defined(__amigaos__)
#define NOAA_TP_AMIGAOS
#elif defined(__QNXNTO__)
#define NOAA_TP_QNXNTO
#endif

#if defined(NOAA_TP_WIN32) || defined(NOAA_TP_WIN64)
#define NOAA_TP_WINDOWS
#endif

#if defined(NOAA_TP_WINDOWS)
#undef max
#undef min
#endif

namespace atl {

    class ThreadPool;
    struct WaitableFunction;

    /**
     * 
     * WaitVariable is used in conjunction with ThreadPool runs where it is 
     * desired to wait for threads to complete before continuing.
     * 
     */
    class WaitVariable {
        friend class ThreadPool;
        std::atomic<int> wait;

        inline std::atomic<int>& operator++(int i) {
            wait++;
            return wait;
        }

        inline std::atomic<int>& operator--(int i) {
            wait--;
            return wait;
        }
    public:

        WaitVariable() : wait(0) {
        }



    };

    /**
     * 
     *Thread pool.
     * 
     */
    class ThreadPool {

        template<class InputIt, typename Function>
        struct ForEachFunctor {
            InputIt start;
            InputIt end;
            Function f;

            ForEachFunctor(InputIt start, InputIt end, Function f) :
            start(start), end(end), f(f) {
            }

            void operator()(void) {
                for (InputIt it = start; it != end; it++) {
                    f((*it));
                }

            }

        };

        /**
         */
        struct WaitableFunction {
            std::function<void(void) > f;
            std::atomic<int>& wait;

            WaitableFunction(const WaitableFunction& other) :
            f(other.f), wait(other.wait) {
            }

            WaitableFunction(const std::function<void(void) >& f, std::atomic<int>& wait) :
            f(f), wait(wait) {
            }

            WaitableFunction(std::function<void(void) >&& f, std::atomic<int>&& wait) :
            f(f), wait(wait) {
            }

            void operator()(void) {
                f.operator()();
                wait--;
            }
        };


    public:

        /**
         * Constructor. Sets the number of threads in this thread pool.
         * 
         * @param threads
         */
        ThreadPool(int threads = std::thread::hardware_concurrency()) : shutdown_(false), started_(true) {
            // Create the specified number of threads
            threads_.reserve(threads);
            for (int i = 0; i < threads; ++i)
                threads_.emplace_back(std::bind(&ThreadPool::threadEntry, this, i));

            this->thread_count = threads;

#ifndef NOAA_TP_WINDOWS

            sched_param sch;
            sch.sched_priority = 70;
            for (int i = 0; i < threads_.size(); i++) {
                pthread_setschedparam(threads_[i].native_handle(), SCHED_FIFO, &sch);
            }
#else
            for (int i = 0; i < threads_.size(); i++) {
         //       SetThreadPriority(threads_[i].native_handle(), 50);
            }
#endif


        }

        /**
         * Destructor. Shuts down all threads.
         */
        ~ThreadPool() {
            {
                // Unblock any threads and tell them to stop
                std::unique_lock <std::mutex> l(lock_);

                shutdown_ = true;
                condVar_.notify_all();
            }

            // Wait for all threads to stop
            for (auto& thread : threads_)
                thread.join();
        }

        void Start(int threads = std::thread::hardware_concurrency()) {
            if (!started_) {
                threads_.reserve(threads);
                for (int i = 0; i < threads; ++i)
                    threads_.emplace_back(std::bind(&ThreadPool::threadEntry, this, i));

                started_ = true;
            }
        }

        /**
         * Returns the number of threads in this thread pool.
         * @return 
         */
        size_t Size() {
            return threads_.size();
        }

        /**
         * Do a job that doesn't require wait.
         * 
         * @param func
         */
        void DoJob(std::function <void (void) > func) {

            // Place a job on the queue and unblock a thread
            std::unique_lock <std::mutex> l(lock_);
            jobs_.emplace(std::move(func));
            condVar_.notify_one();
        }

        /**
         * Do a job that requires wait. The wait variable is incremented.
         * @param func
         * @param wait
         */
        void DoJob(std::function <void (void) > func, WaitVariable& wait) {

            wait.wait++;

            // Place a job on the queue and unblock a thread
            std::unique_lock <std::mutex> l(lock_);
            std::function<void(void) > f = WaitableFunction(func, wait.wait);
            jobs_.emplace(std::move(f));
            condVar_.notify_one();
        }

        /**
         * For each job.
         * 
         * @param first
         * @param elements
         * @param function
         */
        template<class InputIt, typename Function>
        void ForEach(InputIt first, const size_t& elements, const Function &function) {
            if (!elements)return;



            size_t max_threads = std::max(1ul, (unsigned long int)std::min((elements), this->thread_count));

            size_t inc = elements / max_threads;
            auto siter = first;
            size_t end;
            size_t start;

            for (int t = 0; t < max_threads; t++) {
                start = inc*t;
                siter = first + start;
                t == (max_threads - 1) ? end = elements : end = inc * (t + 1);
                auto eiter = first + end;
                std::function<void(void) > func = ForEachFunctor<InputIt, Function>(siter, eiter, std::move(function)); //std::bind(std::for_each<InputIt, Function>, siter, eiter, std::move(function)); 
                std::unique_lock <std::mutex> l(lock_);

                jobs_.emplace(
                        std::move(func)
                        );

                condVar_.notify_one();
            }
        }

        /**
         * For each job with wait.
         * 
         * @param first
         * @param elements
         * @param function
         * @param wait
         */
        template<class InputIt, typename Function>
        void ForEach(InputIt first, const size_t& elements, const Function &function, WaitVariable& wait) {
            if (!elements)return;



            size_t max_threads = std::max(1ul, (unsigned long int)std::min((elements), this->thread_count));
            wait.wait += max_threads;
            size_t inc = elements / max_threads;
            auto siter = first;
            size_t end;
            size_t start;

            for (int t = 0; t < max_threads; t++) {
                start = inc*t;
                siter = first + start;
                t == (max_threads - 1) ? end = elements : end = inc * (t + 1);
                auto eiter = first + end;
                std::function<void(void) > func = ForEachFunctor<InputIt, Function>(siter, eiter, (function));
                std::function<void(void) > f = WaitableFunction(std::move(func), std::move(wait.wait));
                std::unique_lock <std::mutex> l(lock_);

                jobs_.emplace(
                        std::move(f)
                        );

                condVar_.notify_one();
            }
        }

        /**
         * Wait for this wait variable to signal completion.
         * 
         * @param wait
         */
        void Wait(WaitVariable& wait) {
            while (wait.wait > 0) {
            }
        }



    protected:

        void threadEntry(int i) {
            std::function <void (void) > job_;

            while (1) {
                {
                    std::unique_lock <std::mutex> l(lock_);

                    while (!shutdown_ && jobs_.empty())
                        condVar_.wait(l);

                    if (jobs_.empty()) {
                        // No jobs to do and we are shutting down
                        return;
                    }

                    job_ = std::move(jobs_.front());
                    jobs_.pop();
                }

                // Do the job without holding any locks
                std::function <void (void) > task = job_;

                task();

            }

        }


        std::mutex lock_;
        std::condition_variable condVar_;
        bool shutdown_;
        bool started_;
        size_t thread_count;
        std::queue <std::function <void (void) > > jobs_;
        std::vector <std::thread> threads_;
    };

    //global thread pool
    atl::ThreadPool thread_pool_g;

}




#endif /* THREADPOOL_HPP */

