/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VariableInfo.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 9:55 AM
 */

#ifndef VARIABLEINFO_HPP
#define VARIABLEINFO_HPP
#include <stack>
#include <mutex>

namespace atl {

    /*!
     * Creates a unique identifier for variables. Identifiers are recyclable.
     * @return 
     */
    class VariableIdGenerator {
        std::stack<uint32_t> available;
        std::atomic<uint32_t> available_size;
        static std::mutex mutex_g;

        class SpinLock {
            std::atomic_flag locked = ATOMIC_FLAG_INIT;
        public:

            void lock() {
                while (locked.test_and_set(std::memory_order_acquire)) {

                }
            }

            void unlock() {
                locked.clear(std::memory_order_release);
            }
        };

        SpinLock lock;

    public:
        static std::shared_ptr<VariableIdGenerator> instance();

        const uint32_t next() {
#ifdef ATL_THREAD_SAFE
            lock.lock();
#endif
            uint32_t ret;
            if (!available.empty() > 0) {
                ret = available.top();
                available.pop();

            } else {
                ret = ++_id;
            }


#ifdef ATL_THREAD_SAFE
            lock.unlock();
#endif
            return ret; //(++_id);
        }

        void release(const uint32_t& id) {
#ifdef ATL_THREAD_SAFE
            lock.lock();
#endif
            available.push(id);
            available_size++;
#ifdef ATL_THREAD_SAFE
            lock.unlock();
#endif
        }

        const uint32_t current() {
            return _id;
        }

        //    private:

        VariableIdGenerator() :  available_size(0),_id(1) {
        }

        std::atomic<uint32_t> _id;
    };




    std::mutex VariableIdGenerator::mutex_g;
    static std::shared_ptr<VariableIdGenerator> only_copy;

    inline std::shared_ptr<VariableIdGenerator>
    VariableIdGenerator::instance() {

        if (!only_copy) {
            only_copy = std::make_shared<VariableIdGenerator>();
        }

        return only_copy;
    }

    
    class VISpinLock {
            std::atomic_flag locked = ATOMIC_FLAG_INIT;
        public:

            void lock() {
                while (locked.test_and_set(std::memory_order_acquire)) {

                }
            }

            void unlock() {
                locked.clear(std::memory_order_release);
            }
        };
    
    /**
     * Holds unique info for \ref Variable objects.
     */
    template<typename REAL_T>
    struct VariableInfo {

        
        static VISpinLock vinfo_mutex_g;
        static std::vector<VariableInfo<REAL_T>* > freed;
        std::atomic<int> count;
        uint32_t id;
        REAL_T value;
        bool is_nl;

        /**
         * Constructor 
         * 
         * @param value
         */
        VariableInfo(REAL_T value = static_cast<REAL_T> (0.0)) :
        id(VariableIdGenerator::instance()->next()), count(1) , value(value){
        }

        /**
         * Destructor. 
         */
        ~VariableInfo() {
//            if(VariableIdGenerator::instance(). != 0)
//            VariableIdGenerator::instance()->release(id);
        }

        inline void Aquire() {
            count++;
        }

        inline void Release() {
            count--;

            if ((count) == 0) {
                //store this pointer in the freed list and delete when the gradient 
                //structure resets.
#ifdef ATL_THREAD_SAFE
                VariableInfo<REAL_T>::vinfo_mutex_g.lock();
                freed.push_back(this);
                VariableInfo<REAL_T>::vinfo_mutex_g.unlock();
#else
                freed.push_back(this);
#endif

            }
        }
        
          static void FreeAll() {

//            VariableInfo<REAL_T>::vinfo_mutex_g.lock();
//#pragma unroll
//            for (int i = 0; i < freed.size(); i++) {
//                if (freed[i] != NULL) {//memory pool may have destructed first
//                    VariableIdGenerator::instance()->release(freed[i]->id);
//                    freed[i]->value = 0;
//                    freed[i]->is_nl = false;
////                    freed[i]->id= 0;
//                    delete freed[i];
//                }
//            }
//            freed.resize(0);
//            VariableInfo<REAL_T>::vinfo_mutex_g.unlock();
        }


    };
    
     template<typename REAL_T>
    VISpinLock VariableInfo<REAL_T>::vinfo_mutex_g;

    template<typename REAL_T>
    std::vector<VariableInfo<REAL_T>* > VariableInfo<REAL_T>::freed;

}


#endif /* VARIABLEINFO_HPP */

