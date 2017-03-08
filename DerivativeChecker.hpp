/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DerivativeChecker.hpp
 * Author: matthewsupernaw
 *
 * Created on March 3, 2016, 4:12 PM
 */

#ifndef DERIVATIVECHECKER_HPP
#define DERIVATIVECHECKER_HPP

#define HESSIAN_USE_AD_GRADIENT

#include <chrono>

#include "Variable.hpp"

namespace atl {

    template<class T>
    class DerivativeChecker {
        std::vector<atl::Variable<T>* > check_parameters_m;
        static T tolerance;
        static T second_tolerance;
        static T third_tolerance;
    public:

        
        virtual void Objective_Function(atl::Variable<T>& f){
            
        }
        
        virtual void Initialize() {

        }

        void RegisterCheckParameter(atl::Variable<T>& p) {
            this->check_parameters_m.push_back(&p);
        }

        void Check() {
            atl::Variable<T>::tape.Reset();
            atl::Variable<T>::SetRecording(true);
            atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
//            this->Initialize();
            atl::Variable<T> f;
            //
            T hmin;
            T hmax;
            T gmin;
            T gmax;
            std::ofstream out;
            out.open("derivatives.txt");
            //                    //                    //

           
            T fval;

            std::vector<T> gradient(this->check_parameters_m.size());

            out << "Number of Parameters: " << this->check_parameters_m.size() << "\n";


            std::cout << "evaluating..." << std::flush;
this->Objective_Function(f);

            atl::Variable<T>::tape.Reset();
            auto eval_gstart = std::chrono::steady_clock::now();
            this->Objective_Function(f);
            auto eval_gend = std::chrono::steady_clock::now();
            std::chrono::duration<double> eval_gtime = eval_gend - eval_gstart;

            fval = f.GetValue();

            std::cout << "computing exact gradient..." << std::flush;
            auto exact_gstart = std::chrono::steady_clock::now();
            atl::Variable<T>::tape.AccumulateFirstOrder();
            auto exact_gend = std::chrono::steady_clock::now();
            std::chrono::duration<double> exact_gtime = (exact_gend - exact_gstart);

            for (int i = 0; i < this->check_parameters_m.size(); i++) {
                gradient[i] = this->check_parameters_m[i]->info->dvalue;
                std::cout<<gradient[i] <<" ";
            }
            std::cout<<"\n";

            atl::Variable<T>::tape.Reset();
            std::cout << "done!\nevaluating..." << std::flush;

            f = 0.0;
            atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;

            //                    //run hessian twice to make sure everything resets properly
            //                    this->Objective_Function(f);
            //                    atl::Variable<T>::tape.HessianAndGradientAccumulate();
            //                    atl::Variable<T>::tape.Reset();

            f = 0.0;
            auto eval_start = std::chrono::steady_clock::now();
            this->Objective_Function(f);
            auto eval_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> eval_time = eval_end - eval_start;

            fval = f.GetValue();

            std::cout << "computing exact hessian..." << std::flush;
            auto exact_start = std::chrono::steady_clock::now();
            atl::Variable<T>::tape.AccumulateSecondOrderMixed();
            auto exact_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> exact_time = (exact_end - exact_start);
            std::vector<std::vector<T> > exact_hessian(this->check_parameters_m.size(), std::vector<T> (this->check_parameters_m.size()));
            std::vector<std::vector<T> > exact_hessian2(this->check_parameters_m.size(), std::vector<T> (this->check_parameters_m.size()));
            for (int i = 0; i < this->check_parameters_m.size(); i++) {
                //                        gradient[i] = this->check_parameters_m[i]->info->dvalue;
                for (int j = 0; j < this->check_parameters_m.size(); j++) {
#ifdef USE_EIGEN

                    exact_hessian[i][j] = atl::Variable<T>::tape.Value(this->check_parameters_m[i]->info->id, this->check_parameters_m[j]->info->id); //this->check_parameters_m[i]->info->hessian_row(this->check_parameters_m[j]->info->id);
#else
                    exact_hessian[i][j] = atl::Variable<T>::tape.Value(this->check_parameters_m[i]->info->id, this->check_parameters_m[j]->info->id); //this->check_parameters_m[i]->info->hessian_row(this->check_parameters_m[j]->info->id);this->check_parameters_m[i]->info->hessian_row[this->check_parameters_m[j]->info->id];
#endif
                }
            }
            std::cout << "done!\n";
            std::cout << std::scientific;

            atl::Variable<T>::tape.Reset();
            std::cout << "estimating hessian..." << std::flush;
            auto estimated_start = std::chrono::steady_clock::now();
            std::valarray<std::valarray<T> > estimated_hessian = this->EstimatedHessian();
            auto estimated_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> estimated_time = (estimated_end - estimated_start);

            std::cout << "done\n";
            T sum = 0;

            for (int i = 0; i < exact_hessian.size(); i++) {
                for (int j = 0; j < exact_hessian[0].size(); j++) {
                    T diff = (exact_hessian[i][j] - estimated_hessian[i][j]);
                    if (i == 0 && j == 0) {
                        hmax = diff;
                        hmin = diff;
                    } else {
                        if (diff > hmax) {
                            hmax = diff;
                        }
                        if (diff < hmin) {
                            hmin = diff;
                        }
                    }

                    sum += (exact_hessian[i][j] - estimated_hessian[i][j])* (exact_hessian[i][j] - estimated_hessian[i][j]);
                }
            }

            T mse = sum / (exact_hessian.size() * exact_hessian.size());

            atl::Variable<T>::tape.Reset();
            auto estimatedg_start = std::chrono::steady_clock::now();
            std::valarray<T> estimated_gradient = this->EstimateGradient();
            auto estimatedg_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> estimatedg_time = (estimatedg_end - estimatedg_start);

            T gsum = 0;
            for (int i = 0; i < gradient.size(); i++) {
                T diff = (gradient[i] - estimated_gradient[i]);
                if (i == 0) {
                    gmax = diff;
                    gmin = diff;
                } else {
                    if (diff > gmax) {
                        gmax = diff;
                    }
                    if (diff < gmin) {
                        gmin = diff;
                    }
                }

                gsum += (gradient[i] - estimated_gradient[i])*(gradient[i] - estimated_gradient[i]);
            }
            T gmse = gsum / gradient.size();




            atl::Variable<T> tf;
            atl::Variable<T>::tape.recording = true;
            atl::Variable<T>::tape.derivative_trace_level = atl::THIRD_ORDER_REVERSE;
            atl::Variable<T>::tape.Reset();
            std::cout << "evaluating..." << std::flush;

            auto eval_to_start = std::chrono::steady_clock::now();
            this->Objective_Function(tf);
            auto eval_to_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> eval_to_time = (eval_to_end - eval_to_start);

            std::cout << "computing exact third order mixed..." << std::flush;

            auto exact_to_start = std::chrono::steady_clock::now();
            atl::Variable<T>::tape.AccumulateThirdOrderMixed();
            auto exact_to_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> exact_to_time = (exact_to_end - exact_to_start);
            std::cout << "done.\n" << std::flush;

            std::valarray< std::valarray<std::valarray < T> > > third_mixed_exact(std::valarray<std::valarray < T> > (std::valarray<T > (check_parameters_m.size()), check_parameters_m.size()), check_parameters_m.size());

            for (int i = 0; i < this->check_parameters_m.size(); i++) {
                if (std::fabs(gradient[i] - this->check_parameters_m[i]->info->dvalue) > 1e-5) {
                    std::cout << "gradient doesn't match....\n";
                    std::cout << gradient[i] << " != " << this->check_parameters_m[i]->info->dvalue;
//                    exit(0);
                }
                for (int j = 0; j < this->check_parameters_m.size(); j++) {
                    for (int k = 0; k < this->check_parameters_m.size(); k++) {
                        if (i == 0) {
                            exact_hessian2[j][k] = atl::Variable<T>::tape.Value(this->check_parameters_m[j]->info->id, this->check_parameters_m[k]->info->id);
                            if (std::fabs(exact_hessian[j][k] - exact_hessian2[j][k]) > 1e-5) {

                                std::cout << "hessian doesn't match....\n";
                                std::cout << exact_hessian[j][k] << " != " << exact_hessian2[j][k];
                                //                                        exit(0);
                            }
                        }
                        third_mixed_exact[i][j][k] = atl::Variable<T>::tape.Value(this->check_parameters_m[i]->info->id, this->check_parameters_m[j]->info->id, this->check_parameters_m[k]->info->id);
                    }
                }
            }

            std::valarray< std::valarray<std::valarray < T> > > third_mixed_estimated(std::valarray<std::valarray < T> > (std::valarray<T > (check_parameters_m.size()), check_parameters_m.size()), check_parameters_m.size());
            std::cout << "estimating third order mixed..." << std::flush;

            auto estimated_to_start = std::chrono::steady_clock::now();
            third_mixed_estimated = this->EstimateThirdOrderMixed();
            auto estimated_to_end = std::chrono::steady_clock::now();
            std::chrono::duration<double> estimated_to_time = (estimated_to_end - estimated_to_start);
            std::cout << "done.\n" << std::flush;

            T to_sum = 0;
            for (int i = 0; i < this->check_parameters_m.size(); i++) {
                for (int j = 0; j < this->check_parameters_m.size(); j++) {
                    for (int k = 0; k < this->check_parameters_m.size(); k++) {
                        to_sum += third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k];
                        //                                if (std::fabs(third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k]) > 1e-2) {
                        //                                    std::cout << "{" << this->check_parameters_m[i]->info->id << "," << this->check_parameters_m[j]->info->id << "," << this->check_parameters_m[k]->info->id << "}" << "third order failed " << third_mixed_exact[i][j][k] << " != " << third_mixed_estimated[i][j][k] << " <-----\n";
                        //                                }
                        //                                    else {
                        //                                        std::cout << "{" << this->check_parameters_m[i]->info->id << "," << this->check_parameters_m[j]->info->id << "," << this->check_parameters_m[k]->info->id << "}" << "third order PASSED " << third_mixed_exact[i][j][k] << " == " << third_mixed_estimated[i][j][k] << "\n";
                        //                                    }
                    }
                }
            }

