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
#define ATL_THREAD_SAFE

#include "Platform.hpp"

#include <iostream>
#include "VariableInfo.hpp"
#include "CLFAllocator.hpp"
#include <set>
#include <vector>
#include <unordered_map>
#include <set>
#include <cfloat>
#include <cmath>


namespace atl {

    enum DerivativeTraceLevel {
        FIRST_ORDER_REVERSE = 0,
        SECOND_ORDER_REVERSE,
        THIRD_ORDER_REVERSE,
        FIRST_ORDER_FORWARD,
        SECOND_ORDER_FORWARD,
        THIRD_ORDER_FORWARD
    };

    template<typename REAL_T>
    struct less_variable_info {

        bool operator()(const std::shared_ptr<VariableInfo<REAL_T> >& lhs, const std::shared_ptr<VariableInfo<REAL_T> >& rhs) const {
            return lhs->id < rhs->id;
        }
    };

    template<typename REAL_T>
    struct StackEntry {
        typedef typename std::shared_ptr<VariableInfo<REAL_T> > VariableInfoPtr;
        VariableInfoPtr w;

        std::string exp;
        typedef std::set<VariableInfoPtr, atl::less_variable_info<REAL_T> > vi_storage;
        typedef typename vi_storage::iterator vi_iterator;

        bool is_nl = false;
        vi_storage ids;
        vi_storage nl_ids;
        vi_storage pushed_ids;
        std::vector<VariableInfoPtr > id_list;
        std::vector<REAL_T> first;
        std::vector<REAL_T> second;
        std::vector<REAL_T> third;

        StackEntry() {
        }

        StackEntry(const StackEntry<REAL_T>& other) :
        w(other.w), exp(other.exp), is_nl(other.is_nl), ids(other.ids), nl_ids(other.nl_ids), pushed_ids(other.pushed_ids), id_list(other.id_list), first(other.first), second(other.second), third(other.third) {
        }

        /**
         * Push an id into this entry. Used for higher-order
         * derivatives.
         * 
         * @param id
         */
        inline void Push(const VariableInfoPtr& id) {
            vi_iterator it = ids.find(id);
            if (it == ids.end()) {
                pushed_ids.insert(id);
            }
        }

        /**
         * Prepares this entry for evaluation.
         */
        inline void Prepare() {


            vi_iterator it;
            for (it = this->ids.begin(); it != ids.end(); ++it) {
                id_list.push_back((*it));
            }

            for (it = this->pushed_ids.begin(); it != pushed_ids.end(); ++it) {
                id_list.push_back((*it));
            }

        }

        /**
         * Reset this entry.
         */
        void Reset() {
            exp = "";
            w.reset(); // = NULL;
            is_nl = false;
            first.resize(0);
            second.resize(0);
            third.resize(0);
            ids.clear();
            pushed_ids.clear();
            id_list.clear();
            nl_ids.clear();
        }





    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const StackEntry<T>& entry) {
        out << "--------------------------------------------------------------\n";
        out << entry.exp << "\n";
        out << "w = " << entry.w->id << "\n";
        out << " value = " << entry.w->value << "\n";
        typename StackEntry<T>::vi_iterator it;

        out << "ids[ ";
        for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
            out << (*it)->id << " ";
        }
        out << "]\n";

        out << "pushed_ids[ ";
        for (it = entry.pushed_ids.begin(); it != entry.pushed_ids.end(); ++it) {
            out << (*it)->id << " ";
        }
        out << "]\n";


        out << "local gradient[ ";
        for (int i = 0; i < entry.first.size(); i++) {
            out << entry.first[i] << " ";
        }
        out << "]\n";

        if (entry.second.size() == entry.first.size() * entry.first.size()) {
            out << "local hessian[ \n";
            for (int i = 0; i < entry.first.size(); i++) {
                out << "[";
                for (int j = 0; j < entry.first.size(); j++) {
                    out << entry.second[i * entry.first.size() + j] << " ";
                }
                out << "]\n";
            }
            out << "]\n";
        }

