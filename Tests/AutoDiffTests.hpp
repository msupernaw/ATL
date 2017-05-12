/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   AutoDiffTests.hpp
 * Author: matthewsupernaw
 *
 * Created on January 24, 2017, 12:08 PM
 */

#ifndef AUTODIFFTESTS_HPP
#define AUTODIFFTESTS_HPP

#include <valarray>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>
#include "../ATL.hpp"
#include <cmath>
#define HESSIAN_USE_AD_GRADIENT

namespace atl {
    namespace tests {
        namespace auto_diff {
            int fail = 0;
            int tests = 0;

            template<class T>
            class AutoDiffTest {
                typedef atl::Variable<T> var;
                std::vector<var*> active_parameters_m;
                std::vector<atl::VariableInfo<T>* > independents;
                std::vector<uint32_t> pids;
                static T tolerance;
                static T second_tolerance;
                static T third_tolerance;
            public:

                const std::vector<var*>& GetActiveParameters() {
                    return this->active_parameters_m;
                }

                operator int() {
                    return fail;
                }

                AutoDiffTest() {
                }

                AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                static T GetTolerance() {
                    return tolerance;
                }

                static void SetTolerance(T tolerance) {
                    AutoDiffTest::tolerance = tolerance;
                }

                static T GetSecondOrderTolerance() {
                    return AutoDiffTest::second_tolerance;
                }

                static void SetSecondOrderTolerance(T tolerance) {
                    AutoDiffTest::second_tolerance = tolerance;
                }

                static T GetThirdOrderTolerance() {
                    return AutoDiffTest::third_tolerance;
                }

                static void SetThirdOrderTolerance(T tolerance) {
                    AutoDiffTest::third_tolerance = tolerance;
                }

                void Register(var& v) {
                    this->active_parameters_m.push_back(&v);
                    this->independents.push_back(v.info.get());
                    this->pids.push_back(v.info->id);
                }

                virtual void Initialize() {

                }

                virtual void ObjectiveFunction(atl::Variable<T>& f) {

                }

                virtual void Description(std::stringstream& out) {

                }

                void RunTestToFile(std::ostream& out) {

                    tests += 2;
                    var::tape.Reset();
                    var::tape.recording = (true);
                    var::tape.derivative_trace_level = FIRST_ORDER_REVERSE;
                    this->Initialize();
                    var f;
                    //
                    T hmin;
                    T hmax;
                    T gmin;
                    T gmax;
                    //                    //                    //
                    std::stringstream ss;
                    this->Description(ss);
                    out << "\n\n" << ss.str() << "\n";

                    std::cout << "\n\n" << ss.str() << "\n";
                    T fval;
                    //                std::cout.precision(50);
                    std::vector<T> gradient(this->active_parameters_m.size());

                    out << "Number of Parameters: " << this->active_parameters_m.size() << "\n";


                    std::cout << "evaluating..." << std::flush;


                    var::tape.Reset();
                    auto eval_gstart = std::chrono::steady_clock::now();
                    this->ObjectiveFunction(f);
                    auto eval_gend = std::chrono::steady_clock::now();
                    std::chrono::duration<double> eval_gtime = eval_gend - eval_gstart;

                    fval = f.GetValue();

                    std::cout << "computing exact gradient..." << std::flush;
                    auto exact_gstart = std::chrono::steady_clock::now();
                    var::tape.AccumulateFirstOrder();
                    auto exact_gend = std::chrono::steady_clock::now();
                    std::chrono::duration<double> exact_gtime = (exact_gend - exact_gstart);

                    for (int i = 0; i < this->active_parameters_m.size(); i++) {
                        gradient[i] = var::tape.Value(this->active_parameters_m[i]->info->id);
                    }


                    var::tape.Reset();
                    std::cout << "done!\nevaluating..." << std::flush;

                    f = 0.0;
                    var::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;

                    //                    //run hessian twice to make sure everything resets properly
                    //                    this->ObjectiveFunction(f);
                    //                    var::tape.HessianAndGradientAccumulate();
                    //                    var::tape.Reset();

                    f = 0.0;
                    auto eval_start = std::chrono::steady_clock::now();
                    this->ObjectiveFunction(f);
                    auto eval_end = std::chrono::steady_clock::now();
                    std::chrono::duration<double> eval_time = eval_end - eval_start;

                    fval = f.GetValue();

                    std::cout << "computing exact hessian..." << std::flush;
                    auto exact_start = std::chrono::steady_clock::now();
                    var::tape.AccumulateSecondOrder(); //AccumulateSecondOrderReverseOverForward(this->independents);//
                    auto exact_end = std::chrono::steady_clock::now();
                    std::chrono::duration<double> exact_time = (exact_end - exact_start);
                    std::vector<std::vector<T> > exact_hessian(this->active_parameters_m.size(), std::vector<T> (this->active_parameters_m.size()));
                    std::vector<std::vector<T> > exact_hessian2(this->active_parameters_m.size(), std::vector<T> (this->active_parameters_m.size()));
                    for (int i = 0; i < this->active_parameters_m.size(); i++) {
                        //                        gradient[i] = this->active_parameters_m[i]->info->dvalue;
                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
#ifdef USE_EIGEN

                            exact_hessian[i][j] = var::tape.Value(this->active_parameters_m[i]->info->id, this->active_parameters_m[j]->info->id); //this->active_parameters_m[i]->info->hessian_row(this->active_parameters_m[j]->info->id);
#else
                            exact_hessian[i][j] = var::tape.Value(this->active_parameters_m[i]->info->id, this->active_parameters_m[j]->info->id); //this->active_parameters_m[i]->info->hessian_row(this->active_parameters_m[j]->info->id);this->active_parameters_m[i]->info->hessian_row[this->active_parameters_m[j]->info->id];
#endif
                        }
                    }
                    std::cout << "done!\n";
                    std::cout << std::scientific;