            T to_mse = to_sum / (exact_hessian.size() * exact_hessian.size() * exact_hessian.size());



            if (gmse <= DerivativeChecker<T>::tolerance) {
                std::cout << "Gradient Test Passed.\n";
                out << "Gradient  Test Passed.\n";
            } else {
                //fail++;
                std::cout << "Gradient  Test Failed(" << gmse << ">" << DerivativeChecker<T>::tolerance << ")\n";
                out << "Gradient Test Failed.\n";
            }

            if (mse <= DerivativeChecker<T>::second_tolerance) {
                std::cout << "Hessian Test Passed.\n";
                out << "Hessian  Test Passed.\n";
            } else {
                //fail++;
                std::cout << "Hessian  Test Failed(" << mse << ">" << DerivativeChecker<T>::tolerance << ")\n";
                out << "Hessian Test Failed.\n";
            }


            if (to_mse <= DerivativeChecker<T>::third_tolerance) {
                std::cout << "Third-Order Test Passed.\n";
                out << "Third-Order Test Passed.\n";
            } else {
                //fail++;
                std::cout << "Third-Order Test Failed(" << to_mse << ">" << DerivativeChecker<T>::tolerance << ")\n";
                out << "Third-Order Test Failed.\n";
            }

            out << "Function value: " << fval << "\n";
            out << "Gradient mse = " << gmse << ", Error Range{" << gmin << " - " << gmax << "}\n";
            out << "Hessian mse = " << mse << ", Error Range{" << hmin << " - " << hmax << "}\n";
            out << "Third-Order mse = " << to_mse << "\n"; // ", Error Range{" << hmin << " - " << hmax << "}\n";
            out << std::fixed;
            out << "Time to evaluate objective function with only gradient info: " << eval_gtime.count() << " sec\n";
            out << "Time to evaluate objective function with Hessian info: " << eval_time.count() << " sec\n";
            out << "Time to evaluate objective function with Third-Order info: " << eval_to_time.count() << " sec\n";
            out << "Time to compute exact gradient: " << exact_gtime.count() << " sec\n";
            out << "Time to compute exact gradient and Hessian: " << exact_time.count() << " sec\n";
            out << "Time to compute exact gradient, Hessian, and Third-Order: " << exact_to_time.count() << " sec\n";
            out << "Time to estimate gradient: " << estimatedg_time.count() << " sec\n";
            out << "Time to estimate Hessian: " << estimated_time.count() << " sec\n";
            out << "Time to estimate Third-Order: " << estimated_to_time.count() << " sec\n";
            out << "Gradient Speed up = " << estimatedg_time / exact_gtime << "\n";
            out << "Hessain Speed up = " << estimated_time / exact_time << "\n\n";
            out << "Third-Order Speed up = " << estimated_to_time / exact_to_time << "\n\n";
            std::cout << std::fixed;
            std::cout << "Gradient Speed up = " << estimatedg_time / exact_gtime << "\n";
            std::cout << "Hessain Speed up = " << estimated_time / exact_time << "\n";
            std::cout << "Third-Order Speed up = " << estimated_to_time / exact_to_time << "\n\n";

