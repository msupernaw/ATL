/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FunctionMinimizerTests.hpp
 * Author: matthewsupernaw
 *
 * Created on March 8, 2017, 2:15 PM
 */

#ifndef FUNCTIONMINIMIZERTESTS_HPP
#define FUNCTIONMINIMIZERTESTS_HPP

#include "../Optimization.hpp"
#include <vector>

namespace atl {
    namespace tests {
        namespace fmin {

            template<typename T>
            struct MinimizerTest {

                bool const IsClose(const T& f1, const T& f2, T figures_tolerance = 3.0) {
                    T res = f1 - f2;
                    res = res * std::pow(10.0, figures_tolerance);
                    return !bool(int(res));
                }

            };

            template<typename T>
            struct Rosenbrock : atl::ObjectiveFunction<T>, MinimizerTest<T> {
                typedef atl::Variable<T> var;
                std::vector<var> x;
                T f_;

                void Initialize() {
                    x.resize(10);
                    T temp = -10.0;
                    for (int i = 0; i < x.size(); i++) {
                        std::stringstream ss;
                        ss << "x_" << i;
                        x[i] = temp += 1.2; //(var(i));
                        x[i].SetName(ss.str());
                        this->RegisterParameter(x[i]);

                    }



                }

                void Objective_Function(var& f) {
                    f = static_cast<T> (0.0);
                    for (int i = 0; i < x.size() - 1; i++) {
                        f += 100.0 * atl::pow(x[i + 1] - atl::pow(x[i], 2.0), 2.0) + atl::pow(x[i] - 1.0, 2.0);
                    }
                    this->f_ = f.GetValue();
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }

                bool Check() {
                    if (this->IsClose(this->f_, 1e-5, 3.0)) {
                        return true;
                    } else {
                        return false;
                    }
                }


            };

            template<class T>
            class Schaffer : public atl::ObjectiveFunction<T>, MinimizerTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;
                T f_;

                void Initialize() {
                    x = -0.0324234;
                    x.SetName("x");
                    y = 1.01;
                    y.SetName("y");

                    this->RegisterParameter(x);
                    this->RegisterParameter(y);
                }

                void Objective_Function(var& f) {
                    f = 0.5 + (atl::pow(atl::sin(atl::pow(x, 2.0) - atl::pow(y, 2.0)), 2.0) - 0.5) / atl::pow(1.0 + .001 * (atl::pow(x, 2.0) + atl::pow(y, 2.0)), 2.0);
                    f_ = f.GetValue();
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }

                bool Check() {
                    if (this->IsClose(this->f_, 1e-5, 3.0)) {
                        return true;
                    } else {
                        return false;
                    }
                }

            };

            template<class T>
            class McCormick : public atl::ObjectiveFunction<T>, MinimizerTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;
                T f_;

                void Initialize() {
                    x = -1.4;
                    x.SetName("x");
                    y = 2.0;
                    y.SetName("y");

                    this->RegisterParameter(x);
                    this->RegisterParameter(y);
                }

                void Objective_Function(var& f) {
                    f = atl::sin(x + y) + atl::pow(x - y, 2.0) - 1.5 * x + 2.5 * y + 1.0;
                    f_ = f.GetValue();
                }

                bool Check() {
                    if (this->IsClose(x.GetValue(), -0.54719, 3.0) && this->IsClose(y.GetValue(), -1.54719, 3.0)) {
                        return true;
                    } else {
                        return false;
                    }
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }

            };

            template<class T>
            class Easom : public atl::ObjectiveFunction<T>, MinimizerTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;
                T f_;

                void Initialize() {
//                    x.SetMinBoundary(-100.0);
//                    x.SetMaxBoundary(100.0);
////                    x = M_PI - 1.50;
                    x.SetName("x");
//                    y.SetMinBoundary(M_PI);
//                    y.SetMaxBoundary(M_PI);
//                    y = M_PI + 1.50;
                    y.SetName("y");

                    this->RegisterParameter(x);
                    this->RegisterParameter(y);
                }

                void Objective_Function(var& f) {
                    f = 0.0;
                    f = -1.0 * atl::cos(x) * atl::cos(y) * atl::exp( (-1.0*((x - M_PI)*(x - M_PI)) - ((y - M_PI)*(y - M_PI))));
                    f_ = f.GetValue();
                }