                    var::tape.Reset();
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

                    var::tape.Reset();
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
                    this->ObjectiveFunction(tf);
                    auto eval_to_end = std::chrono::steady_clock::now();
                    std::chrono::duration<double> eval_to_time = (eval_to_end - eval_to_start);

                    std::cout << "computing exact third order mixed..." << std::flush;

                    auto exact_to_start = std::chrono::steady_clock::now();
                    atl::Variable<T>::tape.AccumulateThirdOrder();
                    auto exact_to_end = std::chrono::steady_clock::now();
                    std::chrono::duration<double> exact_to_time = (exact_to_end - exact_to_start);
                    std::cout << "done.\n" << std::flush;

                    std::valarray< std::valarray<std::valarray < T> > > third_mixed_exact(std::valarray<std::valarray < T> > (std::valarray<T > (active_parameters_m.size()), active_parameters_m.size()), active_parameters_m.size());

                    for (int j = 0; j < this->active_parameters_m.size(); j++) {
                        for (int k = 0; k < this->active_parameters_m.size(); k++) {

                            exact_hessian2[j][k] = atl::Variable<T>::tape.Value(this->active_parameters_m[j]->info->id, this->active_parameters_m[k]->info->id);

                            if (std::fabs(exact_hessian[j][k] - exact_hessian2[j][k]) > 1e-5) {

                                std::cout << "hessian doesn't match....\n";
                                std::cout << exact_hessian[j][k] << " != " << exact_hessian2[j][k];
                                //                                        exit(0);
                            }
                        }
                    }

                    for (int i = 0; i < this->active_parameters_m.size(); i++) {

                        if (std::fabs(gradient[i] - var::tape.Value(this->active_parameters_m[i]->info->id)) > 1e-5) {
                            std::cout << "gradient doesn't match....\n";
                            std::cout << gradient[i] << " != " << var::tape.Value(this->active_parameters_m[i]->info->id);
                            //                            exit(0);
                        }


                        gradient[i] = var::tape.Value(this->active_parameters_m[i]->info->id);
                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {

                                third_mixed_exact[i][j][k] = atl::Variable<T>::tape.Value(this->active_parameters_m[i]->info->id, this->active_parameters_m[j]->info->id, this->active_parameters_m[k]->info->id);
                            }
                        }
                    }

                    std::valarray< std::valarray<std::valarray < T> > > third_mixed_estimated(std::valarray<std::valarray < T> > (std::valarray<T > (active_parameters_m.size()), active_parameters_m.size()), active_parameters_m.size());
                    std::cout << "estimating third order mixed..." << std::flush;

                    auto estimated_to_start = std::chrono::steady_clock::now();
                    third_mixed_estimated = this->EstimateThirdOrderMixed();
                    auto estimated_to_end = std::chrono::steady_clock::now();
                    std::chrono::duration<double> estimated_to_time = (estimated_to_end - estimated_to_start);
                    std::cout << "done.\n" << std::flush;

                    T to_sum = 0;
                    for (int i = 0; i < this->active_parameters_m.size(); i++) {
                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {
                                to_sum += third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k];
                                //                                if (std::fabs(third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k]) > 1e-2) {
                                //                                    std::cout << "{" << this->active_parameters_m[i]->info->id << "," << this->active_parameters_m[j]->info->id << "," << this->active_parameters_m[k]->info->id << "}" << "third order failed " << third_mixed_exact[i][j][k] << " != " << third_mixed_estimated[i][j][k] << " <-----\n";
                                //                                }
                                //                                    else {
                                //                                        std::cout << "{" << this->active_parameters_m[i]->info->id << "," << this->active_parameters_m[j]->info->id << "," << this->active_parameters_m[k]->info->id << "}" << "third order PASSED " << third_mixed_exact[i][j][k] << " == " << third_mixed_estimated[i][j][k] << "\n";
                                //                                    }
                            }
                        }
                    }

                    T to_mse = to_sum / (exact_hessian.size() * exact_hessian.size() * exact_hessian.size());



                    if (gmse <= AutoDiffTest<T>::tolerance) {
                        std::cout << "Gradient Test Passed.\n";
                        out << "Gradient  Test Passed.\n";
                    } else {
                        fail++;
                        std::cout << "Gradient  Test Failed(" << gmse << ">" << AutoDiffTest<T>::tolerance << ")\n";
                        out << "Gradient Test Failed.\n";
                    }

                    if (mse <= AutoDiffTest<T>::second_tolerance) {
                        std::cout << "Hessian Test Passed.\n";
                        out << "Hessian  Test Passed.\n";
                    } else {
                        fail++;
                        std::cout << "Hessian  Test Failed(" << mse << ">" << AutoDiffTest<T>::tolerance << ")\n";
                        out << "Hessian Test Failed.\n";
                    }

