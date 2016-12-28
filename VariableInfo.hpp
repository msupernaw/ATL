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

        VariableIdGenerator() : _id(1), available_size(0) {
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

    template<typename REAL_T>
    struct VariableInfo {
        uint32_t id;
        REAL_T value;
        
        VariableInfo(REAL_T value = static_cast<REAL_T>(0.0)) :
        id(VariableIdGenerator::instance()->next()), value(value) {
        }

        ~VariableInfo(){
            VariableIdGenerator::instance()->release(id);
        }
        
        
    };


}


#endif /* VARIABLEINFO_HPP */