        if (entry.third.size() == entry.first.size() * entry.first.size() * entry.first.size()) {
            out << "local jerk[ \n";
            for (int i = 0; i < entry.first.size(); i++) {
                out << "[";
                for (int j = 0; j < entry.first.size(); j++) {
                    out << "[";
                    for (int k = 0; k < entry.first.size(); k++) {
                        out << entry.third[i * entry.first.size() * entry.first.size() + j * entry.first.size() + k] << " ";
                    }
                    out << "] ";
                }
                out << "]\n";
            }
            out << "]\n";
        }

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
    struct ForwardModeDerivativeInfo {
        uint32_t id;
        std::set<uint32_t> ids;
        typedef typename std::unordered_map<uint32_t, REAL_T > first_order_container;
        typedef typename std::unordered_map<uint32_t, first_order_container > second_order_container;
        typedef typename std::unordered_map<uint32_t, second_order_container > third_order_container;
        first_order_container first_order_derivtives;
        second_order_container second_order_derivtives;
        third_order_container third_order_derivatives;

        void Reset() {
            first_order_derivtives.clear();
            second_order_derivtives.clear();
            third_order_derivatives.clear();
        }
    };

    /**
     * Tape class for recording adjoint derivative information that will be used in 
     * reverse mode accumulation upt to third-order.
     */
    template<typename REAL_T>
    class Tape {
        SpinLock stack_lock;

    public:
        typedef typename std::shared_ptr<VariableInfo<REAL_T> > VariableInfoPtr;
        //first-order storage
        typedef std::unordered_map<uint32_t,
        REAL_T,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, REAL_T> > > first_order_container;

        first_order_container first_order_derivatives;

        typedef typename first_order_container::iterator first_order_iterator;

        //second-order storage
        typedef std::unordered_map<uint32_t,
        first_order_container,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, first_order_container> > > second_order_container;
        second_order_container second_order_derivatives;
        typedef typename second_order_container::iterator second_order_iterator;

        //third-order storage
        typedef std::unordered_map<uint32_t,
        second_order_container,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, second_order_container> > > third_order_container;
        third_order_container third_order_derivatives;
        typedef typename third_order_container::iterator third_order_iterator;



        bool recording = true;
        DerivativeTraceLevel derivative_trace_level = FIRST_ORDER_REVERSE;
        //forward mode derivative info
        std::unordered_map<uint32_t, ForwardModeDerivativeInfo<REAL_T> > forward_mode_derivative_info;


        std::vector<StackEntry<REAL_T>, atl::clfallocator<StackEntry<REAL_T> > > stack;
        std::atomic<size_t> stack_current;

        /**
         * Constructor.
         * 
         * Reserves memory for the tape to \a size.
         * 
         * @param size
         */
        Tape(size_t size = 10000000) {
            stack.reserve(size);
            //            std::cout << "Capacity = " << stack.capacity() << "\n";
        }

        ~Tape() {
            std::cout << "Destructing tape " << std::endl;
        }

        void SetRecording(bool record) {
            this->recording = record;
        }

        /**
         * Atomic operation.
         * 
         * Allocates a block of tape entries. Adds \a size to the \a stack_current 
         * member. Useful for concurrent container operations.
         * 
         * @param size
         * @return 
         */
        size_t GetBlock(size_t size) {
#ifdef ATL_THREAD_SAFE
            stack_lock.lock();
#endif
            size_t ret = this->stack_current;
            if (stack_current + size + 2 >= this->stack.size()) {
                std::cout << "Resizing tape entry vector to size " << (this->stack.size() + size * 2) << "...";
                this->stack.resize((this->stack.size() + size) *2);
                std::cout << "done." << std::endl;
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
                std::cout << "Resizing Stack Entry vector at size " << stack.size() << "." << std::endl;
                this->stack.resize((this->stack.size() + 1) *2);
            }
            size_t ret = this->stack_current++;
#ifdef ATL_THREAD_SAFE
            stack_lock.unlock();
#endif       
            return ret;
        }

