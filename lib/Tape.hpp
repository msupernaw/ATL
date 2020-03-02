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

#include "DynamicExpression.hpp"
#include <iostream>
#include "VariableInfo.hpp"
#include "CLFAllocator.hpp"
#include <set>
#include <vector>
#include <unordered_map>
#include <set>
#include <cfloat>
#include <cmath>
#include "Utilities/flat_set.hpp"
#include "third_party/flat_hash_map/bytell_hash_map.hpp"
#include "Utilities/flat_set.hpp"

namespace atl {

    enum DerivativeTraceLevel {
        FIRST_ORDER_REVERSE = 0,
        FIRST_ORDER_REVERSE_COMPLEX_STEP,
        SECOND_ORDER_REVERSE,
        THIRD_ORDER_REVERSE,
        FIRST_ORDER_FORWARD,
        SECOND_ORDER_FORWARD,
        THIRD_ORDER_FORWARD,
        UTPM_REVERSE,
        DYNAMIC_RECORD
    };

    template<typename REAL_T>
    struct less_variable_info {

        bool operator()(const std::shared_ptr<VariableInfo<REAL_T> >& lhs, const std::shared_ptr<VariableInfo<REAL_T> >& rhs) const {
            return lhs->id < rhs->id;
        }
    };

    template<typename REAL_T>
    struct StackEntry {
        std::shared_ptr<DynamicExpressionBase<REAL_T> > exp;
        std::shared_ptr<DynamicExpressionBase<REAL_T> > texp;

        typedef typename std::shared_ptr<VariableInfo<REAL_T> > VariableInfoPtr;
        VariableInfoPtr w;
        std::vector<VariableInfoPtr> diff_w;
        std::vector< std::shared_ptr<DynamicExpressionBase<REAL_T> > > diff_exp;

        REAL_T tv;
        REAL_T wv;
        uint32_t min_id = std::numeric_limits<uint32_t>::max();
        uint32_t max_id = std::numeric_limits<uint32_t>::min();

        typedef flat_set<VariableInfoPtr, atl::less_variable_info<REAL_T> > vi_storage;
        typedef typename vi_storage::iterator vi_iterator;
        typedef typename vi_storage::reverse_iterator vi_riterator;
        bool is_nl = false;
        vi_storage ids;
        vi_storage nl_ids;
        vi_storage pushed_ids;
        std::vector<VariableInfoPtr > id_list;
        std::vector<REAL_T> first;
#warning use unordered_map here?????
        std::vector<REAL_T> second;
        std::vector<REAL_T> third;
        std::unordered_map<uint32_t, std::vector<REAL_T> > taylor_coeff;

        StackEntry() {

        }

        StackEntry(const StackEntry<REAL_T>& other) :
        exp(other.exp), w(other.w), wv(other.wv), min_id(other.min_id), max_id(other.max_id), is_nl(other.is_nl), ids(other.ids), nl_ids(other.nl_ids), pushed_ids(other.pushed_ids), id_list(other.id_list), first(other.first), second(other.second), third(other.third), taylor_coeff(other.taylor_coeff) {
        }


        //        StackEntry(const StackEntry<REAL_T>& other) :
        //        w(other.w),  exp(other.exp), is_nl(other.is_nl), ids(other.ids), nl_ids(other.nl_ids), pushed_ids(other.pushed_ids), id_list(other.id_list), first(other.first), second(other.second), third(other.third) {
        //        }

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

id_list.clear();

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
            this->exp.reset();
            min_id = std::numeric_limits<uint32_t>::max();
            max_id = std::numeric_limits<uint32_t>::min();
            w.reset(); // = NULL;
            //            for(int i=0; i < this->id_list.size(); i++){
            //                this->id_list[i]->value=0.0;
            //            }
            wv = 0.0;
            is_nl = false;
            first.resize(0);
            second.resize(0);
            third.resize(0);
            ids.clear_no_resize();//clear();
//            pushed_ids.clear();
            id_list.clear();
//            nl_ids.clear();
//            taylor_coeff.clear();
        }