            out << "Estimated gradient:\n";
            for (int i = 0; i < gradient.size(); i++) {
                out << estimated_gradient[i] << " ";
            }
            out << "\n\n";

            out << "Exact gradient:\n";
            for (int i = 0; i < gradient.size(); i++) {
                out << gradient[i] << " ";
            }
            out << "\n\n";


            out << "Gradient difference:\n";
            for (int i = 0; i < gradient.size(); i++) {
                out << gradient[i] - estimated_gradient[i] << " ";
            }
            out << "\n\n";

            out << "Estimated Hessian: \n";
            for (int i = 0; i < estimated_hessian.size(); i++) {
                for (int j = 0; j < estimated_hessian[0].size(); j++) {
                    out << estimated_hessian[i][j] << " ";
                }
                out << std::endl;
            }


            //
            out << "\nExact Hessian:\n";
            for (int i = 0; i < exact_hessian.size(); i++) {
                for (int j = 0; j < exact_hessian[0].size(); j++) {
                    out << exact_hessian[i][j] << " ";
                }
                out << std::endl;

            }

            out << "\nHessian difference:\n";
            for (int i = 0; i < exact_hessian.size(); i++) {
                for (int j = 0; j < exact_hessian[0].size(); j++) {
                    T diff = (exact_hessian[i][j] - estimated_hessian[i][j]);
                    out << diff << " ";
                }
                out << std::endl;
            }

