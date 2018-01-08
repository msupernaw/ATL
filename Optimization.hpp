/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FunctionMinimizer.hpp
 * Author: matthewsupernaw
 *
 * Created on February 9, 2016, 1:29 PM
 */

#ifndef OPTIMIZATION_HPP
#define OPTIMIZATION_HPP

#include "Utilities/StringUtil.hpp"
#include <vector>
#include <map>
#include "DerivativeChecker.hpp"
#include "Matrix.hpp"
#include <iomanip>
#include "support/port.hpp"
#include "support/cs_sparse.hpp"
#include "Tape.hpp"
#include "ThreadPool.hpp"
#include "Variable.hpp"




//#define ATL_HAS_EIGEN


#ifdef ATL_HAS_EIGEN

#define EIGEN_DONT_VECTORIZE 
#define EIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT


#include "third_party/CppNumericalSolvers/include/cppoptlib/meta.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/problem.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/lbfgssolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/gradientdescentsolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/bfgssolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/conjugatedgradientdescentsolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/gradientdescentsolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/neldermeadsolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/newtondescentsolver.h"
#include "third_party/CppNumericalSolvers/include/cppoptlib/solver/cmaessolver.h"

#endif

namespace atl {


    template<class T>
    class OptimizationRoutine;

    template<class T>
    class MCMC;

    template<class T>
    class MinimizerResults {
        T function_value;
        bool converged;
        T convergence_criteria;
        int max_phases;

        std::vector<atl::Variable<T>* > parameters_m;

        std::vector<T> gradient;


        std::vector<std::vector<T> > parameter_hessian;
        T log_determinant_of_parmameter_hessian;
        std::vector<std::vector<T> > parameter_var_covar_matrix;
        std::vector<T> parameter_standard_deviation;

        std::vector<atl::Variable<T>* > random_variables_m;
        std::vector<std::vector<T> > random_variable_hessian;
        T log_determinant_of_random_variable_hessian;
        std::vector<std::vector<T> > random_variable_var_covar_matrix;
        std::vector<T> random_variable_standard_deviation;
    };

    template<class T>
    struct ObjectiveFunctionStatistics {
        T function_value;
        bool converged;
        T convergence_criteria;
        T max_gradient_component;

        std::vector<std::string> parameter_names;
        std::vector<T> parameter_values;
        std::vector<T> gradient;
        std::vector<std::vector<T> > hessian;
        T log_determinant_of_hessian;
        std::vector<std::vector<T> > parameter_var_covar_matrix;
        std::vector<T> parameter_standard_deviation;

        std::vector<std::string> random_variable_names;
        std::vector<T> random_variable_values;


    };

    template<typename Type>
    struct SCResult {
        cs_sparse<Type>* A;
        cs_sparse<Type>* A_inv;
        cs_numeric<Type>* factor;
        Type log_det;

        /**
         * does not free A.
         */
        ~SCResult() {

            if (A_inv)
                cs_spfree<Type>(A_inv);

            if (factor)
                cs_nfree<Type>(factor);
        }
    };

    /**
     * Sparse Cholesky class used in ATL random effects modeling.
     * Uses CSparse
     */
    template<typename Type>
    class SparseCholesky {
        struct cs_symbolic<Type> *S = NULL;
        //    struct cs_numeric<Type> *chol;
        //
        //    struct cs_sparse<Type> *A_inv;
        //    struct cs_sparse<Type> *a_inv;

    public:

        SparseCholesky() {

        }

        /**
         * Constructor.
         * 
         * @param A in compressed format
         */
        SparseCholesky(struct cs_sparse<Type>* A) {
            S = cs_schol<double>(0, A);

        }

        /**
         * 
         * @param A       - input
         * @param A_inv   - output
         * @param factor  - output
         * @param log_det - output
         */
        const SCResult<Type> Analyze(cs_sparse<Type>* A) {

            SCResult<Type> ret;
            ret.A = A;


            //1. do symbolic factorization if needed
            if (S == NULL) {
                S = cs_schol<Type>(0, ret.A);
            }

            //2. do numeric factorization
            if (S != NULL) {
                ret.factor = cs_chol<Type>(ret.A, S);
                if (ret.factor == NULL) {
                    std::cout << "Cholesky factorization error. " << std::endl;
                }
            }



            //cholesky failed
            if (ret.factor == NULL) {
                ret.factor = cs_lu<Type>(ret.A, S, 1e-4);
                if (ret.factor == NULL) {
                    std::cout << "LU factorization error. " << std::endl;
                }
            }

            if (ret.factor == NULL) {
                ret.factor = cs_qr<Type>(ret.A, S);
                if (ret.factor == NULL) {
                    std::cout << "QR factorization error. " << std::endl;
                }
            }



            //3. compute the inverse subset
            if (ret.factor != NULL) {
                inv(ret);

            }

            //4. compute the log determinant
            if (ret.factor != NULL) {
                ret.log_det = cs_log_det<Type>(ret.factor->L);
            }
            return ret;
        }