        void Forward(const std::vector<uint32_t>& wrt) {

            this->w = this->exp->EvaluateDerivative(wrt);
        }



    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const StackEntry<T>& entry) {
        out << "--------------------------------------------------------------\n";
        //        out << entry.exp << "\n";
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

        out << "taylor_coeff:\n";
        if (entry.taylor_coeff.size())
            for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                std::cout << "id[" << (*it)->id << "] [";
                const std::vector<T>& coeff = entry.taylor_coeff.at((*it)->id);
                for (int i = 0; i < coeff.size(); i++) {
                    out << coeff[i] << " ";
                }
                out << "]\n";

            }
        out << "\n";

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

    class SpinLockGuard {
        SpinLock& sl;
    public:

        SpinLockGuard(SpinLock& sl) :
        sl(sl) {
            sl.lock();
        }

        ~SpinLockGuard() {
            sl.unlock();
        }

    };

    template<typename REAL_T>
    struct ForwardModeDerivativeInfo {
        uint32_t id;
        typename atl::StackEntry<REAL_T>::vi_storage ids;
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
        typedef ska::bytell_hash_map<uint32_t,
        REAL_T,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, REAL_T> > > first_order_container;

        first_order_container first_order_derivatives;
        first_order_container range_weights;

        typedef typename first_order_container::iterator first_order_iterator;

        //second-order storage
        typedef ska::bytell_hash_map<uint32_t,
        first_order_container,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, first_order_container> > > second_order_container;
        second_order_container second_order_derivatives;
        typedef typename second_order_container::iterator second_order_iterator;

        //third-order storage
        typedef ska::bytell_hash_map<uint32_t,
        second_order_container,
        std::hash<uint32_t>,
        std::equal_to<uint32_t>,
        atl::clfallocator<std::pair<const uint32_t, second_order_container> > > third_order_container;
        third_order_container third_order_derivatives;
        typedef typename third_order_container::iterator third_order_iterator;

        std::unordered_map<uint32_t, std::vector<REAL_T> > taylor_coeff;
        int taylor_order = 1;

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
        Tape(size_t size = 10000) {
            stack.reserve(size * 1.5);
            stack.resize(size);
        }

        ~Tape() {
            std::cout << "Destructing tape " << this << std::endl;
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
        
        /**
         * Extracts the dependent variables from the tape structure and 
         * returns them in a list.
         * 
         * @return 
         */
        const typename atl::StackEntry<REAL_T>::vi_storage DependentList() {
            typename atl::StackEntry<REAL_T>::vi_storage list;
            for (int i = 0; i < this->stack_current; i++) {
                list.insert(this->stack[i].w);
            }
            return list;
        }

        /**
         * Extracts the independent variables from the tape structure and 
         * returns them in a list.
         * 
         * @return 
         */
        const typename atl::StackEntry<REAL_T>::vi_storage IndependentList() {
            typename atl::StackEntry<REAL_T>::vi_storage dlist;
            //            for (int i = 0; i < this->stack_current; i++) {
            //                dlist.insert(this->stack[i].w);
            //            }

            typename atl::StackEntry<REAL_T>::vi_storage ilist;
            typename atl::StackEntry<REAL_T>::vi_iterator it;
            for (int i = 0; i < this->stack_current; i++) {
                for (it = this->stack[i].ids.begin(); it != this->stack[i].ids.end(); ++it) {
                    if ((*it)->count == 1) {
                        ilist.insert((*it));
                    }
                }
            }

            return ilist;
        }

        /**
         * Finds a list of dependent and independent variables.
         * 
         * @param dependents
         * @param independents
         */
        void GetVariableList(typename atl::StackEntry<REAL_T>::vi_storage& dependents, typename atl::StackEntry<REAL_T>::vi_storage& independents) {

            dependents.clear();
            independents.clear();

            for (int i = 0; i < this->stack_current; i++) {
                dependents.insert(this->stack[i].w);
            }

            typename atl::StackEntry<REAL_T>::vi_iterator it;
            for (int i = 0; i < this->stack_current; i++) {
                for (it = this->stack[i].ids.begin(); it != this->stack[i].ids.end(); ++it) {
                    if ((*it)->count == 1) {
                        independents.insert((*it));
                    }
                }
            }


        }

        inline StackEntry<REAL_T>& NextEntry() {
            return this->stack[this->NextIndex()];
        }

        void SetRangeWeight(uint32_t id, REAL_T w) {
            if (w != static_cast<REAL_T> (0.0))
                this->range_weights[id] = w;
        }

        inline const REAL_T GetRangeWeight(uint32_t i) {
#ifdef ATL_THREAD_SAFE
            SpinLockGuard slg(stack_lock);
#endif
            first_order_iterator it = this->range_weights.find(i);
            if (it != this->range_weights.end()) {
                return (*it).second;
            } else {
                return static_cast<REAL_T> (1.0);
            }
        }

        inline REAL_T& Reference(uint32_t i) {
            return this->first_order_derivatives[i];
        }

        inline REAL_T& Reference(uint32_t i, uint32_t j) {

            if (j > i) {
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
#ifdef ATL_THREAD_SAFE
            SpinLockGuard slg(stack_lock);
#endif
            first_order_iterator it = this->first_order_derivatives.find(i);
            if (it != this->first_order_derivatives.end()) {
                return (*it).second;
            } else {
                return static_cast<REAL_T> (0.0);
            }
        }

        inline const REAL_T Value(uint32_t i, uint32_t j) {
#ifdef ATL_THREAD_SAFE
            SpinLockGuard slg(stack_lock);
#endif
            if (j > i) {
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
#ifdef ATL_THREAD_SAFE
            SpinLockGuard slg(stack_lock);
#endif
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

        std::pair<uint32_t, uint32_t> FindMinMaxIds() {
               
            std::pair<uint32_t, uint32_t> ret;
            ret.first = std::numeric_limits<uint32_t>::max();
            ret.second = std::numeric_limits<uint32_t>::min();

            for (int i = 0; i < this->stack_current; i++) {
                ret.first = std::min(stack[i].min_id, ret.first);
                ret.second = std::max(stack[i].max_id, ret.second);
            }

            return ret;
        }

        /**
         * Accumulates reverse mode derivatives according 
         * to \a derivative_trace_level.
         * 
         */
        void Accumulate() {

            typename atl::StackEntry<REAL_T>::vi_iterator vit;

            std::vector<REAL_T>& W = this->taylor_coeff[this->stack[(stack_current - 1)].w->id];
            if (W.size()<this->taylor_order)W.resize(taylor_order + 1);
            for (int i = 0; i <= this->taylor_order; i++) {
                W[i] = 1.;
            }
            std::vector<REAL_T> WW(this->taylor_order + 1, 0.0);
            for (int i = (stack_current - 1); i >= 0; i--) {
                std::vector<REAL_T>& w = this->taylor_coeff[this->stack[i].w->id];


                for (int j = 1; j <= taylor_order; j++) {
                    WW[j] = w[j];
                }

                for (vit = this->stack[i].ids.begin(); vit != this->stack[i].ids.end(); ++vit) {
                    std::vector<REAL_T>& v = this->taylor_coeff[(*vit)->id];
                    if (v.size()<this->taylor_order + 1) {
                        v.resize(taylor_order + 1);
                    }
                    std::vector<REAL_T>& vbar = this->stack[i].taylor_coeff[(*vit)->id];
                    assert(vbar.size() == (taylor_order + 1));


                    for (int j = 1; j <= taylor_order; j++) {
                        for (int k = j; k <= taylor_order; k++) {
                            v[k] += (WW[k] * vbar[k]) / (1.0 / (REAL_T) factorial(k - 1));
                        }
                    }
                }
            }


        }

        int factorial(int n) {
            if (n > 1)
                return n * factorial(n - 1);
            else
                return 1;
        }

        void DynamicForward() {
            for (int i = 0; i < stack_current; i++) {

                atl::StackEntry<REAL_T>& entry = this->stack[i];
                entry.first.resize(entry.ids.size());
                int index = 0;
                typename atl::StackEntry<REAL_T>::vi_storage::const_iterator it;
                for (it = entry.ids.begin(); it != entry.ids.end(); ++it) {
                    entry.min_id = std::min((*it)->id, entry.min_id);
                    entry.max_id = std::max((*it)->id, entry.max_id);
                }

                REAL_T v = entry.exp->GetValue();
                entry.w->value = v;
            }
        }

        void DynamicReverse(first_order_container& derivatives) {
            derivatives[this->stack[(stack_current - 1)].w->id] = static_cast<REAL_T> (1.0);
            REAL_T w = static_cast<REAL_T> (0.0);
            typename atl::StackEntry< REAL_T>::vi_iterator vit;
            size_t index;
            for (int i = (stack_current - 1); i >= 0; i--) {

                REAL_T& W = derivatives[this->stack[i].w->id];
                if (W != static_cast<REAL_T> (0.0)) {
                    w = W;

                    W = static_cast<REAL_T> (0.0);
                    index = 0;
                    for (vit = this->stack[i].ids.begin(); vit != this->stack[i].ids.end(); ++vit) {
                        derivatives[(*vit)->id] += w * this->stack[i].first[index];
                        index++;
                    }

                }
            }

        }

        void AccumulateFirstOrder(bool reset = true) {
            if (recording) {

                if (reset) {
                    this->first_order_derivatives[this->stack[(stack_current - 1)].w->id] = static_cast<REAL_T> (1.0);
                }
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

        inline void PushLive(ska::bytell_hash_map<uint32_t, typename atl::StackEntry<REAL_T>::vi_storage >& live_set,
                typename atl::StackEntry<REAL_T>::VariableInfoPtr a,
                typename atl::StackEntry<REAL_T>::VariableInfoPtr b) {
            if (a->count > 1 && (b->id != a->id) && a->is_nl) {
                live_set[a->id].insert(b);
            }
        }

        void AccumulateSecondOrder() {
            if (recording) {


                REAL_T w;

                typename atl::StackEntry< REAL_T>::vi_iterator vit;
                //initialize w
                this->Reference(this->stack[stack_current - 1].w->id) = static_cast<REAL_T> (1.0);


                unsigned rows = 0; //the size of the local derivatives, anything higher was pushed from previous calculation

                std::vector<REAL_T> vij; //holds current second order derivative for i wrt j



                REAL_T hii = 0.0;
                REAL_T hij = 0.0;
                REAL_T dj = 0;
                REAL_T dk = 0;

                VariableInfoPtr vi;
                VariableInfoPtr vj;
                VariableInfoPtr vk;

                ska::bytell_hash_map<uint32_t, typename atl::StackEntry<REAL_T>::vi_storage > live_sets;


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

                    typename atl::StackEntry<REAL_T>::vi_storage& ls = live_sets[current_entry.w->id];
                    typename atl::StackEntry<REAL_T>::vi_iterator lsit;
                    for (lsit = ls.begin(); lsit != ls.end(); lsit++) {
                        current_entry.Push(*lsit);
                    }
                    ls.clear();

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




                            if (static_cast<REAL_T> (0.0) != entry) {//h[j][k] needs to be updated
                                this->Reference(vj->id, vk->id) += entry;
                                //                                this->PushLive(live_sets, vj, vk);
                                this->PushLive(live_sets, vk, vj);
                            }

                        }
                        for (int k = rows; k < ID_LIST_SIZE; k++) {


                            gh[j * ID_LIST_SIZE + k] = false;
                            gh[k * ID_LIST_SIZE + j] = false;
                            vk = current_entry.id_list[k];

                            entry = static_cast<REAL_T> (0.0); //the entry value for h[j][k]

                            dk = 0;





                            entry += vij[k] * dj;


                            if (j < rows && k < rows) {
                                entry += w * current_entry.second[j * rows + k];
                            }


                            if (static_cast<REAL_T> (0.0) != entry) {//h[j][k] needs to be updated
                                this->Reference(vj->id, vk->id) += entry;
                                //                                this->PushLive(live_sets, vj, vk);
                                this->PushLive(live_sets, vk, vj);
                            }

                        }
                    }
                }

            }

        }

        void AccumulateThirdOrder() {

            if (recording) {

                //                std::unordered_map<uint32_t, typename StackEntry<REAL_T>::vi_storage > dmap;
                //                for (int i = 0; i < this->stack_current; i++) {
                //                    typename StackEntry<REAL_T>::vi_iterator it;
                //                    typename StackEntry<REAL_T>::vi_storage& ids_ = dmap[stack[i].w->id];
                //                    for (it = stack[i].ids.begin(); it != stack[i].ids.end(); ++it) {
                //
                //                        if ((*it)->count == 1) {
                //                            ids_.insert((*it));
                //                        }
                //                    }
                //                }

                REAL_T w;

                //initialize w
                this->Reference(stack[stack_current - 1].w->id) = static_cast<REAL_T> (1.0);
                unsigned rows = 0; //the size of the local derivatives, anything higher was pushed from previous calculation

                std::vector<REAL_T> vij; //holds current second order derivative for i wrt j
                std::vector<REAL_T> viij_;
                std::vector<REAL_T> vijk_;


                REAL_T hii = 0.0;
                REAL_T diii = 0.0;
                REAL_T dj = 0.0;
                REAL_T dk = 0.0;
                REAL_T dl = 0.0;
                REAL_T pjl = 0.0;
                REAL_T pkl = 0.0;
                REAL_T entry_3 = 0;
                REAL_T d3 = 0.0;
                REAL_T pjk = 0.0;

                ska::bytell_hash_map<uint32_t, typename atl::StackEntry<REAL_T>::vi_storage > live_sets;


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

                    if (hii != static_cast<REAL_T> (0.0)) {
                        Reference(vi->id, vi->id) = 0.0;
                    }


                    diii = 0.0;
                    diii = Value(vi->id, vi->id, vi->id);

                    if (diii != static_cast<REAL_T> (0.0)) {
                        Reference(vi->id, vi->id, vi->id) = 0.0;
                    }


                    //retrieve live variables
                    typename atl::StackEntry<REAL_T>::vi_storage& ls = live_sets[current_entry.w->id];
                    typename atl::StackEntry<REAL_T>::vi_iterator lsit;
                    for (lsit = ls.begin(); lsit != ls.end(); lsit++) {
                        current_entry.Push(*lsit);
                    }
                    //clear the live variable set
                    ls.clear();


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
                            //                            this->PushLive(live_sets, vi, vj);
                            //                            this->PushLive(live_sets, vj, vi);
                        }

                        viij_[j] = 0.0;
                        viij_[j] = Value(vi->id, vi->id, vj->id);

                        if (std::fabs(viij_[j]) > 0.0) {
                            Reference(vi->id, vi->id, vj->id) = static_cast<REAL_T> (0.0);
                            //                            this->PushLive(live_sets, vi, vj);
                            //                            this->PushLive(live_sets, vj, vi);
                        }


#pragma unroll
                        for (unsigned k = j; k < ID_LIST_SIZE; k++) {
                            vk = current_entry.id_list[k];

                            vijk_[(j * ID_LIST_SIZE) + k] = 0.0;
                            vijk_[(j * ID_LIST_SIZE) + k] = Value(vi->id, vj->id, vk->id);

                            if (vijk_[(j * ID_LIST_SIZE) + k] != static_cast<REAL_T> (0.0)) {
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
                                        //                                        std::cout << "Derivative signaling NaN\n";
                                    }
                                    if (/*std::fabs(entry)*/entry != REAL_T(0.0)) {//h[j][k] needs to be updated
                                        this->Reference(vk->id, vl->id) += entry;
                                        //                                        this->PushLive(live_sets, vi, vj);
                                        this->PushLive(live_sets, vj, vi);
                                    }
                                }

                                for (int l = rows; l < ID_LIST_SIZE; l++) {

                                    vl = current_entry.id_list[l];

                                    hdk = 0;

                                    entry = 0.0; //the entry value for h[j][k]

                                    entry += vij[l] * hdj;

                                    if (entry_3 != entry_3) {
                                        //                                        std::cout << "Derivative signaling NaN\n";
                                    }
                                    if (entry != REAL_T(0.0)) {//h[j][k] needs to be updated
                                        Reference(vk->id, vl->id) += entry;
                                        //                                        this->PushLive(live_sets, vi, vj);
                                        this->PushLive(live_sets, vj, vi);
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

                                if (entry_3 != static_cast<REAL_T> (0.0)) {
                                    Reference(vj->id, vk->id, vl->id) += entry_3;
                                    //                                    this->PushLive(live_sets, vj, vl);
                                    //                                    this->PushLive(live_sets, vj, vk);
                                    this->PushLive(live_sets, vk, vj);
                                    this->PushLive(live_sets, vk, vl);

                                    //                                    this->PushLive(live_sets, vl, vk);
                                    //                                    this->PushLive(live_sets, vl, vj);
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
                                if (entry_3 != static_cast<REAL_T> (0.0)) {
                                    Reference(vj->id, vk->id, vl->id) += entry_3;
                                    //                                    this->PushLive(live_sets, vj, vl);
                                    //                                    this->PushLive(live_sets, vj, vk);
                                    this->PushLive(live_sets, vk, vj);
                                    this->PushLive(live_sets, vk, vl);

                                    //                                    this->PushLive(live_sets, vl, vk);
                                    //                                    this->PushLive(live_sets, vl, vj);
                                }

                            }
                        }

                        if (dj != static_cast<REAL_T> (0.0)) {
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
                                    if (entry_3 != static_cast<REAL_T> (0.0)) {
                                        Reference(vj->id, vk->id, vl->id) += entry_3;
                                        //                                        this->PushLive(live_sets, vj, vl);
                                        //                                        this->PushLive(live_sets, vj, vk);
                                        this->PushLive(live_sets, vk, vj);
                                        this->PushLive(live_sets, vk, vl);

                                    }

                                }
                            }
                        }
                    }

                }


            }

        }

        void Reset(bool reset_tape = true) {
            this->first_order_derivatives.clear();
            this->range_weights.clear();
            this->second_order_derivatives.clear();
            this->third_order_derivatives.clear();


            if (reset_tape) {
                for (int i = 0; i < this->stack_current; i++) {
                    stack[i].Reset();
                }
                this->stack_current = 0;
            }
            //           

            //            if (empty_trash) {
            //                //                                                atl::VariableInfo< REAL_T>::FreeAll();
            //            }

            //            

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

        void DumpHessianRow(uint32_t id) {


            first_order_iterator ft;
            second_order_iterator st;

            std::cout << "\n\nSecond:\n";

            st = this->second_order_derivatives.find(id);
            if (st != this->second_order_derivatives.end()) {
                for (ft = (*st).second.begin(); ft != (*st).second.end(); ++ft) {

                    std::cout << "[" << (*ft).first << "," << (*ft).second << "] \n";

                }
            }


        }


    };

    std::ostream& operator<<(std::ostream& out, const std::vector<uint32_t>& v) {
        for (int i = 0; i < v.size(); i++) {
            out << v[i] << " ";
        }
        out << std::endl;
        return out;
    }

    //    template<typename REAL_T>
    //    class DerivativeStructure {
    //    public:
    //        atl::Tape<REAL_T>& tape;
    //
    //        typename atl::Tape<REAL_T>::first_order_container gradient;
    //        typename atl::Tape<REAL_T>::second_order_container higher_order;
    //        std::vector< typename atl::Tape<REAL_T>::first_order_container > derivatives;
    //        int order;
    //        uint32_t min;
    //        uint32_t max;
    //        std::vector<std::vector<uint32_t> > indexes;
    //        typename atl::StackEntry<REAL_T>::vi_storage dependents;
    //        typename atl::StackEntry<REAL_T>::vi_storage independents;
    //
    //        DerivativeStructure(atl::Tape<REAL_T>& tape, int order = 1) :
    //        tape(tape), order(order) {
    //            this->derivatives.resize(order);
    //            std::pair<uint32_t, uint32_t> min_max = tape.FindMinMaxIds();
    //            this->min = min_max.first;
    //            this->max = min_max.second;
    //
    //            tape.GetVariableList(this->dependents, this->independents);
    //
    //            for (int i = 1; i <= order; i++) {
    //                indexes.push_back(std::vector<uint32_t>(i));
    //            }
    //
    //            typename atl::StackEntry<REAL_T>::vi_storage dependents = tape.DependentList();
    //
    //            for (int i = 0; i < tape.stack_current; i++) {
    //                tape.stack[i].diff_exp.resize(order + 1);
    //                tape.stack[i].diff_w.resize(order + 1);
    //                tape.stack[i].diff_exp[0] = tape.stack[i].exp;
    //                tape.stack[i].diff_w[0] = tape.stack[i].w;
    //
    //            }
    //
    //
    //            for (int j = 1; j <= order; j++) {
    //                std::unordered_map<uint32_t, typename atl::StackEntry<REAL_T>::VariableInfoPtr > mapped_var_info;
    //                typename atl::StackEntry<REAL_T>::vi_iterator it;
    //                for (it = dependents.begin(); it != dependents.end(); ++it) {
    //                    mapped_var_info[(*it)->id] =
    //                            std::shared_ptr<atl::VariableInfo<REAL_T> > (new atl::VariableInfo<REAL_T>((*it)->value));
    //                }
    //                for (int i = 0; i < tape.stack_current; i++) {
    //                    tape.stack[i].diff_w[j] = mapped_var_info[tape.stack[i].w->id];
    //                    tape.stack[i].diff_exp[j] = tape.stack[i].diff_exp[j - 1]->Differentiate();
    //                    tape.stack[i].diff_exp[j]->PushOrder(j);
    //                    tape.stack[i].diff_exp[j]->SwapDependents(mapped_var_info);
    //                    //                    tape.stack[i].diff_w[j]->value = tape.stack[i].diff_exp[j]->GetValue();
    //                }
    //            }
    //
    //
    //
    //        }
    //
    //        void Run() {
    //            int current_order = 1;
    //            gradient[tape.stack[(tape.stack_current - 1)].w->id] = static_cast<REAL_T> (1.0);
    //            REAL_T w = static_cast<REAL_T> (0.0);
    //            typename atl::StackEntry< REAL_T>::vi_iterator vit;
    //
    //
    //            for (int i = (tape.stack_current - 1); i >= 0; i--) {
    //                REAL_T& GW = gradient[tape.stack[i].w->id];
    //                REAL_T gw = GW;
    //                std::cout << "variable[" << tape.stack[i].w->id << "," << tape.stack[i].w->value << "] = " << tape.stack[i].exp->ToString() << "\n";
    //
    //                GW = static_cast<REAL_T> (0.0);
    //
    //                for (vit = tape.stack[i].ids.begin(); vit != tape.stack[i].ids.end(); ++vit) {
    //                    //Gradient 
    //                    gradient[(*vit)->id] += gw * tape.stack[i].exp->EvaluateDerivative((*vit)->id);
    //
    //                    if (this->order > 1) {
    //                        this->Recursive(current_order + 1, i,{(*vit)->id});
    //                    }
    //
    //                }
    //
    //            }
    //
    //            //            for (vit = this->independents.begin(); vit != this->independents.end(); ++vit) {
    //            //                std::vector<uint32_t> ids;
    //            //                ids.push_back((*vit)->id);
    //            //                this->EvalTop(1, ids);
    //            //            }
    //
    //
    //        }
    //
    //        inline REAL_T Value(std::vector<uint32_t> index) {
    //            int order = index.size();
    //            if (order == 1) {
    //                return this->gradient[index[0]];
    //            }
    //            //                        std::sort(index.begin(), index.end());
    //            uint32_t key = this->key(index, this->max);
    //            return this->derivatives[order - 1][key];
    //
    //        }
    //
    //    private:
    //
    //        inline void Eval(const int& current_order,
    //                const typename atl::StackEntry<REAL_T>::VariableInfoPtr& w_,
    //                const std::shared_ptr < atl::DynamicExpressionBase<REAL_T > >& exp_,
    //                const typename atl::StackEntry<REAL_T>::vi_storage& ids_,
    //                const std::vector< typename atl::StackEntry<REAL_T>::vi_iterator >& ids) {
    //
    //            typename atl::StackEntry<REAL_T>::vi_iterator piter = ids[ids.size() - 1];
    //            uint32_t pid = (*ids[ids.size() - 1])->id;
    //            typename atl::StackEntry<REAL_T>::vi_iterator it;
    //
    //            //            std::shared_ptr < atl::DynamicExpressionBase<REAL_T > > exp;
    //
    //
    //            //            uint32_t rank = (current_order % 2);
    //            //            if (rank == 0 && current_order != order) {
    //            //                exp = exp_->Differentiate((*ids[ids.size() - (current_order - 1)])->id);
    //            //            } else {
    //            //                exp = exp_;
    //            //            }
    //
    //            std::vector<uint32_t>& _ids = this->indexes[current_order - 1];
    //
    //            for (int i = 0; i < ids.size(); i++) {
    //                _ids[i] = (*ids[i])->id;
    //            }
    //            exp_->DifferentiatedBy(_ids);
    //
    //
    //
    //            _ids[_ids.size() - 1] = tape.stack[tape.stack_current - 1].w->id;
    //            size_t key = this->key(_ids, this->max);
    //            this->derivatives[current_order - 1][key] = static_cast<REAL_T> (1.0);
    //
    //            _ids[_ids.size() - 1] = w_->id;
    //            key = this->key(_ids, this->max);
    //            REAL_T& W = this->derivatives[current_order - 1][key];
    //
    //            REAL_T w = W;
    //            W = static_cast<REAL_T> (0.0);
    //
    //            REAL_T dx;
    //
    //            //            std::cout << exp->ToString() << " = ";
    //            for (it = ids_.begin(); it != ids_.end(); ++it) {
    //                _ids[_ids.size() - 1] = (*it)->id;
    //
    //                //
    //
    //                dx = w * exp_->EvaluateDerivative((*it)->id);
    //                if (dx != std::fpclassify(0.0)) {
    //                    key = this->key(_ids, this->max);
    //                    std::cout << std::scientific << "D[" << pid << "," << (*it)->id << "]{" << key << "} = ";
    //                    this->derivatives[current_order - 1][key] += dx;
    //                    std::cout << w << " * " << exp_->EvaluateDerivative(pid, _ids[_ids.size() - 1]) << "\n";
    //                    //                    std::cout << this->derivatives[current_order - 1][key] << "\n";
    //                }
    //                if (current_order < this->order) {
    //                    std::vector< typename atl::StackEntry<REAL_T>::vi_iterator > nids = ids;
    //                    nids.push_back(it);
    //                    this->Eval(current_order + 1, w_, exp_, ids_, nids);
    //                }
    //            }
    //
    //            //            dx = w * exp_->EvaluateDerivative(pid, pid);
    //            //            std::cout << "w{" << w << "} " << dx << " ";
    //            //            if (dx != std::fpclassify(0.0)) {
    //            //                _ids[_ids.size() - 1] = pid;
    //            //                key = this->key(_ids, this->max);
    //            //                this->derivatives[current_order - 1][key] += dx;
    //            //            }
    //            //
    //            //            if (current_order < this->order) {
    //            //                std::vector< typename atl::StackEntry<REAL_T>::vi_iterator > nids = ids;
    //            //                nids.push_back(piter);
    //            //                this->Eval(current_order + 1, w_, exp, ids_, nids);
    //            //            }
    //
    //        }
    //
    //        inline void Recursive(const int& current_order, size_t i, const std::vector<uint32_t>& ids) {
    //            std::vector<uint32_t>& index_l = this->indexes[current_order - 1];
    //
    //            REAL_T w = static_cast<REAL_T> (0.0);
    //            typename atl::StackEntry< REAL_T>::vi_iterator vit;
    //            index_l[index_l.size() - 1] = tape.stack[this->tape.stack_current - 1].diff_w[current_order - 1]->id;
    //            size_t key = this->key(index_l, this->max);
    //            this->derivatives[current_order - 1][key] = 1.0;
    //            for (vit = tape.stack[i].ids.begin(); vit != tape.stack[i].ids.end(); ++vit) {
    //                index_l[index_l.size() - 1] = tape.stack[i].diff_w[current_order - 1]->id;
    //                key = this->key(index_l, this->max);
    //                REAL_T& gw = this->derivatives[current_order - 1][key];
    //                index_l[index_l.size() - 1] = (*vit)->id;
    //                key = this->key(index_l, this->max);
    //                tape.stack[i].diff_exp[current_order - 1]->DifferentiatedBy(ids);
    //                this->derivatives[current_order - 1][key] += gw * tape.stack[i].diff_exp[current_order - 1]->EvaluateDerivative((*vit)->id);
    //                std::cout << this->derivatives[current_order - 1][key] << "\n";
    //            }
    //
    //
    //        }
    //
    //        inline void EvalTop(const int& current_order, const std::vector<uint32_t>& ids) {
    //
    //            //forward wrt to ids
    //            for (int i = 0; i < this->tape.stack_current; i++) {
    //                this->tape.stack[i].diff_exp[current_order]->DifferentiatedBy(ids);
    //                //                this->tape.stack[i].diff_w[current_order]->value = this->tape.stack[i].diff_exp[current_order]->GetValue();
    //            }
    //
    //            REAL_T w = static_cast<REAL_T> (0.0);
    //            typename atl::StackEntry< REAL_T>::vi_iterator vit;
    //
    //            std::vector<uint32_t> ids_l(ids.size() + 1);
    //            for (int i = 0; i < ids.size(); i++) {
    //                ids_l[i] = ids[i];
    //            }
    //            std::cout << "w[" << this->tape.stack[(tape.stack_current - 1)].diff_w[current_order]->id << "] = 1.0" << std::endl;
    //            ids_l[ids_l.size() - 1] = this->tape.stack[(tape.stack_current - 1)].diff_w[current_order]->id;
    //            higher_order[ids_l[0]][ids_l[1]] = 1.0;
    //            size_t key = this->key(ids_l, this->max);
    //            this->derivatives[current_order][key] = 1.0;
    //
    //            for (int i = (tape.stack_current - 1); i >= 0; i--) {
    //
    //                ids_l[ids_l.size() - 1] = this->tape.stack[i].diff_w[current_order]->id;
    //                size_t key = this->key(ids_l, this->max);
    //                //higher_order[ids_l[0]][ids_l[1]] = 1.0;
    //                REAL_T& GW = higher_order[ids_l[0]][ids_l[1]]; //this->derivatives[current_order][key];
    //                REAL_T gw = GW;
    //
    //                GW = static_cast<REAL_T> (0.0);
    //
    //
    //                for (vit = tape.stack[i].ids.begin(); vit != tape.stack[i].ids.end(); ++vit) {
    //                    ids_l[ids_l.size() - 1] = (*vit)->id;
    //                    size_t key = this->key(ids_l, this->max);
    //
    //                    //Gradient 
    //                    //                    this->derivatives[current_order][key] 
    //                    higher_order[ids_l[0]][ids_l[1]] += gw * this->tape.stack[i].diff_exp[current_order]->EvaluateDerivative((*vit)->id);
    //                    //                    if ((ids_l[0] == ids_l[1] && ids_l[0] == 4)) {
    //                    std::cout << tape.stack[i].diff_w[current_order]->id << "+=" << tape.stack[i].diff_exp[current_order]->Differentiate(ids_l[0])->ToString() << "\n";
    //                    //                    std::cout << tape.stack[i].diff_w[current_order]->id << "+=" << tape.stack[i].diff_exp[current_order]->ToString() << "\n";
    //                    //                    std::cout << higher_order[ids_l[0]][ids_l[1]] << "+= " << gw << "*" << this->tape.stack[i].diff_exp[current_order]->EvaluateDerivative((*vit)->id) << std::endl;
    //                    //                    }
    //
    //                }
    //
    //            }
    //
    //
    //
    //
    //        }
    //
    //        size_t key(const std::vector< uint32_t>& ids, int max) {
    //            if (ids.size() == 2) {
    //                return higher_order[ids[0]][ids[1]];
    //            }
    //            size_t order = ids.size();
    //            size_t key = 0;
    //            size_t m = max + 1;
    //            size_t no = order - 1;
    //            size_t tmp = 0;
    //            for (int i = 0; i < order - 1; i++) {
    //                tmp = (ids[i]);
    //                tmp *= std::pow(m, no);
    //                key += tmp;
    //                no--;
    //
    //
    //            }
    //            key += (ids[ids.size() - 1]);
    //            //            std::cout<<"key = "<<key<<"\n";
    //            return key;
    //        }
    //    };
    //

}


#endif /* TAPE_HPP */