        inline StackEntry<REAL_T>& NextEntry() {
            return this->stack[this->NextIndex()];
        }

        inline REAL_T& Reference(uint32_t i) {
            return this->first_order_derivatives[i];
        }

        inline REAL_T& Reference(uint32_t i, uint32_t j) {

            if (j < i) {
                std::swap(i, j);
            }
            return this->second_order_derivatives[i][j];
        }

        inline REAL_T& Reference(uint32_t i, uint32_t j, uint32_t k) {
            if (i < j) {
                if (k < i) std::swap(i, k);
            } else {
                if (j < k) std::swap(i, j);
                else std::swap(i, k);
            }
            if (k < j) std::swap(j, k);

            return third_order_derivatives[i][j][k];
        }

        inline const REAL_T Value(uint32_t i) {
            first_order_iterator it = this->first_order_derivatives.find(i);
            if (it != this->first_order_derivatives.end()) {
                return (*it).second;
            } else {
                return static_cast<REAL_T> (0.0);
            }
        }

        inline const REAL_T Value(uint32_t i, uint32_t j) {
            if (j < i) {
                std::swap(i, j);
            }

            second_order_iterator it = this->second_order_derivatives.find(i);
            if ((it != this->second_order_derivatives.end())) {
                first_order_iterator jt = (*it).second.find(j);
                if (jt != (*it).second.end()) {
                    return (*jt).second;
                } else {
                    return static_cast<REAL_T> (0.0);
                }

            }
            return static_cast<REAL_T> (0.0);

        }

        inline const REAL_T Value(uint32_t i, uint32_t j, uint32_t k) {

            if (i < j) {
                if (k < i) std::swap(i, k);
            } else {
                if (j < k) std::swap(i, j);
                else std::swap(i, k);
            }

            if (k < j) std::swap(j, k);

            third_order_iterator it = this->third_order_derivatives.find(i);
            if (unlikely(it != this->third_order_derivatives.end())) {
                second_order_iterator jt = (*it).second.find(j);
                if (unlikely(jt != (*it).second.end())) {
                    first_order_iterator kt = (*jt).second.find(k);
                    if (kt != (*jt).second.end()) {
                        return (*kt).second;
                    } else {
                        return static_cast<REAL_T> (0.0);
                    }
                } else {
                    return static_cast<REAL_T> (0.0);
                }
            }

            return static_cast<REAL_T> (0.0);

        }

        /**
         * Accumulates reverse mode derivatives according 
         * to \a derivative_trace_level.
         * 
         */
        void Accumulate() {

        }

        void AccumulateFirstOrder() {
            if (recording) {
                this->first_order_derivatives.clear();
                this->first_order_derivatives[this->stack[(stack_current - 1)].w->id] = static_cast<REAL_T> (1.0);
                REAL_T w = static_cast<REAL_T> (0.0);
                typename atl::StackEntry< REAL_T>::vi_iterator vit;
                size_t index;
                for (int i = (stack_current - 1); i >= 0; i--) {

                    REAL_T& W = this->first_order_derivatives[this->stack[i].w->id];
                    if (W != static_cast<REAL_T> (0.0)) {
                        w = W;

                        W = static_cast<REAL_T> (0.0);
                        index = 0;
                        for (vit = this->stack[i].ids.begin(); vit != this->stack[i].ids.end(); ++vit) {
                            this->first_order_derivatives[(*vit)->id] += w * this->stack[i].first[index];
                            index++;
                        }

                    }
                }
            }
        }