        Type trace(struct cs_sparse<Type>* A) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;
            Type trace = 0.0;
            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    if (j == Ai[k]) {
                        trace += Ax[k];
                    }
                }
            }
            return trace;
        }

        void print_as_vector(struct cs_sparse<Type>* A) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {

                    std::cout << Ax[k] << "\n";

                }
            }
        }

        void diagonal(struct cs_sparse<Type>* A, Type* d, Type multiplier = static_cast<Type> (1.0)) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    if (j == Ai[k]) {
                        d[j] = Ax[k] * multiplier;
                    }
                }
            }
        }

        void half_diagonal(struct cs_sparse<Type>* A) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    if (j == Ai[k]) {
                        Ax[k] *= static_cast<Type> (.5);
                    }
                }
            }
        }

        void as_vector(struct cs_sparse<Type>* A, std::vector<Type>& d) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    d.push_back(Ax[k]);
                }
            }
        }

        ////            struct cs_sparse<Type>* cs_tril(struct cs_sparse<Type>* A, csi i) {
        //                struct cs_sparse<Type>* B = cs_spalloc<Type>(0, 0, 1, 1, 1);
        //                csi p, j, m, n, nzmax, nz, *Ap, *Ai;
        //                Type* Ax;
        //                n = A->n;
        //                Ap = A->p;
        //                Ai = A->i;
        //                Ax = A->x;
        //
        //                for (int j = 0; j < n; j++) {
        //                    for (int k = Ap [j]; k < Ap [j + 1]; k++) {
        //                        if (!(Ai[k] < j - i)) {
        //                            cs_entry<Type>(B, Ai[k], j, Ax[k]);
        //                        }
        //                    }
        //                }
        //
        //
        //                struct cs_sparse<Type>* BB = cs_compress<Type>(B);
        //                cs_free(B);
        //                return BB;
        //            }

        struct cs_sparse<Type>* cs_tril(struct cs_sparse<Type>* A, csi i) {
            struct cs_sparse<Type>* B = cs_spalloc<Type>(0, 0, 1, 1, 1);
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    if (!(Ai[k] < j - i)) {
                        cs_entry<Type>(B, Ai[k], j, Ax[k]);
                    }
                }
            }


            struct cs_sparse<Type>* BB = cs_compress<Type>(B);
            cs_free(B);
            return BB;
        }

        void cs_print_dense(struct cs_sparse<Type>* A) {
            struct cs_sparse<Type>* B = cs_transpose<Type>(A, 0);

            csi n, *Ap, *Ai;
            csi m = B->m;
            n = B->n;
            Ap = B->p;
            Ai = B->i;
            Type* Ax;
            Ax = B->x;
            for (int i = 0; i < n; i++) {
                int j = 0;
                if (Ap [i] == 0 && Ap [i + 1] == 0) {
                    int k = 0;
                    for (int k = 0; k < n; k++) {
                        std::cout << "0 ";
                    }
                }
                int k = 0;
                for (k = Ap [i]; k < Ap [i + 1]; k++) {


                    while (j++ < Ai[k]) {
                        std::cout << "0 ";
                    }
                    //                        std::cout << Ax[k] << " ";

                    if (k == Ap [i + 1] - 1) {
                        k = Ap [i + 1] - 1;
                        while (++k < n) {
                            std::cout << "0 ";
                        }
                    }

                }
                std::cout << std::endl;
            }
            cs_free(B);
        }

        void cs_clone(struct cs_sparse<Type>* A, struct cs_sparse<Type>* B) {
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    cs_entry<Type>(B, Ai[k], j, Ax[k]);
                }
            }

        }

        struct cs_sparse<Type>* clone(struct cs_sparse<Type>* A) {


            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            n = A->n;
            struct cs_sparse<Type>* B = cs_spalloc<Type>(n, n, 1, 1, 1);
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    cs_entry<Type>(B, Ai[k], j, Ax[k]);
                }
            }

            struct cs_sparse<Type>* ret = cs_compress<Type>(B);
            cs_spfree<Type>(B);
            return ret;
        }

    private:

        struct cs_sparse<Type>* mrdivide(struct cs_sparse<Type>* A, Type* B) {
            struct cs_sparse<Type>* ret = cs_spalloc<Type>(0, 0, 1, 1, 1);
            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            Type* Ax;
            m = A->m;
            n = A->n;
            Ap = A->p;
            Ai = A->i;
            Ax = A->x;
            nzmax = A->nzmax;
            nz = A->nz;

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    cs_entry<Type>(ret, Ai[k], j, Ax[k] / B[j]);
                }
            }

            struct cs_sparse<Type>* RET = cs_compress<Type>(ret);
            cs_free(ret);
            return RET;
        }

        void inv(SCResult<Type>& ret) {
            int n = static_cast<int> (ret.A->n);

            cs_sparse<Type>* a_inv = cs_spalloc<Type>(n, n, 1, 1, 1);
            cs_clone(ret.A, a_inv);

            ret.A_inv = cs_compress<Type>(a_inv);
            cs_spfree<Type>(a_inv);



            csi* L_p = ret.factor->L->p;
            csi* L_i = ret.factor->L->i;
            Type* L_x = ret.factor->L->x;


            //get the diagonal of L
            std::vector<Type> d(n);
            for (size_t j = 0; j < n; j++) {
                d[j] = L_x[L_p[j]];
                assert(d[j] != 0.0);
            }


            //right divide L by the diagonal
            cs_sparse<Type>* L_D = mrdivide(ret.factor->L, d.data());


            //Lower triangular part of L
            cs_sparse<Type>* L_tril = cs_tril(L_D, -1);

            L_p = L_tril->p;
            L_i = L_tril->i;
            L_x = L_tril->x;

            //square the diagonal
            for (size_t j = 0; j < n; j++) {
                d[j] = d[j] * d[j];
                assert(d[j] != 0.0);
            }







            // work space
            std::vector<Type> z(ret.A->nzmax);
            std::vector<csi> Zdiagp(ret.A->nzmax), Lmunch(ret.A->nzmax);
            //
            // compute the subset of the inverse 
            int flops = sparseinv(
                    n,
                    L_p,
                    L_i,
                    L_x,
                    d.data(),
                    L_p,
                    L_i,
                    L_x,
                    ret.A_inv->p,
                    ret.A_inv->i,
                    ret.A_inv->x,
                    z.data(),
                    Zdiagp.data(),
                    Lmunch.data()
                    );

            //free temporary structures

            cs_spfree<Type>(L_tril);
            cs_spfree<Type>(L_D);


        }


    };

    template<class T>
    class ObjectiveFunction : public DerivativeChecker<T> {
        std::map<atl::Variable<T>*, int> phase_info;
        std::vector<atl::Variable<T>* > parameters_m;
        std::vector<int> parameter_phases_m;
        std::vector<atl::Variable<T>* > random_variables_m;
        std::vector<int> random_variable_phases_m;

        std::vector<atl::Variable<T>* > all_parameters_m;
        int max_phase_m = 0;
        friend class OptimizationRoutine<T>;
        friend class MCMC<T>;

    public:

        int phase_m = 1;

        inline operator const atl::Variable<T>() {
            return this->Evaluate();
        }

        inline void ReassignIds() {
            std::vector<uint32_t > ids;
            for (int i = 0; i < this->parameters_m.size(); i++) {
                ids.push_back(parameters_m[i]->info->id); // = VariableIdGenerator::instance()->next();
            }

            for (int i = 0; i < this->random_variables_m.size(); i++) {
                ids.push_back(random_variables_m[i]->info->id); // = VariableIdGenerator::instance()->next();
            }

            size_t id = 0;
            for (int i = 0; i < this->parameters_m.size(); i++) {
                parameters_m[i]->info->id = ids[id++];
            }

            for (int i = 0; i < this->random_variables_m.size(); i++) {
                random_variables_m[i]->info->id = ids[id++];
            }

        }

        virtual void Initialize() {
        }

        int Phase() {
            return phase_m;
        }

        //        virtual inline const atl::Variable<T> Evaluate() = 0;

        void RegisterParameter(atl::Variable<T>& v, int phase = 1) {
            this->parameters_m.push_back(&v);
            this->parameter_phases_m.push_back(phase);
            this->phase_info[&v] = phase;
            if (phase > max_phase_m) {
                max_phase_m = phase;
            }
        }

        void RegisterRandomVariable(atl::Variable<T>& v, int phase = 1) {
            this->random_variables_m.push_back(&v);
            this->random_variable_phases_m.push_back(phase);
            if (phase > max_phase_m) {
                max_phase_m = phase;
            }
        }

        int GetActivePhase(const atl::Variable<T>& v) {
            typedef typename std::map<atl::Variable<T>*, int>::iterator piter;
            piter pi;
            pi = this->phase_info.find((atl::Variable<T>*) & v);
            if (pi != this->phase_info.end()) {
                return pi->second;
            }
            return 0;
        }

        bool IsActive(const atl::Variable<T>& v) {
            return GetActivePhase(v) <= this->phase_m;
        }

        bool LastPhase() {
            return this->current_phase == this->max_phase;
        }

        virtual void Objective_Function(atl::Variable<T>& ff) {
            //            ff.Copy(this->Evaluate());
        }

        const ObjectiveFunctionStatistics<T> GetObjectiveFunctionStatistics() {

            ObjectiveFunctionStatistics<T> stats;

            return stats;
        }

        const atl::RealMatrix<T> GetVarianceCovariance() {

            atl::Variable<T>::tape.Reset();
            struct cs_sparse<T>* RHessian = cs_spalloc<T>(0, 0, 1, 1, 1);
            atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;

            atl::Variable<T>::tape.recording = true;
            atl::Variable<T> f;

            this->Objective_Function(f);

            atl::Variable<T>::tape.AccumulateSecondOrder();


            for (int i = 0; i < this->parameters_m.size(); i++) {
                for (int j = 0; j < this->parameters_m.size(); j++) {
                    T dxx = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id,
                            this->parameters_m[j]->info->id);
                    if (dxx != dxx) {//this is a big hack
                        dxx = std::numeric_limits<T>::min();
                    }
                    if (dxx != static_cast<T> (0.0)) {
                        cs_entry<T>(RHessian, i, j, dxx);
                    }
                }
            }

            struct cs_sparse<T> *hessian = cs_compress<T>(RHessian);
            SparseCholesky<T> sparse_cholesky;

            SCResult<T> ret = sparse_cholesky.Analyze(hessian);

            csi p, j, m, n, nzmax, nz, *Ap, *Ai;
            T* Ax;
            n = ret.A_inv->n;
            Ap = ret.A_inv->p;
            Ai = ret.A_inv->i;
            Ax = ret.A_inv->x;

            atl::RealMatrix<T> inverse_hess(this->parameters_m.size(), this->parameters_m.size());

            for (int j = 0; j < n; j++) {
                for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                    inverse_hess(Ai[k], j) = Ax[k];
                }
            }

            cs_spfree(RHessian);
            cs_spfree(hessian);

            std::vector<T> se(this->parameters_m.size());
            for (int i = 0; i < this->parameters_m.size(); i++) {
                se[i] = std::sqrt(inverse_hess(i, i));
            }



            atl::RealMatrix<T> outer_product(this->parameters_m.size(), this->parameters_m.size());
            for (size_t i = 0; i < this->parameters_m.size(); i++) {
                for (size_t j = 0; j < this->parameters_m.size(); j++) {
                    outer_product(i, j) = se[i] * se[j];
                }
            }



            atl::RealMatrix<T> ret_m(this->parameters_m.size(), this->parameters_m.size());
            for (size_t i = 0; i < this->parameters_m.size(); i++) {
                for (size_t j = 0; j < this->parameters_m.size(); j++) {
                    ret_m(i, j) = inverse_hess(i, j) * outer_product(i, j);
                }
            }
            return ret_m;

            //            atl::Matrix<T> inverse_hess = atl::Matrix<T>::Identity(hess.Size(0));
            //
            //            std::cout << "\nHessian matrix\n" << hess << std::endl;
            //
            //            hess.Invert();
            //            inverse_hess = hess;
            //
            //            std::cout << "\nInverse of Hessian matrix\n" << inverse_hess << std::endl;
            //
            //            atl::Vector<T> se(hess.Size(0));
            //
            //            for (int i = 0; i < hess.Size(0); i++) {
            //                se(i) = std::sqrt(inverse_hess(i, i));
            //            }
            //
            //            std::cout << "\nSE vector\n" << se << std::endl;
            //
            //            atl::Matrix<T> var_covar = inverse_hess / (atl::OuterProduct(se, se));
            //
            //            std::cout << "\nVariance-covariance matrix\n" << var_covar << std::endl;
        }
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const ObjectiveFunctionStatistics<T>& stats) {
        out << "Function Value = " << stats.function_value << std::endl;
        out << "Max Gradient Component = " << stats.max_gradient_component << std::endl;
        out << std::left << std::setw(15) << "Parameter" << std::setw(15) << "Value" << std::setw(15) << "Gradient" << std::endl;
        for (int i = 0; i < stats.parameter_values.size(); i++) {
            out << std::setw(15) << stats.parameter_names[i] << std::setw(15) << stats.parameter_values[i] << std::setw(15) << stats.gradient[i] << std::endl;
        }
        return out;
    }

    template<typename T>
    struct CholSol {

        inline void operator()() {

        }
    };


    template<typename T>
    class Objective_Function2;

    template<class T>
    class OptimizationRoutine {
    protected:

        friend class Objective_Function2<T>;


        SparseCholesky<T> sparse_cholesky;

        ObjectiveFunction<T>* objective_function_m = NULL;
        std::vector<atl::Variable<T>* > parameters_m;
        std::vector<atl::Variable<T>* > random_variables_m;
        std::vector<atl::Variable<T>* > all_variables_m;

        //runtime 
        T function_value;
        T inner_function_value;
        T tolerance = 1e-4;
        T maxgc;
        T inner_maxgc;
        int phase_m;
        bool re_active = true;
        uint32_t max_line_searches = 1000;
        uint32_t max_iterations = 10000;
        size_t max_history = 1000;
        int print_width = 3;
        std::valarray<T> x;
        std::valarray<T> best;
        std::valarray<T> gradient;

        std::valarray<T> inner_x;
        std::valarray<T> inner_best;
        std::valarray<T> inner_gradient;
        std::valarray<T> inner_wg;

        atl::Tape<T> inner_gs;
        atl::Variable<T> log_det;

        std::vector< std::vector<int> > hessian_pattern_map;
        //
        long outer_iteration;


        struct cs_symbolic<T> *S_outer = NULL;
        struct cs_symbolic<T>* S_inner = NULL;
        std::vector<T> x_scratch;
        std::vector<T> re_dx;
        bool pattern_known = false;
    public:

        OptimizationRoutine(ObjectiveFunction<T>* objective_function = NULL) :
        objective_function_m(objective_function) {
        }

        ObjectiveFunction<T>* GetObjectiveFunction() const {
            return objective_function_m;
        }

        void SetObjectiveFunction(ObjectiveFunction<T>* objective_function) {
            this->objective_function_m = objective_function;
        }

        T GetTolerance() const {
            return tolerance;
        }

        void SetTolerance(T tolerance) {
            this->tolerance = tolerance;
        }

        int GetPrintWidth() const {
            return print_width;
        }

        void SetPrintWidth(int print_width) {
            this->print_width = print_width;
        }

        bool Run() {
            if (this->objective_function_m == NULL) {
                return false;
            }
            bool success = false;
            for (int i = 1; i <= this->objective_function_m->max_phase_m; i++) {
                this->Prepare(i);
                phase_m = i;
                success = this->Evaluate();
            }
            return success;
        }

        const MinimizerResults<T> GetResults() {

        }

    protected:

        void Prepare(int phase) {

            if (phase == 1) {
                this->objective_function_m->ReassignIds();
            }

            this->outer_iteration = 0;
            this->parameters_m.resize(0);
            this->random_variables_m.resize(0);
            this->all_variables_m.resize(0);
            this->objective_function_m->phase_m = phase;
            for (int i = 0; i < this->objective_function_m->parameter_phases_m.size(); i++) {
                if (this->objective_function_m->parameter_phases_m[i] <= phase) {
                    this->parameters_m.push_back(this->objective_function_m->parameters_m[i]);
                    this->all_variables_m.push_back(this->objective_function_m->parameters_m[i]);
                }
            }

            for (int i = 0; i < this->objective_function_m->random_variable_phases_m.size(); i++) {
                if (this->objective_function_m->random_variable_phases_m[i] <= phase) {
                    this->random_variables_m.push_back(this->objective_function_m->random_variables_m[i]);
                    this->all_variables_m.push_back(this->objective_function_m->random_variables_m[i]);
                }
            }


            this->x_scratch.resize(this->random_variables_m.size());
            this->hessian_pattern_map.resize(this->random_variables_m.size());
            re_dx.resize(this->random_variables_m.size());
        }

        void CallInnerObjectiveFunction(atl::Variable<T>& f) {
            atl::Variable<T>::tape.Reset();
            this->objective_function_m->Objective_Function(f);
        }

        void CallObjectiveFunction(atl::Variable<T>& f) {
            atl::Variable<T>::tape.Reset();
            if (this->random_variables_m.size() > 0) {//this is a laplace problem
                this->EvaluateLaplace(f);
            } else {
                this->objective_function_m->Objective_Function(f);
            }
        }

        void TransposeMatrix(std::vector<std::vector<T> >& m) {

#pragma unroll
            for (int i = 0; i < m.size(); i++) {
#pragma unroll
                for (int j = i + 1; j < m.size(); j++) {
                    std::swap(m[i][j], m[j][i]);
                }
            }
        }

        void TransposeMatrix(std::vector<T>& m) {
            size_t size = this->random_variables_m.size();
#pragma unroll
            for (int i = 0; i < size; i++) {
#pragma unroll
                for (int j = i + 1; j < size; j++) {
                    std::swap(m[i * size + j], m[j * size + i]);
                }
            }
        }

        void MatrixMultiply(std::vector<std::vector<T> >& a, std::vector<std::vector<T> >& b, std::vector<std::vector<T> >& ret) {
            for (int i = 0; i < a.size(); i++)
                for (int j = 0; j < a.size(); j++)
                    for (int k = 0; k < a.size(); k++) {
                        ret[i][j] += a[i][k] * b[k][j];
                    }
        }

        void MatrixMultiply(std::vector<T>& a, std::vector<T>& b, std::vector<T>& ret) {
            size_t size = this->random_variables_m.size();
            for (int i = 0; i < size; i++)
                for (int j = 0; j < size; j++)
                    for (int k = 0; k < size; k++) {
                        ret[i * size + j] += a[i * size + k] * b[k * size + j];
                    }
        }

        void ClearReStructures() {

        }

        struct cs_sparse<T>* InverseSubset(struct cs_sparse<T>* L) {

        }


    public:

        void EvaluateLaplace(atl::Variable<T>& f) {

            bool recording = atl::Variable<T>::tape.recording;
            size_t PARAMETERS_SIZE = this->parameters_m.size();
            size_t RANDOM_SIZE = this->random_variables_m.size();
            size_t ALL_SIZE = this->all_variables_m.size();
            struct cs_sparse<T>* RHessian = cs_spalloc<T>(0, 0, 1, 1, 1);

            if (recording) {
                for (int i = 0; i < RANDOM_SIZE; i++) {
                    this->random_variables_m[i]->SetValue(0.0);
                }

#ifdef USE_TIMER
                auto inner_start = std::chrono::steady_clock::now();
#endif
                std::cout << "Inner Minimization:\n";
                if (this->NewtonInner(10, 1e-4)) {
                    std::cout << "Inner converged.\n";
                    std::cout << "Inner f = " << this->inner_function_value << "\n";
                    std::cout << "Inner maxg = " << this->inner_maxgc << "\n";

                } else {
                    std::cout << "Inner failed.\n";
                    std::cout << "Inner f = " << this->inner_function_value << "\n";
                    std::cout << "Inner maxg = " << this->inner_maxgc << "\n";
                }
#ifdef USE_TIMER
                auto inner_end = std::chrono::steady_clock::now();
                std::chrono::duration<double> inner_time = (inner_end - inner_start);
                std::cout << "Inner fit time = " << inner_time.count() << " secs\n" << std::endl;
#endif
                std::unordered_map<std::shared_ptr<atl::VariableInfo<T> >, T> derivatives_logdet;
                std::unordered_map<std::shared_ptr<atl::VariableInfo<T> >, T> derivatives_f;

                this->ClearReStructures();
                atl::Variable<T>::tape.derivative_trace_level = atl::THIRD_ORDER_REVERSE;
                atl::Variable<T>::tape.recording = true;
                atl::Variable<T>::tape.Reset();
                atl::Variable<T> innerf; // = this->objective_function_m->Evaluate();
                this->objective_function_m->Objective_Function(innerf);
                if (innerf.GetValue() != innerf.GetValue()) {
                    std::cout << "Inner minimizer returned NaN, bailing out!" << std::endl;
                    exit(0);
                }

                std::cout << "Accumulating third-order mixed derivatives..." << std::flush;
#ifdef USE_TIMER
                auto to_start = std::chrono::steady_clock::now();
#endif
                atl::Variable<T>::tape.AccumulateThirdOrder();
                std::cout << "done." << std::endl;
#ifdef USE_TIMER
                auto to_end = std::chrono::steady_clock::now();
                std::chrono::duration<double> to_time = (to_end - to_start);
                std::cout << "Third-Order Accumulation time = " << to_time.count() << " secs\n" << std::endl;
#endif
                std::vector<T> g(PARAMETERS_SIZE);
                for (int j = 0; j < PARAMETERS_SIZE; j++) {
                    g[j] = atl::Variable<T>::tape.Value(this->parameters_m[j]->info->id);
                }

                if (!this->pattern_known) {
                    for (int i = 0; i < RANDOM_SIZE; i++) {
                        std::vector<int>& hm = hessian_pattern_map[i];
                        hm.clear();
                        for (int j = 0; j < RANDOM_SIZE; j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[j]->info->id);

                            if (dxx != 0.0) {
                                cs_entry<T>(RHessian, i, j, dxx);
                                hm.push_back(j);
                            }
                        }
                    }
                    this->pattern_known = true;
                } else {
                    for (int i = 0; i < RANDOM_SIZE; i++) {
                        std::vector<int>& hm = hessian_pattern_map[i];
                        for (int j = 0; j < hm.size(); j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[hm[j]]->info->id);
                            cs_entry<T>(RHessian, i, hm[j], dxx);

                        }
                    }
                }
                struct cs_sparse<T> *hessian = cs_compress<T>(RHessian);
                cs_spfree(RHessian);
                //                
                if (this->S_outer == NULL)
                    this->S_outer = cs_schol<T>(0, hessian);
                struct cs_numeric<T> *chol = cs_chol<T>(hessian, this->S_outer);





                //compute logdetH
                T ld = cs_log_det(chol->L);
                log_det = ld;



                for (int i = 0; i < PARAMETERS_SIZE; i++) {
                    derivatives_f[this->parameters_m[i]->info] = g[i];
                }


                //compute derivatives of the logdetH using third-order mixed partials
#ifdef USE_TIMER
                std::cout << "Starting Trace(H)..." << std::flush;
                auto trace_start = std::chrono::steady_clock::now();
#endif


                /*****************************************************************************************************
                 Notes from Kasper:
                 
                The first order banded Hessian is a useful test case as it should in theory take just O(RANDOM_SIZE).

                Your code has a nested loop of length PARAMETERS_SIZE * RANDOM_SIZE.
                Inside that loop there's a further loop of length RANDOM_SIZE hidden in the cs_cholsol call. So your algorithm takes at least
                O( PARAMETERS_SIZE * RANDOM_SIZE^2 )

                The comments following equation 8 eliminate the need for the nested loop:

                The multiple matrix-vector solves can be replaced by a single pass through the inverse subset algorithm.

                The outer loop can be replaced be a single reverse sweep of tape T3 using the elements of the inverse subset as range weights.

                I admit that this may not be completely obvious from the paper :)
                 
                 *****************************************************************************************************/



                std::cout << "start\nlog det actual:     ";
                for (int p = 0; p < PARAMETERS_SIZE; p++) {
                    T trace = 0;
                    for (int i = 0; i < RANDOM_SIZE; i++) {

                        std::fill(re_dx.begin(), re_dx.end(), 0.0);
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j < hm.size(); j++) {

                            re_dx[hm[j]] = atl::Variable<T>::tape.Value(
                                    this->random_variables_m[i]->info->id,
                                    this->random_variables_m[hm[j]]->info->id,
                                    this->parameters_m[p]->info->id);


                        }
                        std::fill(x_scratch.begin(), x_scratch.end(), static_cast<T> (0.0));
                        int error = cs_cholsol_x(0, hessian, re_dx.data(), chol, this->S_outer, this->x_scratch.data(), i);
                        trace += re_dx[i];
                    }
                    //                    std::cout << "{" << this->parameters_m[p]->info->id << ", " << trace << "} ";
                    derivatives_logdet[this->parameters_m[p]->info] = trace;
                }
                std::cout << "\n";