            out << "\nEstimated Third-Order:\n";
            for (int i = 0; i < this->check_parameters_m.size(); i++) {

                for (int j = 0; j < this->check_parameters_m.size(); j++) {
                    out << "[" << i << "] ";
                    for (int k = 0; k < this->check_parameters_m.size(); k++) {
                        out << third_mixed_estimated[i][j][k] << " ";
                    }
                    out << "\n";
                }
            }

            out << "\nExact Third-Order:\n";
            for (int i = 0; i < this->check_parameters_m.size(); i++) {

                for (int j = 0; j < this->check_parameters_m.size(); j++) {
                    out << "[" << i << "] ";
                    for (int k = 0; k < this->check_parameters_m.size(); k++) {
                        out << third_mixed_exact[i][j][k] << " ";
                    }
                    out << "\n";
                }
            }


            out << "\nThird-Order difference:\n";
            for (int i = 0; i < this->check_parameters_m.size(); i++) {

                for (int j = 0; j < this->check_parameters_m.size(); j++) {
                    out << "[" << i << "] ";
                    for (int k = 0; k < this->check_parameters_m.size(); k++) {
                        out << (third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k]) << " ";
                    }
                    out << "\n";
                }
            }


        }

        const std::valarray<T> EstimateGradient() {
            atl::Variable<T>::tape.Reset();
            atl::Variable<T>::SetRecording(false);
            std::valarray<T> gradient(check_parameters_m.size());
            atl::Variable<T> f;
            this->Objective_Function(f);
            atl::Variable<T> fh;
            T fv;
            T delta = 1.e-5;
            for (int i = 0; i < check_parameters_m.size(); i++) {
                check_parameters_m[i]->SetValue(check_parameters_m[i]->GetValue() + delta);
                this->Objective_Function(fh);
                fv = fh.GetValue();
                check_parameters_m[i]->SetValue(check_parameters_m[i]->GetValue() - 2 * delta);
                this->Objective_Function(fh);
                gradient[i] = (fv - fh.GetValue()) / (2.0 * delta);
                check_parameters_m[i]->SetValue(check_parameters_m[i]->GetValue() + delta);
            }
            return gradient;
        }

        /**
         * This function is a port of from admb. 
         * @return 
         */
        const std::valarray<std::valarray<T> > EstimatedHessian() {

            atl::Variable<T>::SetRecording(true);
            atl::Variable<T>::tape.derivative_trace_level = atl::FIRST_ORDER_REVERSE;
            atl::Variable<T>::tape.Reset();
            std::valarray<std::valarray<T> > hessian(
                    std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            T delta = 1.e-3;
            std::valarray < T> g1(check_parameters_m.size());
            std::valarray < T> g2(check_parameters_m.size());
            //            std::valarray < T> hess(check_parameters_m.size());
            std::valarray < T> hess1(check_parameters_m.size());
            std::valarray < T> hess2(check_parameters_m.size());
            T eps = .1;
            T sdelta1;
            T sdelta2;

            atl::Variable<T> f;

            atl::Variable<T>::tape.Reset();
            for (int i = 0; i < check_parameters_m.size(); i++) {



                T xsave = check_parameters_m[i]->GetValue();
                sdelta1 = /*check_parameters_m[i]->GetValue()*/ +delta;
                //                sdelta1 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                Objective_Function(f);
                atl::Variable<T>::tape.Accumulate();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    g1[j] = check_parameters_m[j]->info->dvalue;
                }

                atl::Variable<T>::tape.Reset();
#else
                g1 = this->EstimateGradient();
#endif
                f = 0.0;
                sdelta2 = /*check_parameters_m[i]->GetValue()*/ -delta;
                //                sdelta2 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                Objective_Function(f);
                atl::Variable<T>::tape.Accumulate();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    g2[j] = check_parameters_m[j]->info->dvalue;
                }
                atl::Variable<T>::tape.Reset();
#else
                g2 = this->EstimateGradient();
#endif
                check_parameters_m[i]->SetValue(xsave);

                hess1 = (g1 - g2) / (sdelta1 - sdelta2);


                sdelta1 = /*check_parameters_m[i]->GetValue() +*/ eps*delta;
                //                sdelta1 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                f = 0.0;
                Objective_Function(f);
                atl::Variable<T>::tape.Accumulate();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    g1[j] = check_parameters_m[j]->info->dvalue;
                }
                atl::Variable<T>::tape.Reset();
