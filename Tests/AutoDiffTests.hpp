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
                    this->independents.push_back(v.info);
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

                    for (int i = 0; i < this->active_parameters_m.size(); i++) {

                        if (std::fabs(gradient[i] - var::tape.Value(this->active_parameters_m[i]->info->id)) > 1e-5) {
                            std::cout << "gradient doesn't match....\n";
                            std::cout << gradient[i] << " != " << var::tape.Value(this->active_parameters_m[i]->info->id);
                            //                            exit(0);
                        }
                        gradient[i] = var::tape.Value(this->active_parameters_m[i]->info->id);
                        for (int j = 0; j < this->active_parameters_m.size(); j++) {
                            for (int k = 0; k < this->active_parameters_m.size(); k++) {
                                if (i == 0) {
                                    exact_hessian2[j][k] = atl::Variable<T>::tape.Value(this->active_parameters_m[j]->info->id, this->active_parameters_m[k]->info->id);
                                    if (std::fabs(exact_hessian[j][k] - exact_hessian2[j][k]) > 1e-5) {

                                        std::cout << "hessian doesn't match....\n";
                                        std::cout << exact_hessian[j][k] << " != " << exact_hessian2[j][k];
                                        //                                        exit(0);
                                    }
                                }
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
                    out << "Hessain Speed up = " << estimated_time / exact_time << "\n\n";
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
                        this->ObjectiveFunction(fh);
                        fv = fh.GetValue();
                        active_parameters_m[i]->SetValue(active_parameters_m[i]->GetValue() - 2 * delta);
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
                variable d;

                atl::Variable<T> f;
                std::mt19937 generator; //// mt19937 is a standard mersenne_twister_engine
                std::normal_distribution<T> distribution;

                Ackley(std::ostream& out, int dd) {
                    d = (T) dd;
                    this->RunTestToFile(out);
                }

                void Initialize() {
                    int dimensions = (int) d.GetValue();
                    a = 20;
                    b = .2;
                    c = 2.0 * M_PI;

                    //random seed
                    generator.seed(4090091);
                    //set some random starting values

                    for (int i = 0; i < dimensions; i++) {
                        T r;
                        r = distribution(generator);
                        this->x.push_back(variable(r));
                    }
                    for (int i = 0; i < x.size(); i++) {
                        //                        std::cout<<x[i]<<" ";
                        this->Register(x[i]);
                    }
                    //                    std::cout<<"\n";


                }

                void Description(std::stringstream& out) {
                    out << "Test Problem:\n";
                    out << "Parameters:\n";
                    out << "Variable a = " << a << ";\n";
                    out << "Variable b = " << b << ";\n";
                    out << "Variable c = " << c << ";\n";
                    out << "Variable x[" << (int) d.GetValue() << "];\n";
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
                    f = 0;
                    variable sum1;
                    variable sum2;
                    variable term1;
                    variable term2;

                    for (int i = 0; i < x.size(); i++) {
                        sum1 += atl::pow(x[i], static_cast<T> (2.0));
                        sum2 += atl::cos(x[i] * c);
                    }

                    term1 = -a * atl::exp(-b * atl::sqrt(sum1 / d));
                    term2 = static_cast<T> (-1.0) * atl::exp(sum2 / d);

                    f = term1 + term2 + a + std::exp(1);
                }


            };
            
            
        }
    }
}


#endif /* AUTODIFFTESTS_HPP */