#ifdef USE_TIMER
                auto trace_end = std::chrono::steady_clock::now();
                std::chrono::duration<double> trace_time = (trace_end - trace_start);
                std::cout << "Derivative Logdet time = " << trace_time.count() << " secs\n" << std::endl;
#endif


                //                struct cs_sparse<T>* fuu = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                struct cs_sparse<T>* h_theta = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                struct cs_sparse<T>* fu_theta = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                for (int j = 0; j < this->parameters_m.size(); j++) {
                //                    cs_entry<T>(h_theta, 0, j, atl::Variable<T>::tape.Value(this->parameters_m[j]->info->id));
                //                }
                //                struct cs_sparse<T>* ftheta_theta = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                struct cs_sparse<T>* h_u = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                struct cs_sparse<T>* h_theta = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                struct cs_sparse<T>* fuu_all = cs_spalloc<T>(0, 0, 1, 1, 1);
                //
                //                for (int j = 0; j < this->all_variables_m.size(); j++) {
                //                    for (int i = 0; i < this->all_variables_m.size(); i++) {
                //                        cs_entry<T>(fuu_all, j, i, atl::Variable<T>::tape.Value(this->all_variables_m[j]->info->id, this->all_variables_m[i]->info->id));
                //                    }
                //                }

                //                for (int j = 0; j < this->parameters_m.size(); j++) {
                //                    cs_entry<T>(h_theta, 0, j, atl::Variable<T>::tape.Value(this->parameters_m[j]->info->id));
                //                    for (int i = 0; i < this->parameters_m.size(); i++) {
                //                        cs_entry<T>(ftheta_theta, j, i, atl::Variable<T>::tape.Value(this->parameters_m[j]->info->id, this->parameters_m[i]->info->id));
                //                    }
                //                }

                //                int fuu_nonzeros = 0;
                //                std::vector<T> hu(this->random_variables_m.size());
                //                for (int i = 0; i < this->all_variables_m.size(); i++) {
                //                    for (int j = 0; j < this->all_variables_m.size(); j++) {
                //                        cs_entry<T>(fuu, i, j, atl::Variable<T>::tape.Value(this->all_variables_m[i]->info->id,
                //                                this->all_variables_m[j]->info->id));
                //                    }
                //                }
                //             
                //
                //
                //                struct cs_sparse<T>* fuu_ = cs_compress<T>(fuu);
                //                //                struct cs_sparse<T>* hu_ = cs_compress<T>(h_u);
                //                //                struct cs_sparse<T>* fu_theta_ = cs_compress<T>(fu_theta);
                //                //                struct cs_sparse<T>* h_theta_ = cs_compress<T>(h_theta);
                //                SCResult<T> ret = this->sparse_cholesky.Analyze(fuu_);
                //                atl::Variable<T>::tape.Reset();
                //                
                //                
                //                std::cout << "start\nlog det actual with mult:     ";
                //                struct cs_sparse<T>* Fuu = this->sparse_cholesky.clone(hessian);
                //                //                 
                //                for (int p = 0; p < PARAMETERS_SIZE; p++) {
                //                    T trace = 0;
                //
                //                    csi j, n, *Ap, *Ai;
                //                    T* Ax;
                //                    n = hessian->n;
                //                    Ap = hessian->p;
                //                    Ai = hessian->i;
                //                    Ax = hessian->x;
                //
                //                    T* Bx;
                //                    //                    Bx = Fuu->x
                //                    for (int j = 0; j < n; j++) {
                //                        for (int k = Ap [j]; k < Ap [j + 1]; k++) {
                //                            //                            std::cout<<Ai[k]<<", "<<j<<", "<<p<<"\n";
                //                            T entry = atl::Variable<T>::tape.Value(this->random_variables_m[Ai[k]]->info->id,
                //                                    this->random_variables_m[j]->info->id,
                //                                    this->parameters_m[p]->info->id);
                //                            Fuu->x[k] = entry;
                //                            //                            cs_entry<T>(Fuu, k, j, entry);
                //                        }
                //                    }
                //
                //
                //                    struct cs_sparse<T>* MM = cs_multiply<T>(Fuu, ret.A_inv);
                //
                //                    trace = sparse_cholesky.trace(MM);
                //
                //
                //                    //                    std::cout << "{" << this->parameters_m[p]->info->id << ", " << trace << "} ";
                //                    derivatives_logdet[this->parameters_m[p]->info] = trace;
                //                }
                //                std::cout << "\n";
                //








                //                struct cs_sparse<T>* tril_inv = this->sparse_cholesky.cs_tril(ret.A_inv, 0);
                //                this->sparse_cholesky.half_diagonal(tril_inv);
                //                                for (int p = 0; p < PARAMETERS_SIZE; p++) {
                //                                    struct cs_sparse<T>* ppp = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                
                //                                    T trace = 0;
                //                                    for (int i = 0; i < RANDOM_SIZE; i++) {
                //                
                //                                        std::vector<int>& hm = this->hessian_pattern_map[i];
                //                                        for (int j = 0; j < hm.size(); j++) {
                //                
                //                                            cs_entry<T>(ppp, i, hm[j], atl::Variable<T>::tape.Value(
                //                                                    this->random_variables_m[i]->info->id,
                //                                                    this->random_variables_m[hm[j]]->info->id,
                //                                                    this->parameters_m[p]->info->id));
                //                
                //                                        }
                //                
                //                                    }
                //                                    struct cs_sparse<T>* PPP = cs_compress<T>(ppp);
                //                                    struct cs_sparse<T>* product = cs_multiply<T>(PPP, ret.A_inv);
                //                                    trace = sparse_cholesky.trace(product);
                ////                                    derivatives_logdet[this->parameters_m[p]->info] = trace;
                //                                    std::cout << trace << " ";
                //                                    cs_spfree<T>(ppp);
                //                                    cs_spfree<T>(PPP);
                //                                    cs_spfree<T>(product);
                //                                }
                //                
                //                                std::cout << "\n";


                //                this->sparse_cholesky.print_as_vector(ret.A_inv);
                //
                //                                struct cs_sparse<T>* v = cs_multiply(hu_, ret.A_inv);
                //                                struct cs_sparse<T>* V = cs_compress<T>(v);
                //                                sparse_cholesky.print_as_vector(v);
                //                                cs_print(v, 1);
                //                std::cout << hu.size() << " --- " << this->random_variables_m.size() << "\n";
                //                for (int i = 0; i < hu.size(); i++) {
                //                    cs_entry<T>(v, 0, i, hu[i]);
                //                }
                //                std::fill(x_scratch.begin(), x_scratch.end(), static_cast<T> (0.0));
                //                struct cs_sparse<T>* tri_inv = sparse_cholesky.cs_tril(ret.A_inv, 0);
                //                sparse_cholesky.half_diagonal(tri_inv); //,hu.data(),.5);
                //                struct cs_sparse<T>* v = cs_multiply(hu_, tri_inv);
                //                //                                struct cs_sparse<T>* V = cs_compress<T>(v);
                ////                sparse_cholesky.print_as_vector(v);
                //                std::cout << fuu_nonzeros << " nonzeros in hessian\n";
                //
                //                cs_cholsol(0, tri_inv, hu.data()); //, ret.factor, this->S_outer, this->x_scratch.data());//, 0);
                //                std::cout << ret.A_inv->nz << " non-zeros in hessian^-1\n";
                //                //                std::cout << hu.size() << " --- " << this->random_variables_m.size() << "\n";
                //                //                hu[0] = 1.0;
                //
                //                //                struct cs_sparse<T>* Vc = cs_compress<T>(v);
                //                //                struct cs_sparse<T>* mm = cs_multiply<T>(V, ret.A_inv);
                //                //
                //                //                struct cs_sparse<T>* m2 = cs_multiply<T>(mm, fu_theta_);
                //                //                struct cs_sparse<T>* m3 = cs_add<T>(h_theta_, m2, 1.0, -1.0);
                //                //                                cs_print<T>(mm, 1);
                //                //
                //
                //                atl::Variable<T>::tape.Reset();
                //                atl::Variable<T>::tape.recording = true;
                //                atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
                //                f = this->objective_function_m->Evaluate();
                //
                //
                //                atl::Variable<T>::tape.AccumulateFirstOrder();
                //                std::cout << atl::Variable<T>::tape.DependentList().size() << " == " << hu.size() << "\n";
                //                for (int i = 0; i < hu.size(); i++) {
                //                    //                    std::cout << hu[i] << "\n";
                //                }
                //
                //                //                std::fill(hu.begin(), hu.end(), static_cast<T> (0.0));
                //                //                sparse_cholesky.diagonal(ret.A_inv, hu.data());
                //                //                                for (int i = 0; i < hu.size(); i++) {
                //                //                                                        atl::Variable<T>::tape.SetRangeWeight(-1 * this->random_variables_m[i]->info->id, hu[i]);
                //                //                                    atl::Variable<T>::tape.Reference(this->random_variables_m[i]->info->id) = hu[i];
                //                //                    T& w = atl::Variable<T>::tape.Reference(this->random_variables_m[i]->info->id);
                //                //                    std::cout<<w<<" -- > ";
                //                //                    w += hu[i];
                //                //                     std::cout<<w<<"\n";
                //                //                    iii--;
                //                //                    std::cout<<hu[i]<<" ";
                //                //                                }
                //                //                //                std::cout<<"\n";
                //                //                atl::Variable<T>::tape.AccumulateFirstOrder(false);
                //                atl::Variable<T>::tape.UTPM(hu);
                //                atl::Variable<T>::tape.AccumulateFirstOrder(false);
                //                //
                //                std::cout << "seeded second pass: ";
                //                //                struct cs_sparse<T>* vVV = cs_spalloc<T>(0, 0, 1, 1, 1);
                //                //                 for (int i = 0; i < this->random_variables_m.size(); i++) {
                //                //                    cs_entry<T>(vVV, 0,i, atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id));
                //                //                 }
                //                //                
                //                for (int i = 0; i < this->parameters_m.size(); i++) {
                //                    //                    cs_entry<T>(vVV, i, 0, atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id));
                //                    std::cout << "{" << this->parameters_m[i]->info->id << ", " << atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id) << "} ";
                //                }
                //                std::cout << "\ndone \n";

                //                struct cs_sparse<T>* VVV = cs_compress<T>(vVV);
                //                struct cs_sparse<T>* m4 = cs_multiply<T>(VVV, fu_theta_);
                //                cs_print<T>(m4, 1);
                atl::Variable<T>::tape.Reset();
                atl::Variable<T>::tape.recording = false;
                f = 0.0;
                this->objective_function_m->Objective_Function(f);
                //                f.SetName("F");
                atl::Variable<T>::tape.recording = recording;


                //push adjoint entry for log_det
                atl::StackEntry<T>& entry = atl::Variable<T>::tape.NextEntry();
                entry.w = log_det.info;
                typename std::unordered_map<std::shared_ptr<atl::VariableInfo<T> >, T>::iterator it;
                for (it = derivatives_logdet.begin(); it != derivatives_logdet.end(); ++it) {
                    entry.ids.insert((*it).first);
                }
                typename atl::StackEntry<T>::vi_iterator id_it;
                entry.first.resize(entry.ids.size());
                size_t in = 0;
                for (id_it = entry.ids.begin(); id_it != entry.ids.end(); ++id_it) {
                    T dx = derivatives_logdet[(*id_it)];
                    //                    std::cout<<(*id_it)->id<<" "<<dx<<"\n";
                    entry.first[in] = dx;
                    in++;
                }
                //                exit(0);

                //push adjoint entry for objective function
                atl::StackEntry<T>& entry2 = atl::Variable<T>::tape.NextEntry();
                entry2.w = f.info;
                for (it = derivatives_f.begin(); it != derivatives_f.end(); ++it) {
                    entry2.ids.insert((*it).first);
                }
                entry2.first.resize(entry.ids.size());
                in = 0;
                for (id_it = entry2.ids.begin(); id_it != entry2.ids.end(); ++id_it) {

                    T dx = derivatives_f[(*id_it)];
                    entry2.first[in] = dx;
                    in++;
                }
                atl::Variable<T>::tape.recording = true;

                f += static_cast<T> (.5) * log_det;
                f -= static_cast<T> (.5)*(static_cast<T> (RANDOM_SIZE) * std::log((static_cast<T> (2.0 * M_PI))));

                cs_spfree(hessian);
                cs_nfree(chol);

            } else {

                atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
                this->ClearReStructures();
                atl::Variable<T>::tape.recording = true;
                atl::Variable<T>::tape.Reset();
                atl::Variable<T> innerf; // = this->objective_function_m->Evaluate();
                this->objective_function_m->Objective_Function(innerf);
                atl::Variable<T>::tape.AccumulateSecondOrder();
                if (!this->pattern_known) {

                    for (int i = 0; i < RANDOM_SIZE; i++) {
                        std::vector<int>& hm = hessian_pattern_map[i];
                        hm.clear();
                        for (int j = 0; j < RANDOM_SIZE; j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[j]->info->id);
                            if (dxx != 0.0) {
                                hm.push_back(j);
                                cs_entry<T>(RHessian, i, j, dxx);
                            }
                        }
                    }
                    this->pattern_known = true;
                } else {
                    for (int i = 0; i < RANDOM_SIZE; i++) {
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j < hm.size(); j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[hm[j]]->info->id);
                            cs_entry(RHessian, i, hm[j], dxx);
                        }
                    }
                }

                struct cs_sparse<T> *hessian = cs_compress<T>(RHessian);
                cs_spfree(RHessian);
                //                
                if (this->S_outer == NULL)
                    this->S_outer = cs_schol<T>(0, hessian);


                if (this->S_outer == NULL) {
                    std::cout << "S_OUTER is still NULL" << std::endl;
                    exit(0);

                }
                struct cs_numeric<T> *chol = cs_chol<T>(hessian, this->S_outer);




                //compute logdetH
                T ld;
                if (chol == NULL) {
                    ld = 1.0;
                } else {
                    ld = cs_log_det(chol->L);
                }

                log_det = ld;
                atl::Variable<T>::tape.recording = false;
                f = 0.0;
                //                f = this->objective_function_m->Evaluate();
                this->objective_function_m->Objective_Function(f);
                f += static_cast<T> (.5) * log_det;
                f -= static_cast<T> (.5)*(static_cast<T> (RANDOM_SIZE) * std::log((static_cast<T> (2.0 * M_PI))));

                cs_spfree(hessian);
                cs_nfree(chol);

            }
        }

        void ComputeGradient(std::vector<atl::Variable<T>* >&p,
                std::valarray<T>&g, T & maxgc) {
            g.resize(p.size());
            atl::Variable<T>::tape.AccumulateFirstOrder();

            for (int i = 0; i < g.size(); i++) {
                g[i] = atl::Variable<T>::tape.Value(p[i]->info->id);
                if (i == 0) {
                    maxgc = std::fabs(g[i]);
                } else {
                    if (std::fabs(g[i]) > maxgc) {

                        maxgc = std::fabs(g[i]);
                    }
                }
            }
        }

        void Print() {

            const int name_width = 35;
            const int value_width = 12;
            const int grad_width = 12;

            std::cout << "Iteration: " << this->outer_iteration << "\n";
            std::cout << "Phase: " << this->phase_m << "\n";
            std::cout << "Function Value = " << this->function_value << "\n";
            std::cout << "Max Gradient Component: " << this->maxgc << "\n";
            std::cout << "Floating-Point Type: Float" << (sizeof (T)*8) << "\n";
            std::cout << "Number of Parameters: " << this->parameters_m.size() << "\n";
            std::cout << ' ' << std::string((print_width * (name_width + 1 + value_width + 1 + grad_width + 1)), '-') << "\n";
            for (int i = 0; i < print_width; i++) {
                std::cout << '|' << util::center("Name", name_width) << '|' << util::center("Value", value_width) << '|' << util::center("Gradient", 12);
            }
            //            << '|' << util::center("Name", name_width) << '|' << util::center("Value", value_width) << '|' << util::center("Gradient", 12)
            //                    << '|' << util::center("Name", name_width) << '|' << util::center("Value", value_width) << '|' << util::center("Gradient", 12)
            std::cout << '|' << std::endl;
            std::cout << ' ' << std::string((print_width * (name_width + 1 + value_width + 1 + grad_width + 1)), '-') << "\n";

            int i = 0;
            std::cout.precision(4);
            std::cout << std::scientific;

            for (i = 0; (i + print_width) < this->parameters_m.size(); i += print_width) {
                for (int j = 0; j < print_width; j++) {
                    if (std::fabs(this->gradient[i + j]) == this->maxgc) {
                        std::cout << '|' << util::center("*" + this->parameters_m[i + j]->GetName(), name_width) << '|';
                    } else {
                        std::cout << '|' << util::center(this->parameters_m[i + j]->GetName(), name_width) << '|';
                    }
                    std::cout << std::setw(value_width) << this->parameters_m[i + j]->GetValue() << '|' << std::setw(grad_width) << this->gradient[i + j];
                }
                std::cout << "|\n";
            }

            for (; i< this->parameters_m.size(); i++) {
                if (std::fabs(this->gradient[i]) == this->maxgc) {
                    std::cout << '|' << util::center("*" + this->parameters_m[i]->GetName(), name_width) << '|';
                } else {
                    std::cout << '|' << util::center(this->parameters_m[i]->GetName(), name_width) << '|';
                }
                std::cout << std::setw(value_width) << this->parameters_m[i]->GetValue() << '|' << std::setw(grad_width) << this->gradient[i];

               
            }

            std::cout << "|\n" << ' ' << std::string((print_width * (name_width + 1 + value_width + 1 + grad_width + 1)), '-') << "\n";
            std::cout << "\n\n";
        }

        virtual bool Evaluate() = 0;

        T abs_sum(const std::valarray<T>&array) {
            T sum = 0.0;
            for (int i = 0; i < array.size(); i++) {
                sum += std::fabs(array[i]);
            }
            return sum;
        }

        bool NewtonInner(int max_iter = 20, T tol = 1e-12) {

            atl::Variable<T> fx;

            int nops = this->random_variables_m.size();

            std::vector<T> gradient_(nops);

            struct cs_symbolic<T>* S = NULL;
            for (int iter = 0; iter < max_iter; iter++) {


                atl::Variable<T>::tape.Reset();
                atl::Variable<T>::SetRecording(true);
                atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
                //                fx = this->objective_function_m->Evaluate();
                fx = 0.0;
                this->objective_function_m->Objective_Function(fx);
                this->inner_function_value = fx.GetValue();

                atl::Variable<T>::tape.AccumulateSecondOrder();
                struct cs_sparse<T>* RHessian = cs_spalloc<T>(0, 0, 1, 1, 1);


                if (!this->pattern_known) {

                    for (int i = 0; i <this->random_variables_m.size(); i++) {
                        gradient_[i] = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id);

                        int jj = 0;
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j <this->random_variables_m.size(); j++) {

                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[j]->info->id);

                            if (std::fpclassify(dxx) != FP_ZERO) {
                                hm.push_back(j);
                                cs_entry(RHessian, i, j, dxx);
                            }
                        }
                    }
                    this->pattern_known = true;
                } else {

                    for (int i = 0; i <this->random_variables_m.size(); i++) {
                        gradient_[i] = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id);
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j < hm.size(); j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->random_variables_m[i]->info->id, this->random_variables_m[hm[j]]->info->id);
                            cs_entry(RHessian, i, hm[j], dxx);
                        }
                    }
                }



                struct cs_sparse<T> *hessian = cs_compress<T>(RHessian);
                cs_spfree(RHessian);
                if (this->S_outer == NULL) {
                    this->S_outer = cs_schol<T>(0, hessian);
                }

                this->inner_maxgc = std::fabs(gradient_[0]);
                for (int i = 0; i < gradient_.size(); i++) {
                    if (std::fabs(gradient_[i]) > this->inner_maxgc) {
                        this->inner_maxgc = std::fabs(gradient_[i]);
                    }

                }
                int error = cs_cholsol(0, hessian, gradient_.data(), this->S_outer);


                cs_free(hessian);
                std::cout << "  Newton raphson " << iter << ", inner maxgc = " << this->inner_maxgc << std::endl;
                if (this->inner_maxgc <= tol) {
                    return true;
                }



                for (int j = 0; j < random_variables_m.size(); j++) {
                    random_variables_m[j]->SetValue(random_variables_m[j]->GetValue() - gradient_[j]);
                }
            }


            return false;

        }

        bool line_search(std::vector<atl::Variable<T>* >& parameters,
                T& function_value,
                std::valarray<T>& x,
                std::valarray<T>& best,
                std::valarray<T>& z,
                std::valarray<T>& gradient,
                std::valarray<T>& wg,
                T& maxgc, int& i,
                bool inner = true) {
            T descent = 0;

            int nops = parameters.size();
            std::valarray<T> nwg(nops);
            std::valarray<T> ng(nops);

            for (size_t j = 0; j < nops; j++) {
                descent += z[j] * wg[j];
            }//end for

            T norm_g = this->norm(gradient);
            T relative_tolerance = this->tolerance * std::max<T > (T(1.0), norm_g);

            descent *= T(-1.0); // * Dot(z, g);
            if ((descent > T(-0.00000001) * relative_tolerance /* tolerance relative_tolerance*/)) {
                z = wg + .001;
                if (!inner) {
                    this->max_iterations -= i;
                    i = 0;
                }
                descent = -1.0 * Dot(z, wg);
            }//end if

            T step = i ? 1.0 : (1.0 / norm_g);

            if (step != step) {
                step = 1.0;
            }

            bool down = false;

            int ls;




            for (int j = 0; j < parameters.size(); j++) {
                best[j] = parameters[j]->GetValue();
            }

            atl::Variable<T> fx;
            for (ls = 0; ls < this->max_line_searches; ++ls) {



                // Tentative solution, gradient and loss
                std::valarray<T> nx = x - step * z;

                for (size_t j = 0; j < nops; j++) {

                    if (nx[j] != nx[j]) {
                    }
                    parameters[j]->UpdateValue(nx[j]);
                }

                //line_search:
                atl::Variable<T>::SetRecording(false);
                if (inner) {
                    this->CallInnerObjectiveFunction(fx);
                } else {
                    this->CallObjectiveFunction(fx);
                }
                //                atl::Variable<T>::tape.Reset();
                if (fx.GetValue() <= function_value + tolerance * T(10e-4) * step * descent) { // First Wolfe condition

                    for (size_t j = 0; j < nops; j++) {
                        best[j] = parameters[j]->GetInternalValue();
                    }


                    atl::Variable<T>::SetRecording(true);
                    atl::Variable<T>::tape.Reset();
                    atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
                    if (inner) {
                        this->CallInnerObjectiveFunction(fx);
                    } else {
                        this->CallObjectiveFunction(fx);
                    }

                    this->ComputeGradient(parameters, ng, maxgc);
                    atl::Variable<T>::tape.Reset();
                    if (down || (-1.0 * Dot(z, nwg) >= 0.9 * descent)) { // Second Wolfe condition
                        x = nx;
                        gradient = ng;
                        function_value = fx.GetValue();
                        return true;
                    } else {
                        atl::Variable<T>::SetRecording(false);
                        step *= 10.0; //2.0; //10.0;
                    }
                } else {
                    step /= 10.0; //*= .5; ///
                    down = true;
                }
            }

            for (size_t j = 0; j < nops; j++) {
                parameters[j]->SetValue(best[j]);
            }

            return false;

        }

        /**
         * Compute the Norm of the vector v.
         *  
         * @param v
         * @return 
         */
        const T norm(std::valarray<T> &v) {

            T ret = (T) 0.0;
            unsigned int i;
            for (i = 0; i < v.size(); i++) {

                ret += v[i] * v[i];

            }
            return std::sqrt(ret);
        }

        /**
         * Compute the dot product of two vectors.
         * @param a
         * @param b
         * @return 
         */
        const T Dot(const std::valarray<T> &a, const std::valarray<T> &b) {
            T ret = 0;
            for (size_t i = 0; i < a.size(); i++) {

                ret += a[i] * b[i];
            }
            return ret;
        }

        /**
         * returns the a column of a matrix as a std::valarray.
         * @param matrix
         * @param column
         * @return 
         */
        const std::valarray<T> Column(std::valarray<std::valarray<T> > &matrix, size_t column, size_t length) {

            std::valarray<T> ret(length);

            for (int i = 0; i < ret.size(); i++) {

                ret[i] = matrix[i][column];
            }
            return ret;
        }

    };

    template<typename T>
    class LBFGS : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
            atl::Variable<T>::SetRecording(true);
            atl::Variable<T>::tape.Reset();
            int nops = this->parameters_m.size();
            bool has_random_effects = false;
            if (this->random_variables_m.size()) {
                has_random_effects = true;
            }

            atl::Variable<T> pen;

            this->x.resize(nops);
            this->best.resize(nops);
            this->gradient.resize(nops);

            for (int i = 0; i < nops; i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    this->x[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    this->x[i] = this->parameters_m[i]->GetValue();
                }
                this->gradient[i] = 0;
            }
            //
            //
            std::valarray<T> wg(nops);
            std::valarray<T> nwg(nops);
            std::valarray<T> ng(nops);


            //initial evaluation
            atl::Variable<T> fx(0.0);
            this->CallObjectiveFunction(fx);
            this->function_value = fx.GetValue();
            //
            //Historical evaluations
            std::valarray<T> px(nops);
            std::valarray<T> pg(nops);
            std::valarray<std::valarray<T> > dxs(std::valarray<T > (this->max_history), nops);
            std::valarray<std::valarray<T> > dgs(std::valarray<T > (this->max_history), nops);
            //search direction
            std::valarray<T> z(nops);

            this->ComputeGradient(this->parameters_m, this->gradient, this->maxgc);

            atl::Variable<T>::tape.Reset();



            std::valarray<T> p(this->max_history);
            std::valarray<T>a(this->max_history);

            int i;
            for (int iteration = 0; iteration < this->max_iterations; iteration++) {
                i = iteration;
                this->outer_iteration = iteration;
                for (int j = 0; j < nops; j++) {
                    wg[j] = this->parameters_m[j]->GetScaledGradient(this->parameters_m[j]->GetInternalValue()) * this->gradient[j];
                }

                if ((i % 10) == 0 || i == 0) {
                    std::cout << "Iteration " << i << "\n";
                    std::cout << "Phase = " << this->phase_m << "\n";

                    this->Print();
                }

                if (this->maxgc < this->tolerance) {
                    std::cout << "Iteration " << i << "\n";
                    std::cout << "Phase = " << this->phase_m << "\n";

                    this->Print();
                    return true;
                }

                z = wg;

                if (i > 0 && this->max_history > 0) {

                    size_t h = std::min<size_t > (i, this->max_history);
                    size_t end = (i - 1) % h;

                    //update histories
                    for (size_t r = 0; r < nops; r++) {
                        dxs[r][end] = this->parameters_m[r]->GetInternalValue() - px[r];
                        dgs[r][end] = wg[r] - pg[r];
                    }



                    for (size_t j = 0; j < h; ++j) {
                        const size_t k = (end - j + h) % h;
                        p[k] = 1.0 / this->Dot(this->Column(dxs, k, this->parameters_m.size()), this->Column(dgs, k, this->parameters_m.size()));

                        a[k] = p[k] * this->Dot(this->Column(dxs, k, this->parameters_m.size()), z);
                        z -= a[k] * this->Column(dgs, k, this->parameters_m.size());
                    }
                    // Scaling of initial Hessian (identity matrix)
                    z *= this->Dot(this->Column(dxs, end, this->parameters_m.size()), this->Column(dgs, end, this->parameters_m.size())) / this->Dot(this->Column(dgs, end, this->parameters_m.size()), Column(dgs, end, this->parameters_m.size()));

                    for (size_t j = 0; j < h; ++j) {
                        const size_t k = (end + j + 1) % h;
                        const T b = p[k] * Dot(this->Column(dgs, k, this->parameters_m.size()), z);
                        z += this->Column(dxs, k, this->parameters_m.size()) * (a[k] - b);
                    }

                }//end if(i>0)

                for (size_t j = 0; j < nops; j++) {
                    px[j] = this->parameters_m[j]->GetInternalValue();
                    this->x[j] = px[j];
                    pg[j] = wg[j];


                }//end for




                if (!this->line_search(this->parameters_m,
                        this->function_value,
                        this->x,
                        this->best,
                        z,
                        this->gradient,
                        wg,
                        this->maxgc,
                        iteration, false)) {
                    std::cout << "Outer Max line searches (" << this->max_line_searches << ").";
                    return false;

                }

            }

            std::cout << "Outer Max iterations!";

            return false;
        }
    private:

        /**
         * Compute the dot product of two vectors.
         * @param a
         * @param b
         * @return 
         */
        const T Dot(const std::valarray<T> &a, const std::valarray<T> &b) {
            T ret = 0;
            for (size_t i = 0; i < a.size(); i++) {

                ret += a[i] * b[i];
            }
            return ret;
        }

        /**
         * returns the a column of a matrix as a std::valarray.
         * @param matrix
         * @param column
         * @return 
         */
        const std::valarray<T> Column(std::valarray<std::valarray<T> > &matrix, size_t column, size_t length) {

            std::valarray<T> ret(length);

            for (int i = 0; i < ret.size(); i++) {

                ret[i] = matrix[i][column];
            }
            return ret;
        }


    };

    template<typename T>
    class PortMinimizer : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            port::integer n = this->parameters_m.size();
            std::vector<T> g(n, 0.0);
            std::vector<T> d(n, 0.0);
            std::vector<T> x(n, 0.0);
            std::vector<T> b(n * 2, 0.0);
            port::integer lv = 71 + n * (n + 13) / 2;
            std::vector<T> v(lv, 0.0);
            port::integer liv = 60 + n;
            std::vector<port::integer>iv(liv, 0);
            v[0] = 2;
            std::valarray<T> z(n);
            std::valarray<T> wg(n);
            this->best.resize(n);
            this->x.resize(n);
            for (int i = 0; i < n; i++) {
                d[i] = 1.0;
                x[i] = this->parameters_m[i]->GetInternalValue();
                this->x[i] = x[i];
                if (this->parameters_m[i]->GetMinBoundary() != std::numeric_limits<T>::min()) {
                    b[2 * i] = this->parameters_m[i]->GetMinBoundary();
                } else {
                    b[2 * i] = this->parameters_m[i]->GetMinBoundary() + 1.0;
                }
                if (this->parameters_m[i]->GetMaxBoundary() != std::numeric_limits<T>::max()) {
                    b[2 * i + 1] = this->parameters_m[i]->GetMaxBoundary();
                } else {
                    b[2 * i + 1] = this->parameters_m[i]->GetMaxBoundary() - 1.0;
                }
            }

            atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;

            atl::Variable<T>::SetRecording(true);
            T fx = 0.0;
            atl::Variable<T> f;
            this->CallObjectiveFunction(f);
            fx = f.GetValue();
            this->function_value = f.GetValue();
            this->ComputeGradient(this->parameters_m, this->gradient, this->maxgc);
            for (int i = 0; i < n; i++) {
                g[i] = this->parameters_m[i]->GetScaledGradient(this->parameters_m[i]->GetInternalValue()) * this->gradient[i];
                wg[i] = g[i];
            }
            atl::Variable<T>::tape.Reset();

            int iter = 0;
            this->outer_iteration = iter;
            T maxgc;

            port::ivset_(2, iv.data(), iv.size(), v.size(), v.data());

            T previous_function_value;
            do {



                port::drmng_<T>(/*b.data(),*/ d.data(), &fx, g.data(), iv.data(), &liv, &lv, &n, v.data(), x.data());



                if ((iv[0]) == 2) {
                    iter++;
                    this->outer_iteration = iter;
                    atl::Variable<T>::tape.Reset();
                    for (int i = 0; i < n; i++) {
                        this->parameters_m[i]->UpdateValue(x[i]);
                        this->x[i] = x[i];
                    }

                    atl::Variable<T>::SetRecording(true);
                    //                    atl::Variable<T>::tape.Reset()
                    previous_function_value = f.GetValue();
                    this->CallObjectiveFunction(f);
                    fx = f.GetValue();
                    this->function_value = f.GetValue();
                    this->ComputeGradient(this->parameters_m, this->gradient, this->maxgc);
                    for (int i = 0; i < n; i++) {
                        g[i] = this->parameters_m[i]->GetScaledGradient(this->parameters_m[i]->GetInternalValue()) * this->gradient[i];
                        wg[i] = g[i];
                    }
                    z = wg;

                    maxgc = std::fabs(g[0]); // std::numeric_limits<T>::min();
                    for (int i = 0; i < n; i++) {
                        if (std::fabs(g[i]) > maxgc) {
                            maxgc = std::fabs(g[i]);
                        }

                    }
                    this->maxgc = maxgc;

                    //                                        if ((std::fabs(previous_function_value) - std::fabs(fx)) < 1e-15) {
                    //                                            std::cout << "Line searching....\n";
                    //                                            if (!this->line_search(this->parameters_m,
                    //                                                    this->function_value,
                    //                                                    this->x,
                    //                                                    this->best,
                    //                                                    z,
                    //                                                    this->gradient,
                    //                                                    wg,
                    //                                                    this->maxgc,
                    //                                                    iter, false)) {
                    //                                                std::cout << "Outer Max line searches (" << this->max_line_searches << ").";
                    //                                                for (int i = 0; i < n; i++) {
                    //                                                    this->x[i] = this->best[i];
                    //                                                }
                    //                    
                    //                                            }
                    //                                        }

                    for (int i = 0; i < n; i++) {
                        if (std::fabs(g[i]) > maxgc) {
                            maxgc = std::fabs(g[i]);
                        }

                    }
                    this->maxgc = maxgc;

                    for (int i = 0; i < n; i++) {
                        this->parameters_m[i]->UpdateValue(this->x[i]);
                        x[i] = this->x[i];
                        g[i] = this->gradient[i];
                    }

                    if (iter == 1 || (iter % 10) == 0) {
                        this->Print();
                    }
                } else {

                    for (int i = 0; i < n; i++) {
                        this->parameters_m[i]->UpdateValue(x[i]);
                        this->x[i] = x[i];
                    }



                    atl::Variable<T>::SetRecording(false);
                    previous_function_value = f.GetValue();
                    this->CallObjectiveFunction(f);
                    fx = f.GetValue();
                    this->function_value = f.GetValue();

                    if (fx != fx) {
                        std::cout << "Objective Function signaling NaN";
                    }
                }


                if (this->maxgc <= this->tolerance) {
                    break;
                }


            } while ((iv[0]) < 3);
            for (int i = 0; i < n; i++) {
                this->parameters_m[i]->UpdateValue(x[i]);
                this->x[i] = x[i];
            }

            atl::Variable<T>::SetRecording(true);
            atl::Variable<T>::tape.Reset();
            this->CallObjectiveFunction(f);
            fx = f.GetValue();
            this->function_value = f.GetValue();
            this->ComputeGradient(this->parameters_m, this->gradient, this->maxgc);
            this->Print();

            std::cout << "port return =" << iv[0] << "\n";
            if (this->maxgc <= this->tolerance) {
                return true;
            }
            return false;
        }

    };

    /**
     * Undefined behavior for bounded  problems.
     */
    template<typename T>
    class NewtonMinimizer : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {


            if (this->random_variables_m.size() > 0) {
                std::cout << "NewtonMinimizer not available for mixed effects models.\n";
                return false;
            }

            this->gradient.resize(this->parameters_m.size());
            this->hessian_pattern_map.resize(this->parameters_m.size());
            atl::Variable<T> fx;

            int nops = this->parameters_m.size();

            std::vector<T> gradient_(nops);

            T lambda = 0.1;


            T errorTolerance = 0.0;




            T error = std::numeric_limits<T>::max();
            T error_change = std::numeric_limits<T>::max();
            T err;
            T previous;


            struct cs_symbolic<T>* S = NULL;
            for (int iter = 0; iter < this->max_iterations; iter++) {

                this->outer_iteration = iter;

                atl::Variable<T>::tape.Reset();
                atl::Variable<T>::SetRecording(true);
                atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
                //                fx = this->objective_function_m->Evaluate();
                fx = 0.0;
                this->objective_function_m->Objective_Function(fx);
                this->function_value = fx.GetValue();

                if (iter) {

                    err = (this->function_value - previous) / previous;

                    if (err < error) {
                        error_change = error - err;
                        lambda *= T(.5); // T(10.0);
                    } else {
                        error_change = err - error;
                        lambda *= T(2.0); //T(10.0);
                    }
                    error = err;
                    previous = this->function_value;

                }


                atl::Variable<T>::tape.AccumulateSecondOrder();
                struct cs_sparse<T>* RHessian = cs_spalloc<T>(0, 0, 1, 1, 1);

                this->maxgc = std::numeric_limits<T>::min();

                if (!this->pattern_known) {

                    for (int i = 0; i <this->parameters_m.size(); i++) {
                        gradient_[i] = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id);
                        this->gradient[i] = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id);
                        if (this->parameters_m[i]->bounded_m) {
                            gradient_[i] = this->parameters_m[i]->GetScaledGradient(this->parameters_m[i]->GetInternalValue()) * this->gradient[i];
                        } else {
                            gradient_[i] = this->gradient[i];
                        }
                        this->maxgc = std::max(this->maxgc, std::fabs(this->gradient[i]));
                        int jj = 0;
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j <this->parameters_m.size(); j++) {

                            T dxx = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id, this->parameters_m[j]->info->id);

                            if (dxx != static_cast<T> (0.0)) {
                                hm.push_back(j);
                                cs_entry(RHessian, i, j, dxx);
                            }
                        }
                    }
                    this->pattern_known = true;
                } else {

                    for (int i = 0; i <this->parameters_m.size(); i++) {
                        this->gradient[i] = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id);
                        if (this->parameters_m[i]->bounded_m) {
                            gradient_[i] = this->parameters_m[i]->GetScaledGradient(this->parameters_m[i]->GetInternalValue()) * this->gradient[i];
                        } else {
                            gradient_[i] = this->gradient[i];
                        }

                        this->maxgc = std::max(this->maxgc, std::fabs(this->gradient[i]));
                        std::vector<int>& hm = this->hessian_pattern_map[i];
                        for (int j = 0; j < hm.size(); j++) {
                            T dxx = atl::Variable<T>::tape.Value(this->parameters_m[i]->info->id, this->parameters_m[hm[j]]->info->id);
                            cs_entry(RHessian, i, hm[j], dxx);
                        }
                    }
                }



                struct cs_sparse<T> *hessian = cs_compress<T>(RHessian);
                cs_spfree(RHessian);
                if (this->S_outer == NULL) {
                    this->S_outer = cs_schol<T>(0, hessian);
                }

                //                this->this->maxgc = std::fabs(this->gradient[0]);
                //                for (int i = 0; i < gradient_.size(); i++) {
                //                    if (std::fabs(this->gradient[i]) > this->this->maxgc) {
                //                        this->maxgc = std::fabs(this->gradient[i]);
                //                    }
                //
                //                }
                int error = cs_cholsol(0, hessian, gradient_.data(), this->S_outer);


                cs_free(hessian);

                if (this->maxgc <= this->tolerance) {
                    return true;
                }
                if (iter == 1 || (iter % 10) == 0) {
                    this->Print();
                }


                for (int j = 0; j < this->parameters_m.size(); j++) {
                    this->parameters_m[j]->UpdateValue(this->parameters_m[j]->GetInternalValue() - lambda * gradient_[j]);
                }
            }


            return false;
        }

    };