                    //
                    //                    if (to_mse <= AutoDiffTest<T>::third_tolerance) {
                    //                        std::cout << "Third-Order Test Passed.\n";
                    //                        out << "Third-Order Test Passed.\n";
                    //                    } else {
                    //                        fail++;
                    //                        std::cout << "Third-Order Test Failed(" << to_mse << ">" << AutoDiffTest<T>::tolerance << ")\n";
                    //                        out << "Third-Order Test Failed.\n";
                    //                    }

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
                    out << "Hessain Speed up = " << estimated_time / exact_time << "\n";
                    out << "Third-Order Speed up = " << estimated_to_time / exact_to_time << "\n\n";


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
                    for (int i = 0; i < this->active_parameters_m.size(); i++) {

                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            out << "[" << i << "] ";
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {
                                out << third_mixed_estimated[i][j][k] << " ";
                            }
                            out << "\n";
                        }
                    }
                    //
                    out << "\nExact Third-Order:\n";
                    for (int i = 0; i < this->active_parameters_m.size(); i++) {

                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            out << "[" << i << "] ";
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {
                                out << third_mixed_exact[i][j][k] << " ";
                            }
                            out << "\n";
                        }
                    }


                    out << "\nThird-Order difference:\n";
                    for (int i = 0; i < this->active_parameters_m.size(); i++) {

                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            out << "[" << i << "] ";
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {
                                out << (third_mixed_exact[i][j][k] - third_mixed_estimated[i][j][k]) << " ";
                            }
                            out << "\n";
                        }
                    }


                }

                const std::valarray<T> EstimateGradient() {
                    var::tape.Reset();
                    atl::Variable<T>::tape.recording = (false);
                    std::valarray<T> gradient(active_parameters_m.size());
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    atl::Variable<T> fh;
                    T fv;
                    T delta = 1.e-5;
                    for (int i = 0; i < active_parameters_m.size(); i++) {

                        active_parameters_m[i]->SetValue(active_parameters_m[i]->GetValue() + delta);
                        fh = 0.0;
                        this->ObjectiveFunction(fh);
                        fv = fh.GetValue();
                        active_parameters_m[i]->SetValue(active_parameters_m[i]->GetValue() - 2 * delta);
                        fh = 0.0;
                        this->ObjectiveFunction(fh);
                        gradient[i] = (fv - fh.GetValue()) / (2.0 * delta);
                        active_parameters_m[i]->SetValue(active_parameters_m[i]->GetValue() + delta);
                    }
                    return gradient;
                }

                /**
                 * This function is a port of from admb.
                 * @return
                 */
                const std::valarray<std::valarray<T> > EstimatedHessian() {

                    atl::Variable<T>::tape.recording = (true);
                    atl::Variable<T>::tape.derivative_trace_level = FIRST_ORDER_REVERSE;
                    atl::Variable<T>::tape.Reset();
                    std::valarray<std::valarray<T> > hessian(
                            std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    T delta = 1.e-3;
                    std::valarray < T> g1(active_parameters_m.size());
                    std::valarray < T> g2(active_parameters_m.size());
                    //            std::valarray < T> hess(active_parameters_m.size());
                    std::valarray < T> hess1(active_parameters_m.size());
                    std::valarray < T> hess2(active_parameters_m.size());
                    T eps = .1;
                    T sdelta1;
                    T sdelta2;

                    atl::Variable<T> f;

                    atl::Variable<T>::tape.Reset();
                    for (int i = 0; i < active_parameters_m.size(); i++) {



                        T xsave = active_parameters_m[i]->GetValue();
                        sdelta1 = /*active_parameters_m[i]->GetValue()*/ +delta;
                        //                sdelta1 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateFirstOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            g1[j] = var::tape.Value(this->active_parameters_m[j]->info->id);
                        }

                        atl::Variable<T>::tape.Reset();
#else
                        g1 = this->EstimateGradient();
#endif
                        f = 0.0;
                        sdelta2 = /*active_parameters_m[i]->GetValue()*/ -delta;
                        //                sdelta2 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateFirstOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            g2[j] = var::tape.Value(this->active_parameters_m[j]->info->id);
                            ;
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        g2 = this->EstimateGradient();
#endif
                        active_parameters_m[i]->SetValue(xsave);

                        hess1 = (g1 - g2) / (sdelta1 - sdelta2);


                        sdelta1 = /*active_parameters_m[i]->GetValue() +*/ eps*delta;
                        //                sdelta1 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                        f = 0.0;
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateFirstOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            g1[j] = var::tape.Value(this->active_parameters_m[j]->info->id);
                            ;
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        g1 = this->EstimateGradient();
#endif

                        active_parameters_m[i]->SetValue(xsave - eps * delta);
                        sdelta2 = /*active_parameters_m[i]->GetValue()*/ -eps*delta;
                        //                sdelta2 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                        f = 0.0;
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateFirstOrder();


                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            g2[j] = var::tape.Value(this->active_parameters_m[j]->info->id);
                            ;
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        g2 = this->EstimateGradient();
#endif
                        active_parameters_m[i]->SetValue(xsave);


                        T eps2 = eps*eps;
                        hess2 = (g1 - g2) / (sdelta1 - sdelta2);
                        hessian[i] = (eps2 * hess1 - hess2) / (eps2 - 1.);
                    }
                    var::tape.Reset();
                    return hessian;

                }

                const std::valarray<std::valarray<std::valarray<T> > > EstimateThirdOrderMixed() {
                    atl::Variable<T>::tape.recording = (true);
                    atl::Variable<T>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;
                    atl::Variable<T>::tape.Reset();

                    std::valarray< std::valarray<std::valarray < T> > > third_mixed(std::valarray<std::valarray < T> > (std::valarray<T > (active_parameters_m.size()), active_parameters_m.size()), active_parameters_m.size());

                    T delta = 1.e-3;
                    T eps = .1;
                    T sdelta1;
                    T sdelta2;
                    atl::Variable<T> f;

                    atl::Variable<T>::tape.Reset();
                    std::valarray < std::valarray < T> > h1(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> h2(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    //            std::valarray < T> hess(active_parameters_m.size());
                    std::valarray < std::valarray < T >> t1(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> t2(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());

                    std::valarray < std::valarray < T> > sd1(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> sd2(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T> > sd3(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> sd4(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> eps_squared(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    std::valarray < std::valarray < T >> eps_squared_minus_one(std::valarray<T > (active_parameters_m.size()), active_parameters_m.size());
                    for (int i = 0; i < active_parameters_m.size(); i++) {
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            sd1[i][j] = delta;
                            sd2[i][j] = -1.0 * delta;
                            sd3[i][j] = eps*delta;
                            sd4[i][j] = -1.0 * eps*delta;
                            eps_squared[i][j] = eps*eps;
                            eps_squared_minus_one[i][j] = eps_squared[i][j] - 1.0;
                        }
                    }


                    for (int i = 0; i < this->active_parameters_m.size(); i++) {


                        T xsave = active_parameters_m[i]->GetValue();
                        sdelta1 = /*active_parameters_m[i]->GetValue()*/ +delta;
                        //                sdelta1 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateSecondOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            for (int k = 0; k < active_parameters_m.size(); k++) {
                                h1[j][k] = atl::Variable<T>::tape.Value(active_parameters_m[j]->info->id, active_parameters_m[k]->info->id); //active_parameters_m[j]->info->hessian_row[active_parameters_m[k]->info->id];
                            }
                        }

                        atl::Variable<T>::tape.Reset();
#else
                        h1 = this->EstimatedHessian();
#endif
                        f = 0.0;
                        sdelta2 = /*active_parameters_m[i]->GetValue()*/ -delta;
                        //                sdelta2 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateSecondOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            for (int k = 0; k < active_parameters_m.size(); k++) {
                                h2[j][k] = atl::Variable<T>::tape.Value(active_parameters_m[j]->info->id, active_parameters_m[k]->info->id); //active_parameters_m[j]->info->hessian_row[active_parameters_m[k]->info->id];
                            }
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        h2 = this->EstimatedHessian();
#endif
                        active_parameters_m[i]->SetValue(xsave);


                        t1 = (h1 - h2) / (sd1 - sd2);


                        sdelta1 = /*active_parameters_m[i]->GetValue() +*/ eps*delta;
                        //                sdelta1 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta1);
#ifdef HESSIAN_USE_AD_GRADIENT
                        f = 0.0;
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateSecondOrder();
                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            for (int k = 0; k < active_parameters_m.size(); k++) {
                                h1[j][k] = atl::Variable<T>::tape.Value(active_parameters_m[j]->info->id, active_parameters_m[k]->info->id); //active_parameters_m[j]->info->hessian_row[active_parameters_m[k]->info->id];
                            }
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        h1 = this->EstimatedHessian();
#endif

                        active_parameters_m[i]->SetValue(xsave - eps * delta);
                        sdelta2 = /*active_parameters_m[i]->GetValue()*/ -eps*delta;
                        //                sdelta2 -= active_parameters_m[i]->GetValue();
                        active_parameters_m[i]->SetValue(xsave + sdelta2);
#ifdef HESSIAN_USE_AD_GRADIENT
                        f = 0.0;
                        ObjectiveFunction(f);
                        atl::Variable<T>::tape.AccumulateSecondOrder();

                        for (int j = 0; j < active_parameters_m.size(); j++) {
                            for (int k = 0; k < active_parameters_m.size(); k++) {
                                h2[j][k] = atl::Variable<T>::tape.Value(active_parameters_m[j]->info->id, active_parameters_m[k]->info->id); //active_parameters_m[j]->info->hessian_row[active_parameters_m[k]->info->id];
                            }
                        }
                        atl::Variable<T>::tape.Reset();
#else
                        h2 = this->EstimatedHessian();
#endif
                        active_parameters_m[i]->SetValue(xsave);


                        T eps2 = eps*eps;
                        t2 = (h1 - h2) / (sd3 - sd4);
                        third_mixed[i] = (eps_squared * t1 - t2) / (eps_squared_minus_one);
                    }




                    return third_mixed;

                }

            };

            template<class T>
            T AutoDiffTest<T>::tolerance = T(1e-5);

            template<class T>
            T AutoDiffTest<T>::second_tolerance = T(1e-3);

            template<class T>
            T AutoDiffTest<T>::third_tolerance = T(1e-1);

            template<class T>
            class Ackley : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                typedef atl::Variable<T> variable;
                std::vector<variable > x;

                variable a;
                variable b;
                variable c;
                variable two;
                T d;

                atl::Variable<T> f;
                std::mt19937 generator; //// mt19937 is a standard mersenne_twister_engine
                std::normal_distribution<T> distribution;

                Ackley(std::ostream& out) {
                    //                    d = (T) dd;
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = 20;
                    b = .2;
                    c = 2.0 * M_PI;
                    two = 2.0;

                    T start = -32.768;
                    d = 0.0;
                    for (T i = -32.767; i < 32.768; i += .05) {
                        T r;
                        this->x.push_back(variable((i)));
                        start += .01;
                        d++;
                    }

                    this->Register(a);
                    this->Register(b);
                    this->Register(c);

                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << ";\n";
                    out << "Variable b = " << b << ";\n";
                    out << "Variable c = " << c.info->id << ";\n";
                    out << "Variable x[" << (int) d << "];\n";
                    out << "f = 0;" << "\n";
                    out << "variable sum1;" << ";\n";
                    out << "variable sum2;" << ";\n";
                    out << "variable term1;" << ";\n";
                    out << "variable term2;" << ";\n\n";

                    out << "for (int i = 0; i < x.size(); i++) {" << "\n";
                    out << "    sum1 += atl::pow(x[i], 2.0);" << "\n";
                    out << "    sum2 += atl::cos(x[i] * c);" << "\n";
                    out << "}\n\n";

                    out << "term1 = -a * atl::exp(-b * atl::sqrt(sum1 / d));" << "\n";
                    out << "term2 = -1.0 * atl::exp(sum2 / d);" << "\n\n";

                    out << "f = term1 + term2 + a + exp(1);" << "\n";
                }

                void ObjectiveFunction(var& f) {
                    f = 0.0;
                    variable sum1;
                    variable sum2;
                    variable term1;
                    variable term2;

                    for (int i = 0; i < x.size(); i++) {
                        sum1 += x[i] * x[i]; //10.0;//atl::pow(x[i], static_cast<T> (2.0));
                        sum2 += atl::cos(x[i] * c);
                    }

                    term1 = -1.0 * a * atl::exp(-1.0 * b * atl::sqrt(sum1 / d));
                    term2 = static_cast<T> (-1.0) * atl::exp(sum2 / d);

                    f = term1 + term2 + a + std::exp(1);
                }


            };

            template<class T>
            class LogTheta2 : public AutoDiffTest<T> {
            public:

                std::vector<T> Y = {3.02315,
                    3.21984, 3.65424, 3.60466, 3.5423, 4.43166, 4.29131, 4.36787, 4.37504, 3.94537, 4.2912,
                    4.59584, 4.30217, 4.15193, 4.42821, 4.30537, 4.54861, 4.63205, 4.78491, 4.79962, 4.92626,
                    5.13627, 5.14859, 4.64859, 5.29066, 4.67895, 5.08076, 5.22367, 5.26112, 4.76097, 4.7894,
                    5.22118, 5.20706, 5.20803, 5.18209, 5.41249, 5.8154, 5.20875, 5.78776, 6.08957, 5.90054,
                    5.94058, 6.21408, 5.84143, 6.42039, 6.46694, 6.73742, 6.7644, 6.80683, 6.33115, 6.7795,
                    6.72164, 6.74198, 7.00933, 6.65849, 6.51895, 6.29572, 6.34795, 6.68318, 6.91444, 6.27536,
                    6.41721, 6.82602, 6.58711, 6.50546, 6.85916, 6.67842, 6.83515, 6.88816, 6.82988, 6.62378,
                    6.8927, 6.60941, 6.80232, 6.88835, 6.92828, 7.12176, 6.66136, 6.7966, 6.42991, 6.59199,
                    6.77583, 6.4914, 6.52556, 6.95658, 6.57174, 6.62285, 6.40535, 6.5243, 6.64547, 6.14812,
                    6.58134, 6.24897, 6.04781, 6.51518, 6.21539, 6.99472, 6.40441, 6.99795, 6.82203, 6.73572,
                    6.74861, 6.30817, 6.53584, 6.37093, 6.47595, 6.29266, 6.35378, 6.53895, 6.24377, 6.49304,
                    6.30005, 6.56823, 6.52514, 6.75102, 6.27068, 6.83394, 6.48833, 6.2357, 6.71897, 6.6146,
                    6.71341, 6.62067, 6.50148, 6.38419, 6.72777, 6.76925, 6.66819, 6.31156, 7.05917, 6.69037,
                    6.09881, 6.8032, 6.30866, 6.56355, 6.39023, 6.59533, 6.69288, 6.68218, 6.91985, 6.90063,
                    6.51458, 6.88214, 7.04983, 7.07688, 7.12766, 6.59529, 6.71444, 6.95628, 6.49304, 6.76456,
                    6.4496, 6.52297, 6.47359, 6.72056, 6.67134, 6.1982, 6.47844, 6.91845, 6.7916, 6.96461,
                    6.7269, 6.49073, 6.9752, 7.01309, 7.25482, 6.79274, 7.05099, 6.95183, 6.98215, 7.10636,
                    6.98072, 7.32764, 7.01756, 6.51239, 6.98783, 6.64356, 6.67166, 7.2179, 6.73284, 6.50916,
                    6.32713, 6.90174, 6.5849, 6.71981, 6.44168, 6.74218, 6.6416, 6.87571, 6.727, 7.13012,
                    7.01468, 6.92291, 6.68062, 7.07283, 6.64972, 7.01744, 6.98964, 6.71007, 6.83583};

                std::vector<atl::Variable<T> > X;
                atl::Variable<T> logr0; // = -2.6032947;
                atl::Variable<T> logtheta; // = 0.7625692;
                atl::Variable<T> logK; // = 6.0; //  = 6.7250075;
                atl::Variable<T> logQ = -1.7496015;
                atl::Variable<T> logR; // = -3.1889239;

                LogTheta2() {

                }

                LogTheta2(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Description(std::stringstream& out) {
                    out << "LogTheta model....\n";
                }

                void Initialize() {



                    int size = Y.size();
                    std::cout << "size = " << size;
                    this->X.resize(size);



                    this->Register(logr0);
                    this->Register(logtheta);
                    //                    logK.SetBounds(4.6, 7.6);
                    this->Register(logK);
                    this->Register(logQ);
                    this->Register(logR);

                    for (int i = 0; i < X.size(); i++) {
                        X[i] = 1.0; //atl::Variable<T>(0.012);
                        //                                                this->Register(X[i]);
                    }
                    //                    std::cout<<logK.info->id<<" --- "<<logQ.info->id<<"\n";
                    //                    exit(0);
                }

                inline const atl::Variable<T> dnorm(const atl::Variable<T> x,
                        const atl::Variable<T> mean,
                        const atl::Variable<T> sd, int give_log = 0) {
                    if (sd.GetValue() == 0.0) {
                        throw std::overflow_error("Divide by zero exception");
                    }

                    atl::Variable<T> logres = static_cast<T> (-1.0) * atl::log(T(std::sqrt(2 * M_PI)) * sd) - static_cast<T> (.5) * atl::pow((x - mean) / sd, static_cast<T> (2.0));
                    if (give_log)return logres;
                    else return atl::exp(logres);
                }

                virtual void ObjectiveFunction(atl::Variable<T>& f) {

                    size_t timeSteps = Y.size();
                    atl::Variable<T> r0 = atl::exp(logr0);
                    atl::Variable<T> theta = atl::exp(logtheta);
                    atl::Variable<T> K = atl::exp(logK);
                    atl::Variable<T> Q = atl::exp(logQ);
                    atl::Variable<T> R = atl::exp(logR);


                    //        atl::Variable<T> r0 = atl::exp(logr0);
                    //        atl::Variable<T> theta = atl::exp(logtheta);
                    //        atl::Variable<T> K = atl::exp(logK);
                    //                            atl::Variable<T> Q = atl::exp(logQ);
                    //                            atl::Variable<T> R = atl::exp(logR);


                    f = T(0.0);

                    //        atl::Variable<T> sqrtq = atl::sqrt(Q);
                    for (int i = 1; i < timeSteps; i++) {
                        atl::Variable<T> m = X[i - 1] + r0 * (static_cast<T> (1.0) - atl::pow(atl::exp(X[i - 1]) / K, theta));
                        f -= this->dnorm(X[i], m, atl::sqrt(Q), true);
                    }
                    //        atl::Variable<T> sqrtr = atl::sqrt(R);
                    for (int i = 0; i < timeSteps; i++) {
                        f -= this->dnorm(atl::Variable<T>(Y[i]), X[i], atl::sqrt(R), true);
                    }
                }

            };

            template<class T>
            class AddAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                AddAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a + b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a + b;
                }


            };

            template<class T>
            class Add1AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Add1AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    //                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a + 3.1459" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a + static_cast<T> (3.1459);
                }


            };

            template<class T>
            class Add2AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Add2AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    //                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    //                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = 3.1459 + b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (3.1459) + b;
                }


            };

            template<class T>
            class SubtractAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                SubtractAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a - b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a - b;
                }


            };

            template<class T>
            class Subtract1AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Subtract1AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    //                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a - 3.1459" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a - static_cast<T> (3.1459);
                }


            };

            template<class T>
            class Subtract2AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Subtract2AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    //                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = 3.1459 - b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (3.1459) - b;
                }


            };

            template<class T>
            class MultiplyAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                MultiplyAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a * b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a * b;
                }


            };

            template<class T>
            class Multiply1AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Multiply1AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    //                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a * 3.1459" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a * static_cast<T> (3.1459);
                }


            };

            template<class T>
            class Multiply2AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Multiply2AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    //                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = 3.1459 * b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (3.1459) * b;
                }


            };

            template<class T>
            class DivideAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;
                var c;

                DivideAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    c = .5;
                    this->Register(a);
                    this->Register(b);
                    this->Register(c);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a / b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    //                    f = atl::sin(a) / b;
                    f = ((a - b) / c)*((a - b) / c);
                }


            };

            template<class T>
            class Divide1AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Divide1AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    //                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = a / 3.1459" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = a / static_cast<T> (3.1459);
                }


            };

            template<class T>
            class Divide2AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Divide2AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = 1.03434;
                    b = 1.034230;
                    //                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = 3.1459 / b" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (3.1459) / b;
                }


            };

            template<class T>
            class ACosAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                ACosAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::acos((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::acos((a * b));
                }


            };

            template<class T>
            class ASinAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                ASinAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::asin((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::asin((a * b));
                }


            };

            template<class T>
            class ATanAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                ATanAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::atan((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::atan((a * b));
                }


            };

            template<class T>
            class CeilAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                CeilAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::ceil((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::ceil((a * b));
                }


            };

            template<class T>
            class CosAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                CosAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::cos((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::cos((a * b));
                }


            };

            template<class T>
            class CoshAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                CoshAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::cosh((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::cosh((a * b));
                }


            };

            template<class T>
            class ExpAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                ExpAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::exp((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::exp((a / b));
                }


            };

            template<class T>
            class FabsAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                FabsAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::fabs((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::fabs((a * b));
                }


            };

            template<class T>
            class FloorAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                FloorAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::floor((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::floor((a * b));
                }


            };

            template<class T>
            class LogAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                LogAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .3434;
                    b = .34230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::log((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::log(((a + b)));
                }


            };

            template<class T>
            class Log10AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                Log10AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .3434;
                    b = .34230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::log10((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::log10((a * b));
                }


            };

            template<class T>
            class PowAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;
                var c;

                PowAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = 1.001567;
                    b = 1.00113;
                    c = 1.001;
                    this->Register(a);
                    this->Register(b);
                    this->Register(c);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::pow((a * b),(a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::pow((a * b), (a * c));
                }


            };

            template<class T>
            class PowCAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                PowCAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::pow((a * b),2.0)" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::pow(a, static_cast<T> (2.0));
                    f += atl::pow(b*b, static_cast<T> (2.0));
                }


            };

            template<class T>
            class PowC2AutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                PowC2AutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::pow(2.0,(a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::pow(static_cast<T> (2.0), (a * b));
                }


            };

            template<class T>
            class SinAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                SinAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::sin((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::sin((a * b));
                }


            };

            template<class T>
            class SinhAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                SinhAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::sinh((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::sinh((a * b));
                }


            };

            template<class T>
            class SqrtAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                SqrtAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::sqrt((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::sqrt((a * b));
                }


            };

            template<class T>
            class TanAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                TanAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::tan((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::tan((a * b));
                }


            };

            template<class T>
            class TanhAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var a;
                var b;

                TanhAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    a = .03434;
                    b = .034230;
                    this->Register(a);
                    this->Register(b);



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << "\n";
                    out << "Variable b = " << b << "\n";
                    out << "f = atl::tanh((a * b))" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = atl::tanh((a * b));
                }


            };

            template<class T>
            class RastriginAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                std::vector<var> x;

                RastriginAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x.resize(10);
                    T temp = -5.1;
                    for (int i = 0; i < x.size(); i++) {
                        x[i] = temp++; //(var(i));
                        this->Register(x[i]);

                    }



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Rastrigin Function:\n";
                    out << "Parameters:\nVariable A = 10.0;\n vector<Variable> x;\n";
                    out << "Variable f = 0.0;\nfor(int i =0; i < x.size(); i++){\n";
                    out << "    f+=atl::pow(x[i],2.0) - A*atl::cos(2*M_PI*x[i]);\n";
                    out << "}\n";
                    out << "f = A*static_cast<T>(x.size())+f;\n" << std::endl;

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (0.0);
                    var A = static_cast<T> (10.0);
                    var sum;
                    for (int i = 0; i < x.size(); i++) {
                        sum += atl::pow(x[i], 2.0) - A * atl::cos(2.0 * M_PI * x[i]);
                    }
                    f = A * static_cast<T> (x.size()) + sum;
                }


            };

            template<class T>
            class SphereAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                std::vector<var> x;

                SphereAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x.resize(10);
                    T temp = -1.0;
                    for (int i = 0; i < x.size(); i++) {
                        x[i] = temp += .002; //(var(i));
                        this->Register(x[i]);

                    }



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Sphere Function:\n";
                    out << "Parameters:\n";
                    out << "Variable f = 0.0;\nfor(int i =0; i < x.size(); i++){\n";
                    out << "    f+=atl::pow(x[i],2.0);\n";
                    out << "}\n";

                }

                void ObjectiveFunction(var& f) {
                    var two(2.0);
                    f = static_cast<T> (0.0);
                    for (int i = 0; i < x.size(); i++) {
                        f += atl::pow(x[i], 2.0);
                    }
                }


            };

            template<class T>
            class RosenbrockAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                std::vector<var> x;

                RosenbrockAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x.resize(10);
                    T temp = -1.0;
                    for (int i = 0; i < x.size(); i++) {
                        x[i] = temp += .002; //(var(i));
                        this->Register(x[i]);

                    }



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Rosenbrock Function:\n";
                    out << "Parameters:\n";
                    out << "Variable f = 0.0;\nfor(int i =0; i < x.size()-1; i++){\n";
                    out << "    f+=100.0*atl::pow((x[i+1] - atl::pow(x[i],2.0),2.0)+atl::pow(x[i]-1.0,2.0);\n";
                    out << "}\n";

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (0.0);
                    for (int i = 0; i < x.size() - 1; i++) {
                        f += 100.0 * atl::pow(x[i + 1] - atl::pow(x[i], 2.0), 2.0) + atl::pow(x[i] - 1.0, 2.0);
                    }
                }


            };

            template<class T>
            class StyblinskiTangAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                std::vector<var> x;

                StyblinskiTangAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x.resize(10);
                    T temp = -5.0;
                    for (int i = 0; i < x.size(); i++) {
                        x[i] = temp += .002; //(var(i));
                        this->Register(x[i]);

                    }



                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Styblinski-Tang Function:\n";
                    out << "Parameters:\n";
                    out << "Variable sum, f = 0.0;\nfor(int i =0; i < x.size(); i++){\n";
                    out << "    sum+=atl::pow(x[i],4.0)- 16.0*atl::pow(x[i],2.0)+5.0*x[i];\n";
                    out << "}\n";
                    out << "f = sum/2.0;";

                }

                void ObjectiveFunction(var& f) {
                    f = static_cast<T> (0.0);
                    var sum;
                    for (int i = 0; i < x.size(); i++) {
                        sum += atl::pow(x[i], 4.0) - 16.0 * atl::pow(x[i], 2.0) + 5.0 * x[i];
                    }
                    f = sum / 2.0;
                }


            };

            template<class T>
            class BukinAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;

                BukinAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x = -10.0;
                    y = 2.0;
                    this->Register(x);
                    this->Register(y);
                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Bukin Function:\n";
                    out << "Parameters:\n";
                    out << "Variable f = 0.0;\n";
                    out << "f = 100.0*atl::sqrt(atl::ad_fabs(y-.01*atl::pow(x,2.0),1e-6))+.01*atl::ad_fabs(x+10.0,1e-6); \n";

                }

                void ObjectiveFunction(var& f) {
                    f = 100.0 * atl::sqrt(atl::ad_fabs<T>(y - .01 * atl::pow(x, 2.0), 1e-6)) + .01 * atl::ad_fabs<T>(x + 10.0, 1e-6);
                }


            };

            template<typename T>
            class EasomAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;

                EasomAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x = M_PI + .01;
                    y = M_PI + .01;

                    this->Register(x);
                    this->Register(y);
                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Easom Function:\n";
                    out << "Parameters:\n";
                    out << "Variable f = 0.0;\n";
                    out << "f = -atl::cos(x)*atl::cos(y)*atl::exp(-(atl::pow(x-M_PI,2.0)+atl::pow(y-M_PI,2.0))); \n";

                }

                void ObjectiveFunction(var& f) {
                    f = -1.0 * atl::cos(x) * atl::cos(y) * atl::exp(-1.0 * (atl::pow(x - M_PI, 2.0) + atl::pow(y - M_PI, 2.0)));
                }


            };

        
            template<class T>
            class McCormickAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;

                McCormickAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x = -1.4;
                    y = 2.0;

                    this->Register(x);
                    this->Register(y);
                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "McCormick Function:\n";
                    out << "Parameters:\n";
                    out << "Variable x,y \n";
                    out << "Variable f = 0.0;\n";
                    out << "f = atl::sin(x+y)+atl::pow(x-y,2.0) - 1.5*x + 2.5*y+1.0; \n";

                }

                void ObjectiveFunction(var& f) {
                    f = atl::sin(x + y) + atl::pow(x - y, 2.0) - 1.5 * x + 2.5 * y + 1.0;
                }


            };

            template<class T>
            class SchafferAutoDiffTest : public AutoDiffTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;

                SchafferAutoDiffTest(std::ostream& out) {
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    x = -0.0324234;
                    y = 1.01;

                    this->Register(x);
                    this->Register(y);
                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Schaefer Function:\n";
                    out << "Parameters:\n";
                    out << "Variable x,y \n";
                    out << "Variable f = 0.0;\n";
                    out << "f = 0.5 + (atl::pow(atl::sin(atl::pow(x,2.0)- atl::pow(y,2.0)),2.0)-0.5)/atl::pow(1.0+.001*(atl::pow(x,2.0)+atl::pow(y,2.0)),2.0); \n";

                }

                void ObjectiveFunction(var& f) {
                    f = 0.5 + (atl::pow(atl::sin(atl::pow(x,2.0)- atl::pow(y,2.0)),2.0)-0.5)/atl::pow(1.0+.001*(atl::pow(x,2.0)+atl::pow(y,2.0)),2.0);
                }


            };

            void Run(std::ostream& out) {
                typedef double real_t;
                atl::tests::auto_diff::AddAutoDiffTest<real_t> add(out);
                atl::tests::auto_diff::Add1AutoDiffTest<real_t> add1(out);
                atl::tests::auto_diff::Add2AutoDiffTest<real_t> add2(out);
                atl::tests::auto_diff::SubtractAutoDiffTest<real_t> subtract1(out);
                atl::tests::auto_diff::Subtract1AutoDiffTest<real_t> subtract(out);
                atl::tests::auto_diff::Subtract2AutoDiffTest<real_t> subtract2(out);
                atl::tests::auto_diff::MultiplyAutoDiffTest<real_t> multiply(out);
                atl::tests::auto_diff::Multiply1AutoDiffTest<real_t> multiply1(out);
                atl::tests::auto_diff::Multiply2AutoDiffTest<real_t> multiply2(out);
                atl::tests::auto_diff::DivideAutoDiffTest<real_t> divide(out);
                atl::tests::auto_diff::Divide1AutoDiffTest<real_t> divide1(out);
                atl::tests::auto_diff::Divide2AutoDiffTest<real_t> divide2(out);
                atl::tests::auto_diff::CosAutoDiffTest<real_t> cos(out);
                atl::tests::auto_diff::ACosAutoDiffTest<real_t> acos(out);
                atl::tests::auto_diff::SinAutoDiffTest<real_t> sin(out);
                atl::tests::auto_diff::ASinAutoDiffTest<real_t> asin(out);
                atl::tests::auto_diff::TanAutoDiffTest<real_t> tan(out);
                atl::tests::auto_diff::ATanAutoDiffTest<real_t> atan(out);
                atl::tests::auto_diff::CoshAutoDiffTest<real_t> cosh(out);
                atl::tests::auto_diff::SinhAutoDiffTest<real_t> sinh(out);
                atl::tests::auto_diff::TanhAutoDiffTest<real_t> tanh(out);
                atl::tests::auto_diff::ExpAutoDiffTest<real_t> exp(out);
                atl::tests::auto_diff::LogAutoDiffTest<real_t> log(out);
                atl::tests::auto_diff::Log10AutoDiffTest<real_t> log10(out);
                atl::tests::auto_diff::FabsAutoDiffTest<real_t> fabs(out);
                atl::tests::auto_diff::SqrtAutoDiffTest<real_t> sqrt(out);
                atl::tests::auto_diff::PowAutoDiffTest<real_t> pow(out);
                atl::tests::auto_diff::PowCAutoDiffTest<real_t> pow2(out);
                atl::tests::auto_diff::PowC2AutoDiffTest<real_t> pow3(out);
                atl::tests::auto_diff::CeilAutoDiffTest<real_t> ceil(out);
                atl::tests::auto_diff::FloorAutoDiffTest<real_t> floor(out);
                atl::tests::auto_diff::Ackley<real_t> a(out);
                atl::tests::auto_diff::LogTheta2<real_t> l(out);
                atl::tests::auto_diff::RastriginAutoDiffTest<real_t> r(out);
                atl::tests::auto_diff::SphereAutoDiffTest<real_t> sphere(out);
                atl::tests::auto_diff::RosenbrockAutoDiffTest<real_t> rosenbrock(out);
                atl::tests::auto_diff::StyblinskiTangAutoDiffTest<real_t> styblinskitang(out);
                atl::tests::auto_diff::BukinAutoDiffTest<real_t> bukin(out);
                atl::tests::auto_diff::EasomAutoDiffTest<real_t> easom(out);
                atl::tests::auto_diff::McCormickAutoDiffTest<real_t> mccormick(out);
                atl::tests::auto_diff::SchafferAutoDiffTest<real_t> shaefer(out);
                std::cout << "Test complete.\n";
                if (atl::tests::auto_diff::fail == 0) {
                    std::cout << "All tests passed."; //, review file \"autodiff_test.txt\" for details." << std::endl;

                } else {

                    std::cout << atl::tests::auto_diff::fail << " of " << atl::tests::auto_diff::tests << " tests did not agree within a tolerance of " << atl::tests::auto_diff::AutoDiffTest<real_t>::GetTolerance() << ", review file \"autodiff_test.txt\" for details." << std::endl;
                }

            }

        }
    }
}


#endif /* AUTODIFFTESTS_HPP */