#else
                g1 = this->EstimateGradient();
#endif

                check_parameters_m[i]->SetValue(xsave - eps * delta);
                sdelta2 = /*check_parameters_m[i]->GetValue()*/ -eps*delta;
                //                sdelta2 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                f = 0.0;
                Objective_Function(f);
                atl::Variable<T>::tape.Accumulate();


                for (int j = 0; j < check_parameters_m.size(); j++) {
                    g2[j] = check_parameters_m[j]->info->dvalue;
                }
                atl::Variable<T>::tape.Reset();
#else
                g2 = this->EstimateGradient();
#endif
                check_parameters_m[i]->SetValue(xsave);


                T eps2 = eps*eps;
                hess2 = (g1 - g2) / (sdelta1 - sdelta2);
                hessian[i] = (eps2 * hess1 - hess2) / (eps2 - 1.);
            }
            atl::Variable<T>::tape.Reset();
            return hessian;

        }

        const std::valarray<std::valarray<std::valarray<T> > > EstimateThirdOrderMixed() {
            atl::Variable<T>::SetRecording(true);
            atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
            atl::Variable<T>::tape.Reset();

            std::valarray< std::valarray<std::valarray < T> > > third_mixed(std::valarray<std::valarray < T> > (std::valarray<T > (check_parameters_m.size()), check_parameters_m.size()), check_parameters_m.size());

            T delta = 1.e-3;
            T eps = .1;
            T sdelta1;
            T sdelta2;
            atl::Variable<T> f;

            atl::Variable<T>::tape.Reset();
            std::valarray < std::valarray < T> > h1(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> h2(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            //            std::valarray < T> hess(check_parameters_m.size());
            std::valarray < std::valarray < T >> t1(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> t2(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());

            std::valarray < std::valarray < T> > sd1(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> sd2(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T> > sd3(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> sd4(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> eps_squared(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            std::valarray < std::valarray < T >> eps_squared_minus_one(std::valarray<T > (check_parameters_m.size()), check_parameters_m.size());
            for (int i = 0; i < check_parameters_m.size(); i++) {
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    sd1[i][j] = delta;
                    sd2[i][j] = -1.0 * delta;
                    sd3[i][j] = eps*delta;
                    sd4[i][j] = -1.0 * eps*delta;
                    eps_squared[i][j] = eps*eps;
                    eps_squared_minus_one[i][j] = eps_squared[i][j] - 1.0;
                }
            }


            for (int i = 0; i < this->check_parameters_m.size(); i++) {


                T xsave = check_parameters_m[i]->GetValue();
                sdelta1 = /*check_parameters_m[i]->GetValue()*/ +delta;
                //                sdelta1 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                Objective_Function(f);
                atl::Variable<T>::tape.AccumulateSecondOrderMixed();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    for (int k = 0; k < check_parameters_m.size(); k++) {
                        h1[j][k] = atl::Variable<T>::tape.Value(check_parameters_m[j]->info->id, check_parameters_m[k]->info->id); //check_parameters_m[j]->info->hessian_row[check_parameters_m[k]->info->id];
                    }
                }

                atl::Variable<T>::tape.Reset();
#else
                h1 = this->EstimatedHessian();
#endif
                f = 0.0;
                sdelta2 = /*check_parameters_m[i]->GetValue()*/ -delta;
                //                sdelta2 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                Objective_Function(f);
                atl::Variable<T>::tape.AccumulateSecondOrderMixed();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    for (int k = 0; k < check_parameters_m.size(); k++) {
                        h2[j][k] = atl::Variable<T>::tape.Value(check_parameters_m[j]->info->id, check_parameters_m[k]->info->id); //check_parameters_m[j]->info->hessian_row[check_parameters_m[k]->info->id];
                    }
                }
                atl::Variable<T>::tape.Reset();
#else
                h2 = this->EstimatedHessian();
#endif
                check_parameters_m[i]->SetValue(xsave);


                t1 = (h1 - h2) / (sd1 - sd2);


                sdelta1 = /*check_parameters_m[i]->GetValue() +*/ eps*delta;
                //                sdelta1 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                f = 0.0;
                Objective_Function(f);
                atl::Variable<T>::tape.AccumulateSecondOrderMixed();
                for (int j = 0; j < check_parameters_m.size(); j++) {
                    for (int k = 0; k < check_parameters_m.size(); k++) {
                        h1[j][k] = atl::Variable<T>::tape.Value(check_parameters_m[j]->info->id, check_parameters_m[k]->info->id); //check_parameters_m[j]->info->hessian_row[check_parameters_m[k]->info->id];
                    }
                }
                atl::Variable<T>::tape.Reset();
#else
                h1 = this->EstimatedHessian();
#endif

                check_parameters_m[i]->SetValue(xsave - eps * delta);
                sdelta2 = /*check_parameters_m[i]->GetValue()*/ -eps*delta;
                //                sdelta2 -= check_parameters_m[i]->GetValue();
                check_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                f = 0.0;
                Objective_Function(f);
                atl::Variable<T>::tape.AccumulateSecondOrderMixed();

                for (int j = 0; j < check_parameters_m.size(); j++) {
                    for (int k = 0; k < check_parameters_m.size(); k++) {
                        h2[j][k] = atl::Variable<T>::tape.Value(check_parameters_m[j]->info->id, check_parameters_m[k]->info->id); //check_parameters_m[j]->info->hessian_row[check_parameters_m[k]->info->id];
                    }
                }
                atl::Variable<T>::tape.Reset();
#else
                h2 = this->EstimatedHessian();
#endif
                check_parameters_m[i]->SetValue(xsave);


                T eps2 = eps*eps;
                t2 = (h1 - h2) / (sd3 - sd4);
                third_mixed[i] = (eps_squared * t1 - t2) / (eps_squared_minus_one);
            }




            return third_mixed;

        }


    };
    template<typename T>
    T DerivativeChecker<T>::tolerance = 1e-4;
    template<typename T>
    T DerivativeChecker<T>::second_tolerance = 1e-4;
    template<typename T>
    T DerivativeChecker<T>::third_tolerance = 1e-4;
}


#endif /* DERIVATIVECHECKER_HPP */

