/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Tape.hpp
 * Author: matthewsupernaw
 *
 * Created on November 2, 2016, 9:35 AM
 */

#ifndef TAPE_HPP
#define TAPE_HPP

#include <iostream>
#include "VariableInfo.hpp"
#include "CLFAllocator.hpp"
#include <set>
#include <vector>
#include <unordered_map>

namespace atl {

    template<typename REAL_T>
    struct StackEntry {
        VariableInfo<REAL_T>* w;
        std::set<VariableInfo<REAL_T>*> ids;
        typedef typename std::set<VariableInfo<REAL_T>* > vi_storage;
        typedef typename vi_storage::iterator vi_iterator;
        std::vector<REAL_T> first;
        std::vector<REAL_T> second;
        std::vector<REAL_T> third;

        StackEntry() {
        }

        StackEntry(const StackEntry<REAL_T>& other) :
        w(other.w), ids(other.ids), first(other.first), second(other.second), third(other.third) {
        }


    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const StackEntry<T>& entry) {
        out << "--------------------------------------------------------------\n";
        out << "w = " << entry.w->id << "\n";
        typename StackEntry<T>::vi_iterator it;

        out << "ids[ ";
        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
            out << (*it)->id << " ";
        }
        out << "]\n";

        out << "local gradient[ ";
        for (int i = 0; i < entry.first.size(); i++) {
            out << entry.first[i] << " ";
        }
        out << "]\n";
        out << "--------------------------------------------------------------\n";
        return out;
    }

    class SpinLock {
        std::atomic_flag locked = ATOMIC_FLAG_INIT;
    public:

        void lock() {
            while (locked.test_and_set(std::memory_order_acquire)) {
                ;
            }
        }

        void unlock() {
            locked.clear(std::memory_order_release);
        }
    };

    template<typename REAL_T>
    class Tape {
        SpinLock stack_lock;

        typedef typename std::unordered_map<uint32_t, REAL_T>::iterator first_order_iterator;
    public:
        std::unordered_map<uint32_t, REAL_T> first_order_derivtives;
        std::vector<StackEntry<REAL_T>, atl::clfallocator<StackEntry<REAL_T> > > stack;
        std::atomic<size_t> stack_current;

        Tape(size_t size = 100000) {
            stack.resize(size);
        }

        size_t GetBlock(size_t size) {
#ifdef ATL_THREAD_SAFE
            stack_lock.lock();
#endif
            size_t ret = this->stack_current;
            if (stack_current + size + 2 >= this->stack.size()) {
                std::cout << "Resizing tape entry vector to size " <<(this->stack.size() + size + 100)<<"...";
                this->stack.resize(this->stack.size() + size + 100);
                std::cout<<"done."<< std::endl;
            }
            this->stack_current += size;
#ifdef ATL_THREAD_SAFE
            stack_lock.unlock();
#endif     
            return ret;
        }

        /**
         * Atomic operation. Gets the next available index in the stack.
         *
         * @return
         */
        inline const size_t NextIndex() {
#ifdef ATL_THREAD_SAFE
            stack_lock.lock();
#endif
            if (stack_current + 2 >= this->stack.size()) {
                //                std::cout<<"Resizing Stack Entry vector."<<std::endl;
                this->stack.resize(this->stack.size() + 100);
            }
            size_t ret = this->stack_current++;
#ifdef ATL_THREAD_SAFE
            stack_lock.unlock();
#endif       
            return ret;
        }

        void Accumulate() {
            this->first_order_derivtives.clear();
            this->first_order_derivtives[this->stack[(stack_current - 1)].w->id] = static_cast<REAL_T> (1.0);
            REAL_T w = static_cast<REAL_T> (0.0);
            typename atl::StackEntry< REAL_T>::vi_iterator vit;
            size_t index;
            for (int i = (stack_current - 1); i >= 0; i--) {

                REAL_T& W = this->first_order_derivtives[this->stack[i].w->id];
                //                if (W != 0) {
                w = W;

                W = static_cast<REAL_T> (0.0);
                index = 0;
                for (vit = this->stack[i].ids.begin(); vit != this->stack[i].ids.end(); ++vit) {
                    this->first_order_derivtives[(*vit)->id] += w * this->stack[i].first[index];
                    index++;
                }

                //                }
            }
        }

    };


}


#endif /* TAPE_HPP */

