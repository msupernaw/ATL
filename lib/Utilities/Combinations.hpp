/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Combinations.hpp
 * Author: matthewsupernaw
 *
 * Created on February 4, 2016, 12:19 PM
 */

#ifndef COMBINATIONS_HPP
#define COMBINATIONS_HPP

namespace util {


#include <cassert>
#include <iostream>
#include <vector>

    /**
     * Simple class to create a list of combinations with repetition.
     * 
     * @param elems
     * @param req_len
     * @param pos
     * @param depth
     * @param margin
     * @param combos
     */
    class Combonations {

        inline static void CombinationsRRecursive(const std::vector<int> &elems, unsigned long req_len,
                std::vector<unsigned long> &pos, unsigned long depth,
                unsigned long margin, std::vector<std::vector<int> >& combos) {
            // Have we selected the number of required elements?
            if (depth >= req_len) {
                std::vector<int> v(pos.size());
                for (unsigned long ii = 0; ii < pos.size(); ++ii) {
                    v[ii] = elems[pos[ii]]; //.push_back(elems[pos[ii]]);
                }
                combos.push_back(v);
                return;
            }

            // Try to select new elements to the right of the last selected one.
            for (unsigned long ii = margin; ii < elems.size(); ++ii) {
                pos[depth] = ii;
                Combonations::CombinationsRRecursive(elems, req_len, pos, depth + 1, ii, combos);
            }
            return;
        }

        inline static void CombinationsR(const std::vector<int> &elems, unsigned long req_len, std::vector<std::vector<int> >& combos) {
            //            assert(req_len > 0 && req_len <= elems.size());
            std::vector<unsigned long> positions(req_len, 0);
            Combonations::CombinationsRRecursive(elems, req_len, positions, 0, 0, combos);
        }

        std::vector<std::vector<int> > combinations;

    public:

        Combonations(const std::vector<int>& elems, int choose) {
            this->CombinationsR(elems, choose, this->combinations);
        }

        virtual ~Combonations() {

        }

        inline std::vector<std::vector<int> >& GetCombinations() {
            return this->combinations;
        }

        inline static void Create(const std::vector<int> &elems, unsigned long req_len, std::vector<std::vector<int> >& combos) {
            Combonations::CombinationsR(elems, req_len, combos);
        }
    };

    class CombinationsWithRepetition {
        int n_m;
        int k_m;
        uint32_t number_of_combinations;
        uint32_t current_combination;
        std::vector<int> elements;

        unsigned int Factorial(uint32_t n) {
            unsigned int ret = 1;
            for (unsigned int i = 1; i <= n; ++i)
                ret *= i;
            return ret;
        }

        inline int Next_p(std::vector<int>& v, const int n, const int k) {
            int j; //index

            //easy case, increase rightmost element
            if (v[k - 1] < n - 1) {
                v[k - 1]++;
                return (true);
            }

            //find rightmost element to increase
            for (j = k - 2; j >= 0; j--)
                if (v[j] != n - 1)
                    break;

            //terminate if all elements are n - 1
            if (j < 0)
                return (false);

            //increase
            v[j]++;

            //set right-hand elements
            for (j += 1; j < k; j++)
                v[j] = v[j - 1];

            return (true);
        }

    public:

        CombinationsWithRepetition(int n, int k) :
        n_m(n), k_m(k), current_combination(0) {
            this->elements.resize(k, 0);
//            this->number_of_combinations = this->Factorial(n)/(Factorial(k)*Factorial(n-k));
        }

        inline bool Next() {
            current_combination++;
            return this->Next_p(elements, n_m, k_m);
        }

        int operator[](int i) {
            return elements[i];
        }

        size_t Size() {
            return k_m;
        }

        inline void Reset(int n, int k) {
            n_m = n;
            k_m = k;
            this->elements.resize(k);
            std::fill(elements.begin(), elements.end(), 0);
        }
    };


}


#endif /* COMBINATIONS_HPP */