#ifdef ATL_HAS_EIGEN 

    template<typename T>
    class Objective_Function2 : public cppoptlib::Problem<T> {
    public:
        using typename cppoptlib::Problem<T>::TVector; // Inherit the Vector typedef
        using typename cppoptlib::Problem<T>::THessian;
        atl::OptimizationRoutine<T>* OR;

        int iprint = 10;
        T maxgc;
        atl::Variable<T> f;

        // this is just the objective (NOT optional)

        T value(const TVector &x) {
            f = static_cast<T> (0.0);
            atl::Variable<T>::tape.recording = false;
            for (int i = 0; i < x.size(); i++) {
                this->OR->parameters_m[i]->UpdateValue(x[i]);
                this->OR->x[i] = this->OR->parameters_m[i]->GetValue();
            }
            OR->CallObjectiveFunction(f);
            OR->function_value = f.GetValue();
            return f.GetValue();
        }

        // if you calculated the derivative by hand
        // you can implement it here (OPTIONAL)
        // otherwise it will fall back to (bad) numerical finite differences

        void gradient(const TVector &x, TVector &grad) {
            f = static_cast<T> (0.0);
            atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
            atl::Variable<T>::tape.recording = true;
            atl::Variable<T>::tape.Reset();
            for (int i = 0; i < x.size(); i++) {
                this->OR->parameters_m[i]->UpdateValue(x[i]);
                this->OR->x[i] = this->OR->parameters_m[i]->GetValue();
            }
            OR->CallObjectiveFunction(f);
            OR->function_value = f.GetValue();
            //            atl::Variable<T>.tape.AccumulateFirstOrder();
            std::valarray<T> ng(x.size());
            OR->ComputeGradient(OR->parameters_m, ng, OR->maxgc);
            for (int i = 0; i < x.size(); i++) {
                grad[i] = OR->parameters_m[i]->GetScaledGradient(OR->parameters_m[i]->GetInternalValue()) * ng[i];
                OR->gradient[i] = ng[i];
            }

        }

        void hessian(const TVector &x, THessian &hessian) {
            f = static_cast<T> (0.0);
            atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
            atl::Variable<T>::tape.recording = true;
            atl::Variable<T>::tape.Reset();
            for (int i = 0; i < x.size(); i++) {
                this->OR->parameters_m[i]->UpdateValue(x[i]);
                this->OR->x[i] = this->OR->parameters_m[i]->GetValue();
            }
            OR->CallObjectiveFunction(f);
            OR->function_value = f.GetValue();
            atl::Variable<T>::tape.AccumulateSecondOrder();


            for (int i = 0; i < this->OR->parameters_m.size(); i++) {
                for (int j = 0; j < this->OR->parameters_m.size(); j++) {

                    T dxx = atl::Variable<T>::tape.Value(this->OR->parameters_m[i]->info->id,
                            this->OR->parameters_m[j]->info->id);
                    hessian(i, j) = dxx;

                }
            }

        }

        bool callback(const cppoptlib::Criteria<T> &state, const TVector & x) {
            this->OR->outer_iteration = state.iterations - 1;
            if (((state.iterations - 1) % 10) == 0 || (state.iterations - 1) == 1) {
                this->OR->Print();
            }

            return true;
        }
    };

    template<typename T>
    class LBFGS2 : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            cppoptlib::LbfgsSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };

    template<typename T>
    class BFGS : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            cppoptlib::BfgsSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };

    template<typename T>
    class GradientDescent : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            cppoptlib::GradientDescentSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };

    template<typename T>
    class ConjugatedGradientDescent : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            cppoptlib::ConjugatedGradientDescentSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };

    template<typename T>
    class NelderMead : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            crit.iterations = 1000;
            cppoptlib::NelderMeadSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };

    template<typename T>
    class NewtonDescent : public atl::OptimizationRoutine<T> {
    public:

        virtual bool Evaluate() {
            this->gradient.resize(this->parameters_m.size());
            this->x.resize(this->parameters_m.size());
            cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
            cppoptlib::NewtonDescentSolver<Objective_Function2<T> > min;
            min.setStopCriteria(crit);
            Objective_Function2<T> of;
            of.OR = this;
            //            of.iprint = iprint;
            typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

            for (int i = 0; i < this->parameters_m.size(); i++) {
                if (this->parameters_m[i]->IsBounded()) {
                    xx[i] = this->parameters_m[i]->GetInternalValue();
                } else {
                    xx[i] = this->parameters_m[i]->GetValue();
                }
                this->x[i] = this->parameters_m[i]->GetValue();
            }
            min.minimize(of, xx);
        }

    };
    /*
        //Covariance Matrix Adaptation
        template<typename T>
        class CovarianceMatrixAdaptation : public atl::OptimizationRoutine<T> {
        public:

            virtual bool Evaluate() {
                this->gradient.resize(this->parameters_m.size());
                this->x.resize(this->parameters_m.size());
                cppoptlib::Criteria<T> crit = cppoptlib::Criteria<double>::defaults();
                cppoptlib::CMAesSolver<Objective_Function2<T> > min;
                min.setStopCriteria(crit);
                Objective_Function2<T> of;
                of.OR = this;
                //            of.iprint = iprint;
                typename Objective_Function2<T>::TVector xx(this->parameters_m.size());

                for (int i = 0; i < this->parameters_m.size(); i++) {
                    if (this->parameters_m[i]->IsBounded()) {
                        xx[i] = this->parameters_m[i]->GetInternalValue();
                    } else {
                        xx[i] = this->parameters_m[i]->GetValue();
                    }
                    this->x[i] = this->parameters_m[i]->GetValue();
                }
                min.minimize(of, xx);
            }

        };
     */