        void AccumulateSecondOrder() {
            if (recording) {
                this->first_order_derivatives.clear();
                this->second_order_derivatives.clear();

                REAL_T w;

                typename atl::StackEntry< REAL_T>::vi_iterator vit;
                //initialize w
                this->Reference(this->stack[stack_current - 1].w->id) = static_cast<REAL_T> (1.0);


                unsigned rows = 0; //the size of the local derivatives, anything higher was pushed from previous calculation

                std::vector<REAL_T> vij; //holds current second order derivative for i wrt j



                REAL_T hii = 0.0;
                REAL_T hij = 0.0;
                //                REAL_T hjk = 0;
                REAL_T dj = 0;
                REAL_T dk = 0;

                VariableInfoPtr vi;
                VariableInfoPtr vj;
                VariableInfoPtr vk;
                for (int i = (stack_current - 1); i >= 0; i--) {

                    atl::StackEntry<REAL_T>& current_entry = this->stack[i];

                    vi = stack[i].w; //variable info for i
                    REAL_T& W = this->first_order_derivatives[this->stack[i].w->id];
                    w = W;

                    W = static_cast<REAL_T> (0.0);
                    int index = 0;
                    for (vit = current_entry.ids.begin(); vit != current_entry.ids.end(); ++vit) {
                        this->first_order_derivatives[(*vit)->id] += w * current_entry.first[index];
                        index++;
                    }


                    rows = current_entry.first.size();

                    //get h[i][i]
                    hii = 0.0;

                    hii = this->Value(vi->id, vi->id);
                    if (hii != 0) {
                        this->Reference(vi->id, vi->id) = 0.0;
                    }

                    //                    if (hii != hii) {
                    //                        std::cout << "i " << i << " of " << stack_current << "\n NaN" << std::endl;
                    //                        exit(0);
                    //
                    //                    }

                    current_entry.Prepare();

                    size_t ID_LIST_SIZE = current_entry.id_list.size();
                    vij.resize(ID_LIST_SIZE);

                    std::vector<bool> gh(ID_LIST_SIZE*ID_LIST_SIZE, false);

                    for (unsigned j = 0; j < ID_LIST_SIZE; j++) {
                        vj = current_entry.id_list[j];

                        //load second order partial derivative for i wrt j and k
                        hij = 0.0;

                        hij = this->Value(vi->id, vj->id);

                        if (hij != 0) {
                            this->Reference(vi->id, vj->id) = 0.0;
                        }

                        vij[j] = (hij);
                    }

                    for (int j = 0; j < rows; j++) {
                        vj = current_entry.id_list[j];
                        dj = current_entry.first[j];
                        REAL_T hij = vij[j]; //h[i][j]
                        REAL_T entry = static_cast<REAL_T> (0.0);

                        for (int k = j; k < rows; k++) {

                            gh[j * ID_LIST_SIZE + k] = false;
                            gh[k * ID_LIST_SIZE + j] = false;
                            vk = current_entry.id_list[k];

                            entry = 0.0; //the entry value for h[j][k]

                            dk = current_entry.first[k];

                            entry += vij[k] * dj + (hij * dk) + hii * dj*dk;


                            entry += w * current_entry.second[j * rows + k];




                            if (FP_ZERO != std::fpclassify(entry)) {//h[j][k] needs to be updated
                                this->Reference(vj->id, vk->id) += entry;
                                gh[j * ID_LIST_SIZE + k] = true;
                                gh[k * ID_LIST_SIZE + j] = true;
                            }

                        }
                        for (int k = rows; k < ID_LIST_SIZE; k++) {


                            gh[j * ID_LIST_SIZE + k] = false;
                            gh[k * ID_LIST_SIZE + j] = false;
                            vk = current_entry.id_list[k];

                            entry = static_cast<REAL_T> (0.0); //the entry value for h[j][k]

                            dk = 0;





                            entry += vij[k] * dj; // + (hij * dk) + hii * dj*dk;


                            if (j < rows && k < rows) {
                                entry += w * current_entry.second[j * rows + k];
                            }


                            if (FP_ZERO != std::fpclassify(entry)) {//h[j][k] needs to be updated
                                this->Reference(vj->id, vk->id) += entry;
                                gh[j * ID_LIST_SIZE + k] = true;
                                gh[k * ID_LIST_SIZE + j] = true;
                            }

                        }
                    }

                    if (i > 0) {
                        if (current_entry.is_nl) {
                            for (int ii = 0; ii < ID_LIST_SIZE; ii++) {
                                bool p = false;
                                for (int jj = ii; jj < ID_LIST_SIZE; jj++) {
                                    if (std::fpclassify(vij[jj]) != gh[ii * ID_LIST_SIZE + jj]) {
                                        stack[i - 1].Push(current_entry.id_list[jj]);
                                    }
                                }
                            }
                        }
                    }

                }

            }

        }