                bool Check() {
                    if (this->IsClose(x.GetValue(), M_PI, 3.0) && this->IsClose(y.GetValue(), M_PI, 3.0)) {
                        return true;
                    } else {
                        return false;
                    }
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }

            };

            template<class T>
            class Bukin : public atl::ObjectiveFunction<T>, MinimizerTest<T> {
            public:
                typedef atl::Variable<T> var;
                var x;
                var y;
                T f_;

                void Initialize() {
                    x = -15.0;
                    x.SetName("x");
                    y = 2.0;
                    y.SetName("y");
                    this->RegisterParameter(x);
                    this->RegisterParameter(y);
                }

                void Objective_Function(var& f) {
                    f = 100.0 * atl::sqrt(atl::ad_fabs<T>(y - .01 * atl::pow(x, 2.0), 1e-4)) + .01 * atl::ad_fabs<T>(x + 10.0, 1e-4);
                    f_ = f.GetValue();
                }

                bool Check() {
                    if (this->IsClose(x.GetValue(), -10.0) && this->IsClose(y.GetValue(), 1.0)) {
                        return true;
                    } else {
                        return false;
                    }
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }


            };

            template<class T>
            class StyblinskiTang : public atl::ObjectiveFunction<T>, MinimizerTest<T> {
            public:
                typedef atl::Variable<T> var;
                std::vector<var> x;

                void Initialize() {
                    x.resize(10);
                    T temp = -5.0;
                    for (int i = 0; i < x.size(); i++) {
                        x[i] = temp += .002; //(var(i));
                        std::stringstream ss;
                        ss << "x_" << i;
                        x[i].SetName(ss.str());
                        this->RegisterParameter(x[i]);

                    }



                }

                void Objective_Function(var& f) {
                    f = static_cast<T> (0.0);
                    var sum;
                    for (int i = 0; i < x.size(); i++) {
                        sum += atl::pow(x[i], 4.0) - 16.0 * atl::pow(x[i], 2.0) + 5.0 * x[i];
                    }
                    f = sum / 2.0;
                }

                bool Check() {
                    for (int i = 0; i < x.size(); i++) {
                        if (!this->IsClose(x[i].GetValue(), -2.9035, 3.0)) {
                            return false;
                        }
                    }
                    return true;
                }

                const atl::Variable<T> Evaluate() {
                    atl::Variable<T> f;
                    this->ObjectiveFunction(f);
                    return f;
                }


            };

            void Run() {

                atl::Variable<double>::tape.Reset();
                atl::Variable<double>::tape.recording = true;
                std::stringstream out;

                atl::tests::fmin::Rosenbrock<double> rosenbrock;
                rosenbrock.Initialize();
                atl::LBFGS<double> lbfgs;
                lbfgs.SetObjectiveFunction(&rosenbrock);
                lbfgs.Run();
                if (rosenbrock.Check()) {
                    out << "Rosenbrock test passed.\n";
                    std::cout << "Rosenbrock test passed.\n";
                } else {
                    out << "Rosenbrock test failed.\n";
                    std::cout << "Rosenbrock test failed.\n";
                }

                Schaffer<double> shaffer;
                shaffer.Initialize();
                lbfgs.SetObjectiveFunction(&shaffer);
                lbfgs.Run();
                if (shaffer.Check()) {
                    out << "Shaefer test passed.\n";
                    std::cout << "Shaefer test passed.\n";
                } else {
                    out << "Shaefer test failed.\n";
                    std::cout << "Shaefer test failed.\n";

                }

                atl::tests::fmin::McCormick<double> mccormick;
                mccormick.Initialize();
                lbfgs.SetObjectiveFunction(&mccormick);
                lbfgs.Run();
                if (mccormick.Check()) {
                    out << "McCormick test passed.\n";
                    std::cout << "McCormick test passed.\n";
                } else {
                    out << "McCormick test failed.\n";
                    std::cout << "McCormick test failed.\n";
                }


             

                atl::tests::fmin::Bukin<double> bukin;
                bukin.Initialize();
                lbfgs.SetObjectiveFunction(&bukin);
                lbfgs.Run();
                if (bukin.Check()) {
                    out << "Bukin test passed.\n";
                    std::cout << "Bukin test passed.\n";

                } else {
                    out << "Bukin test failed.\n";
                    std::cout << "Bukin test failed.\n";

                }


                atl::tests::fmin::StyblinskiTang<double> styblinski_tang;
                styblinski_tang.Initialize();
                lbfgs.SetObjectiveFunction(&styblinski_tang);
                lbfgs.Run();
                if (styblinski_tang.Check()) {
                    out << "Styblinski Tang test passed.\n";
                    std::cout << "Styblinski Tang test passed.\n";
                } else {
                    out << "Styblinski Tang test failed.\n";
                    std::cout << "Styblinski Tang test failed.\n";
                }

                   atl::tests::fmin::Easom<double> easom;
                easom.Initialize();
                lbfgs.SetTolerance(1e-15);
                lbfgs.SetObjectiveFunction(&easom);
                lbfgs.Run();
                if (easom.Check()) {
                    out << "Easom test passed.\n";
                    std::cout << "Easom test passed.\n";

                } else {
                    out << "Easom test failed.\n";
                    std::cout << "Easom test failed.\n";
                }
                
                std::cout << out.str();

            }



        }
    }
}


#endif /* FUNCTIONMINIMIZERTESTS_HPP */