#endif

    template<class T>
    class MCMC {
        ObjectiveFunction<T>* objective_function_m = NULL;
        std::vector<atl::Variable<T>* > parameters_m;
        std::vector<atl::Variable<T>* > random_variables_m;
        std::vector<atl::Variable<T>* > all_variables_m;
    public:

        MCMC(ObjectiveFunction<T>* objective_function = NULL) :
        objective_function_m(objective_function) {
        }

        ObjectiveFunction<T>* GetObjectiveFunction() const {

            return objective_function_m;
        }

        void SetObjectiveFunction(ObjectiveFunction<T>* objective_function) {

            this->objective_function_m = objective_function;
        }

        bool Run() {
            if (this->objective_function_m == NULL) {
                return false;
            }
            bool success = false;
            for (int i = 1; i <= this->objective_function_m->max_phase_m; i++) {

                this->Prepare(i);
                success = this->MetropoliHastings();
            }
            return success;
        }

        const MinimizerResults<T> GetResults() {

        }

    private:

        void Prepare(int phase) {
            this->parameters_m.resize(0);
            this->random_variables_m.resize(0);

            for (int i = 0; i < this->objective_function_m->parameter_phases_m.size(); i++) {
                if (this->objective_function_m->parameter_phases_m[i] <= phase) {
                    this->parameters_m.push_back(this->objective_function_m->parameters_m[i]);
                    this->all_variables_m.push_back(this->objective_function_m->parameters_m[i]);
                }
            }

            for (int i = 0; i < this->objective_function_m->random_variable_phases_m.size(); i++) {
                if (this->objective_function_m->random_variable_phases_m[i] <= phase) {

                    this->parameters_m.push_back(this->objective_function_m->random_variables_m[i]);
                    this->all_variables_m.push_back(this->objective_function_m->random_variables_m[i]);
                }
            }

        }

        bool MetropoliHastings() {
            //minimizer wrt parameters
        }

        bool LBGFS_Inner() {
            //minimize wrt random variables
        }

    };



}


#endif /* FUNCTIONMINIMIZER_HPP */