        void AccumulateThirdOrder() {

            if (recording) {
                this->first_order_derivatives.clear();
                this->second_order_derivatives.clear();
                this->third_order_derivatives.clear();

                REAL_T w;

                //initialize w
                this->Reference(stack[stack_current - 1].w->id) = static_cast<REAL_T> (1.0);
                unsigned rows = 0; //the size of the local derivatives, anything higher was pushed from previous calculation

                std::vector<REAL_T> vij; //holds current second order derivative for i wrt j
                std::vector<REAL_T> viij_;
                std::vector<REAL_T> vijk_;


                REAL_T hii = 0.0;
                REAL_T diii = 0.0;
                //                REAL_T dijk = 0.0;
                //                REAL_T diil = 0.0;
                REAL_T dj = 0.0;
                REAL_T dk = 0.0;
                REAL_T dl = 0.0;
                REAL_T pjl = 0.0;
                REAL_T pkl = 0.0;
                REAL_T entry_3 = 0;
                REAL_T d3 = 0.0;
                REAL_T hij = 0.0;
                REAL_T pjk = 0.0;


                for (int i = (stack_current - 1); i >= 0; i--) {
                    atl::StackEntry<REAL_T>& current_entry = this->stack[i];
                    VariableInfoPtr vi = current_entry.w; //variable info for i
                    REAL_T& W = this->Reference(vi->id);
                    w = W;
                    W = 0.0;

                    rows = current_entry.first.size();

                    hii = 0.0;

                    //get h[i][i]
                    hii = Value(vi->id, vi->id);

                    if (hii != 0.0) {
                        Reference(vi->id, vi->id) = 0.0;
                    }


                    diii = 0.0;
                    diii = Value(vi->id, vi->id, vi->id);

                    if (diii != 0.0) {
                        Reference(vi->id, vi->id, vi->id) = 0.0;
                    }


                    //prepare for the third-order calculation. 
                    //builds a list of variables to use, statement level variables come first,
                    current_entry.Prepare();

                    size_t ID_LIST_SIZE = current_entry.id_list.size();
                    std::vector<bool> gh(ID_LIST_SIZE*ID_LIST_SIZE, false);
                    std::vector<bool> ghh(ID_LIST_SIZE * ID_LIST_SIZE*ID_LIST_SIZE, false);
                    vij.resize(ID_LIST_SIZE);
                    viij_.resize(ID_LIST_SIZE);
                    vijk_.resize(ID_LIST_SIZE * ID_LIST_SIZE);


                    //compute gradient
                    if (w != REAL_T(0.0)) {
                        for (unsigned j = 0; j < rows; j++) {
                            this->Reference(current_entry.id_list[j]->id) += w * current_entry.first[j];
                        }
                    }
                    VariableInfoPtr vk;
                    VariableInfoPtr vj;
                    VariableInfoPtr vl;
                    //prepare higher order stuff

                    for (unsigned j = 0; j < ID_LIST_SIZE; j++) {

                        vj = current_entry.id_list[j];

                        //load second order partial derivative for i wrt j and k
                        vij[j] = 0.0;
                        vij[j] = Value(vi->id, vj->id);

                        if (std::fabs(vij[j]) > 0.0) {
                            Reference(vi->id, vj->id) = static_cast<REAL_T> (0.0);
                        }

                        viij_[j] = 0.0;
                        viij_[j] = Value(vi->id, vi->id, vj->id);

                        if (std::fabs(viij_[j]) > 0.0) {
                            Reference(vi->id, vi->id, vj->id) = static_cast<REAL_T> (0.0);
                        }


#pragma unroll
                        for (unsigned k = j; k < ID_LIST_SIZE; k++) {
                            vk = current_entry.id_list[k];



                            vijk_[(j * ID_LIST_SIZE) + k] = 0.0;
                            vijk_[(j * ID_LIST_SIZE) + k] = Value(vi->id, vj->id, vk->id);

                            if (vijk_[(j * ID_LIST_SIZE) + k] != 0.0) {
                                Reference(vi->id, vj->id, vk->id) = static_cast<REAL_T> (0.0);
                            }

                            vijk_[(k * ID_LIST_SIZE) + j] = vijk_[(j * ID_LIST_SIZE) + k]; // dijk;
                        }
                    }



                    REAL_T entry;
                    REAL_T hdk;
                    REAL_T hdj;

                    for (int j = 0; j < rows; j++) {
                        vj = current_entry.id_list[j];
                        dj = current_entry.first[j];

                        if (j == 0) {
#pragma unroll
                            for (int k = 0; k < rows; k++) {
                                hdj = 0;
                                hdj = current_entry.first[k];
                                VariableInfoPtr vk = current_entry.id_list[k];
#pragma unroll

                                for (int l = k; l < rows; l++) {

                                    vl = current_entry.id_list[l];
                                    hdk = 0;

                                    entry = 0.0; //the entry value for h[j][k]

                                    hdk = current_entry.first[l];


                                    entry += vij[l] * hdj + (vij[k] * hdk) + hii * hdj*hdk;


                                    entry += w * current_entry.second[k * rows + l];


                                    if (entry != entry) {
                                        std::cout << "Derivative signaling NaN\n";
                                    }
                                    if (/*std::fabs(entry)*/entry != REAL_T(0.0)) {//h[j][k] needs to be updated
                                        this->Reference(vk->id, vl->id) += entry;
                                        gh[l * ID_LIST_SIZE + k] = true;
                                        gh[k * ID_LIST_SIZE + l] = true;

                                    }
                                }

                                for (int l = rows; l < ID_LIST_SIZE; l++) {

                                    vl = current_entry.id_list[l];

                                    hdk = 0;

                                    entry = 0.0; //the entry value for h[j][k]

                                    entry += vij[l] * hdj; // + (vij[k] * hdk) + hii * hdj*hdk;

                                    if (entry_3 != entry_3) {
                                        std::cout << "Derivative signaling NaN\n";
                                    }
                                    if (entry != REAL_T(0.0)) {//h[j][k] needs to be updated
                                        Reference(vk->id, vl->id) += entry;
                                        gh[l * ID_LIST_SIZE + k] = true;
                                        gh[k * ID_LIST_SIZE + l] = true;


                                    }
                                }
                            }
                        }

#pragma unroll
                        for (int k = j; k < rows; k++) {
                            vk = current_entry.id_list[k];

                            dk = current_entry.first[k];
                            pjk = current_entry.second[j * rows + k];
                            int ind = (j * rows * rows) + (k * rows);
                            for (int l = k; l < rows; l++) {

                                vl = current_entry.id_list[l];
                                entry_3 = 0.0;

                                dl = current_entry.first[l];
                                pjl = current_entry.second[j * rows + l];
                                pkl = current_entry.second[k * rows + l];

                                d3 = current_entry.third[ind + l];

                                entry_3 += (d3 * w)
                                        +(pjl * vij[k])
                                        + (dk * pjl * hii)
                                        + (dl * vijk_[(j * ID_LIST_SIZE + k)])
                                        + (pkl * vij[j])
                                        + (dk * dl * viij_[j])
                                        + (pjk * dl * hii);


                                entry_3 += (pjk * vij[l])
                                        + (dk * vijk_[(j * ID_LIST_SIZE + l)]);


                                entry_3 += dj * (vijk_[(k * ID_LIST_SIZE + l)] + (pkl * hii)+(dl * viij_[k]) + (dk * viij_[l])
                                        +(dk * dl * diii));

                                if (entry_3 != 0.0) {
                                    Reference(vj->id, vk->id, vl->id) += entry_3;
                                    ghh[j * ID_LIST_SIZE * ID_LIST_SIZE + k * ID_LIST_SIZE + j] = true;
                                }

                            }

                            for (int l = rows; l < ID_LIST_SIZE; l++) {

                                vl = current_entry.id_list[l];
                                entry_3 = 0;

                                dl = 0.0;
                                pjl = 0.0;
                                pkl = 0.0;


                                entry_3 += (pjk * vij[l])
                                        + (dk * vijk_[(j * ID_LIST_SIZE + l)]);


                                entry_3 += dj * (vijk_[(k * ID_LIST_SIZE + l)] + (dk * viij_[l]));

                                if (entry_3 != entry_3) {
                                    std::cout << "Derivative signaling NaN\n";
                                }
                                if (entry_3 != 0.0) {
                                    Reference(vj->id, vk->id, vl->id) += entry_3;
                                    ghh[j * ID_LIST_SIZE * ID_LIST_SIZE + k * ID_LIST_SIZE + j] = true;

                                }

                            }
                        }

                        if (dj != 0.0) {
#pragma unroll
                            for (int k = rows; k < ID_LIST_SIZE; k++) {
                                VariableInfoPtr vk = current_entry.id_list[k];
#pragma unroll
                                for (int l = k; l < ID_LIST_SIZE; l++) {

                                    VariableInfoPtr vl = current_entry.id_list[l];
                                    entry_3 = dj * (vijk_[(k * ID_LIST_SIZE + l)]);

                                    if (entry_3 != entry_3) {
                                        std::cout << "Derivative signaling NaN\n";
                                    }
                                    if (entry_3 != 0.0) {
                                        Reference(vj->id, vk->id, vl->id) += entry_3;
                                        ghh[j * ID_LIST_SIZE * ID_LIST_SIZE + k * ID_LIST_SIZE + j] = true;

                                    }

                                }
                            }
                        }
                    }



                    if (i > 0) {
                        if (current_entry.is_nl) {
                            for (int ii = 0; ii < ID_LIST_SIZE; ii++) {
                                for (int jj = ii; jj < ID_LIST_SIZE; jj++) {
                                    if (std::fpclassify(vij[jj]) != gh[ii * ID_LIST_SIZE + jj]) {
                                        stack[i - 1].Push(current_entry.id_list[jj]);
                                    }
                                    for (int kk = jj; kk < ID_LIST_SIZE; kk++) {
                                        if (std::fpclassify(viij_[kk]) != ghh[ii * ID_LIST_SIZE * ID_LIST_SIZE + jj * ID_LIST_SIZE + kk]) {
                                            stack[i - 1].Push(current_entry.id_list[kk]);
                                        }
                                    }
                                }
                            }
                        }

                    }
                }


            }

        }

        void Reset(bool empty_trash = true) {
            this->first_order_derivatives.clear();
            this->second_order_derivatives.clear();
            this->third_order_derivatives.clear();

            for (int i = 0; i < this->stack_current; i++) {
                stack[i].Reset();
            }

            if (empty_trash) {
                //                                                atl::VariableInfo< REAL_T>::FreeAll();
            }

            this->stack_current = 0;

        }

        void DumpDerivatives() {


            first_order_iterator ft;
            second_order_iterator st;
            third_order_iterator tt;

            std::cout << "first:\n";
            for (ft = this->first_order_derivatives.begin();
                    ft != this->first_order_derivatives.end(); ++ft) {

                std::cout << "[" << (*ft).first << "," << (*ft).second << "] ";

            }
            std::cout << "\n\nSecond:\n";

            for (st = this->second_order_derivatives.begin(); st != this->second_order_derivatives.end(); ++st) {

                for (ft = (*st).second.begin(); ft != (*st).second.end(); ++ft) {

                    std::cout << "[" << (*ft).first << "," << (*ft).second << "] ";

                }
                std::cout << "\n\n";

            }

        }



    };


}


#endif /* TAPE_HPP */

