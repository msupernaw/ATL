/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: matthewsupernaw
 *
 * Created on October 31, 2016, 12:55 PM
 */

#include <sys/resource.h>

#include <cstdlib>
#include "ATL.hpp"
#include "MinMax.hpp"
#include "Expression.hpp"
#include "Matrix.hpp"
#include "Vector.hpp"
#include "Add.hpp"
#include "Subtract.hpp"
#include "Multiply.hpp"
#include "Divide.hpp"
#include "Real.hpp"
#include "ACos.hpp"
#include "ASin.hpp"
#include "ATan.hpp"
#include "Ceil.hpp"
#include "Cos.hpp"
#include "Exp.hpp"
#include "Fabs.hpp"
#include "Floor.hpp"
#include "Log.hpp"
#include "Log10.hpp"
#include "Sin.hpp"
#include "Sinh.hpp"
#include "Sqrt.hpp"
#include "Tan.hpp"
#include "Cosh.hpp"
#include "Tanh.hpp"
#include "Pow.hpp"
#include "Helpers.hpp"
#include "Tests/AutoDiffTests.hpp"
#include "Optimization.hpp"
#include <chrono>
#include "Tests/FunctionMinimizerTests.hpp"

using namespace std;

template <typename T>
class mytest : public atl::tests::auto_diff::AutoDiffTest<T> {
public:
    atl::Variable<T> a;
    atl::Variable<T> b;
    std::vector<atl::Variable<T> > x;

    mytest() {
    }

    mytest(std::ostream& out) {
        this->RunTestToFile(out);
    }

    void Initialize() {
        a = 12.0;
        b = 4.0;
        x.resize(5000);
        this->Register(a);
        this->Register(b);
        for (int i = 0; i < x.size(); i++)
            x[i] = (T) (i + 1);
    }

    void ObjectiveFunction(atl::Variable<T>& f) {
        f = 0.0;
        for (int i = 0; i < x.size(); i++)
            f += x[i] + (atl::pow(a, 2.0) + atl::pow(b, 3.0)) / a;
    }



};

// There are other clocks, but this is usually the one you want.
// It corresponds to CLOCK_MONOTONIC at the syscall level.
using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
//using namespace std::literals::chrono_literals;
using std::this_thread::sleep_for;

//template<typename T, class LHS, class RHS, int>
//struct Test: atl::ExpressionBase<T,Test<T,LHS,RHS,int> >{
//    
//};
//
//
//template<typename T, class LHS, class RHS >
//inline const Test<T,LHS,RHS,int> test(const atl::ExpressionBase<T,LHS>& l, 
//        const atl::ExpressionBase<T,RHS>& r){
//    return Test<T,LHS,RHS,10>();
//}

atl::Variable<double> abss(const atl::Variable<double>& x, double eps = std::numeric_limits< double >::denorm_min()) {
    return atl::pow((x * x) + eps, .5);
}

///**
// * Returns the maximum between a and b in a continuous manner using:
// * 
// * (a + b + |a.GetValue() - b.GetValue()|) / 2.0;
// * 
// * @param a
// * @param b
// * @return 
// */

template <typename T>
inline const atl::Variable<T> damax(const atl::Variable<T>& a, const atl::Variable<T>& b) {
    return (a + b + abss(a - b)) / (2.0);
}
//
///**
// * Returns the minimum between a and b in a continuous manner using:
// * 
// * (a + b - |a.GetValue() - b.GetValue()|) / 2.0;
// * 
// * @param a
// * @param b
// * @return 
// */
//template <typename T>
//inline const atl::Variable<T> admin(const atl::Variable<T>& a, const atl::Variable<T>& b) {
//    return (a + b - atl::fabs((a - b))) / 2.0;
//}

struct ThreadFunction {
    int start;
    int end;
    std::vector<atl::Variable<double> >& v;
    atl::Variable<double>& maxv;
    int estart;

    ThreadFunction(int start, int end, std::vector<atl::Variable<double> >& v, atl::Variable<double>& m, int estart) :
    start(start), end(end), v(v), maxv(m), estart(estart) {
    }

    void operator()() {

        for (int i = start; i < end; i++) {
            this->maxv.Assign(atl::ad_max(maxv, v[i]), estart + i);
        }
    }
};

struct DivideThreadFunction {
    int start;
    int end;
    std::vector<atl::Variable<double> >& v;
    atl::Variable<double>& sum;
    atl::Variable<double>& maxv;
    int estart;

    DivideThreadFunction(int start, int end, std::vector<atl::Variable<double> >& v, atl::Variable<double>& m, atl::Variable<double>& s, int estart) :
    start(start), end(end), v(v), maxv(m), sum(s), estart(estart) {
    }

    void operator()() {

        for (int i = start; i < end; i++) {
            v[i].Assign(v[i] / maxv, estart++);
            sum.Assign(sum + v[i], estart++);
        }
    }
};

inline const atl::Variable<double> ad_norm_and_sum_threaded(std::vector<atl::Variable<double> >& v) {


    int nthreads = atl::thread_pool_g.Size();
    int range = v.size() / nthreads;

    atl::WaitVariable wv;
    std::vector<atl::Variable<double> > threads(nthreads);

    size_t index_start = atl::Variable<double>::tape.GetBlock(v.size());
    for (int i = 0; i < nthreads; i++) {
        int start = i*range;
        int end;
        if (i == nthreads - 1) {
            end = v.size();
        } else {
            end = ((i + 1) * range);
        }
        atl::thread_pool_g.DoJob(std::bind(ThreadFunction(start, end, v, threads[i], index_start)), wv);
    }

    atl::thread_pool_g.Wait(wv);

    atl::Variable<double> maxv;
    for (int i = 0; i < threads.size(); i++) {
        maxv = atl::ad_max(maxv, threads[i]);
        std::cout << maxv << "\n";
    }

    atl::WaitVariable wv2;

    //    std::vector<atl::Variable<double> > sums(nthreads);
    //    size_t block = atl::Variable<double>::tape.GetBlock(v.size()*2);
    //    size_t block_range = (v.size()*2) / nthreads;
    //    for (int i = 0; i < nthreads; i++) {
    //        int start = i*range;
    //        int estart = block + (block_range * i);
    //        int end;
    //        if (i == nthreads - 1) {
    //            end = v.size();
    //        } else {
    //            end = ((i + 1) * range);
    //        }
    //        atl::thread_pool_g.DoJob(std::bind(DivideThreadFunction(start, end, v, threads[i], sums[i], estart)), wv2);
    //    }
    //
    //    atl::thread_pool_g.Wait(wv2);


    std::cout << "Max Value is " << maxv << "\n";
    //
    for (int i = 0; i < v.size(); i++) {
        v[i] /= maxv;
    }

    atl::Variable<double> sum;
    for (int i = 0; i < v.size(); i++) {

        sum += v[i];
    }

    return sum;

}

inline const atl::Variable<double> ad_norm_and_sum(std::vector<atl::Variable<double> >& v) {
    atl::Variable<double> maxv;
    for (int i = 0; i < v.size(); i++) {
        maxv = atl::ad_max(maxv, v[i]);
    }
    std::cout << "Max Value is " << maxv << "\n";

    for (int i = 0; i < v.size(); i++) {
        v[i] /= maxv;

    }

    atl::Variable<double> sum;
    for (int i = 0; i < v.size(); i++) {

        sum += v[i];
    }

    return sum;

}

inline const atl::Variable<double> ad_min_max_test(int nvar, std::vector<atl::Variable<double> >& x, bool concurrent = false) {
    std::vector<atl::Variable<double> > X(nvar);
    for (int i = 0; i < x.size(); i++) {
        X[i] = atl::Variable<double>(x[i] * x[i]);
    }
    atl::Variable<double> v;

    if (!concurrent) {
        v = ad_norm_and_sum(X);
    } else {
        v = ad_norm_and_sum_threaded(X);
    }
    return v;
}

/*
 * 
 */
int main(int argc, char** argv) {
    
//    atl::Variable<double>::tape.recording = false;
//    atl::VariableMatrix<double> a(500, 500);
//    a = .01233;
//    atl::VariableMatrix<double> b(500, 500);
//    b = 2.0;
//
//    std::cout<<a(0,0).info->id<<" "<<a(0,1).info->id<<"\n";
//    
//    atl::VariableMatrix<double> c;
//    atl::Variable<double>::tape.recording = true;
//    std::cout << "Starting assign concurrent\n";
//    c.AssignConcurrent(atl::pow(a, b)*(a + b));
//
//    

//    while(true)
    atl::tests::auto_diff::Run(std::cout);
//    atl::tests::fmin::Run();
    exit(0);
    atl::Variable<double>::tape.Reset();
    atl::tests::auto_diff::LogTheta2<double> l;

    l.Initialize();
    std::valarray<std::valarray<double> > eh = l.EstimatedHessian();
    atl::Variable<double>::tape.derivative_trace_level = atl::SECOND_ORDER_REVERSE;

    atl::Variable<double> f;
    l.ObjectiveFunction(f);
    atl::Variable<double>::tape.AccumulateSecondOrder();
    std::cout << std::fixed;
    std::cout << "\n";
    std::cout << "\n";
    const std::vector<atl::Variable<double>*>& p = l.GetActiveParameters();
    for (int i = 0; i < p.size(); i++) {
        for (int j = 0; j < p.size(); j++) {
            std::cout << atl::Variable<double>::tape.Value(p[i]->info->id, p[j]->info->id) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "\n";

    for (int i = 0; i < p.size(); i++) {
        for (int j = 0; j < p.size(); j++) {
            std::cout << eh[i][j] << " ";
        }
        std::cout << "\n";
    }

    exit(0);
    //(std::cout);
    //    atl::Variable<std::complex<double> > a(std::complex<double>(12.0));
    //    atl::Variable<std::complex<double> > b(std::complex<double>(12.0));
    //    b.info->value.imag(1e-20);
    //    std::cout<<a<<", "<<b<<"\n";
    //    std::complex<double> dx = (atl::max(a, b)).EvaluateDerivative(a.info->id) / 1e-20;
    //    std::cout<<dx;
    exit(0);
    atl::Variable<double>::tape.derivative_trace_level = atl::THIRD_ORDER_REVERSE;
    //    atl::Variable<double> f;
    atl::Variable<double>::tape.recording = true;
    mytest<double> test(std::cout);
    //        std::cout<<std::fixed;
    //            test.Initialize();
    //            test.ObjectiveFunction(f);
    //            atl::Variable<double>::tape.AccumulateThirdOrder();
    //            std::cout << atl::Variable<double>::tape.Value(test.b.info->id,test.b.info->id,test.b.info->id)<<"\n";
    exit(0);

    std::cout << sizeof (atl::StackEntry<double>);
    //    exit(0);

    //    std::cout<<(20 % 10)<<"\n";
    //    atl::Variable<double> abs = -M_PI;
    //    atl::Variable<double> abs2 = M_PI;
    //    atl::Variable<double> ans = atl::min(abs,abs2);
    ////    std::cout << (atl::min(abs,abs2))EvaluateDerivative(abs.info->id,abs2.info->id) << "\n";
    ////    std::cout << (AD_MIN(abs,abs2))<< "\n";
    //    atl::Variable<double>::tape.Accumulate();
    //    std::cout << atl::Variable<double>::tape.first_order_derivtives[abs.info->id] << " ";
    //    std::cout << atl::Variable<double>::tape.first_order_derivtives[abs2.info->id] << " ";
    //    exit(0);
    //    //    
    //atl::Variable<double>::tape.stack.resize(100000000);
    std::vector<atl::Variable<double> > x(10);
    atl::Variable<double> xx = (double) x.size();

    for (int i = 0; i < x.size() - 1; i++) {
        x[i] = (double) (i + 1);
    }
    x[x.size() - 1] = (x.size() - 1);

    //    std::cout<<"x = ";
    //    for (int i = 0; i < x.size(); i++) {
    //        std::cout<<(*x[i])<<" ";
    //    }
    //    std::cout<<std::endl;
    time_point<Clock> start = Clock::now();
    atl::Variable<double> ret = (ad_min_max_test(x.size(), x, false));

    std::cout << atl::Variable<double>::tape.stack_current << "\n";
    //
    std::cout << "Accumulating...";
    atl::Variable<double>::tape.AccumulateSecondOrder();
    std::cout << "done." << std::endl;
    time_point<Clock> end = Clock::now();
    milliseconds diff = duration_cast<milliseconds>(end - start);
    std::cout << diff.count() << " ms" << std::endl;
    std::cout << "Gradient:\n";
    for (int i = 0; i < x.size(); i++) {
        std::cout << atl::Variable<double>::tape.first_order_derivatives[x[i].info->id] << " ";
    }

    std::cout << "\n\nHessian:\n";
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < x.size(); j++) {
            std::cout << atl::Variable<double>::tape.Value(x[i].info->id, x[j].info->id) << " ";
        }
        std::cout << "\n";
    }

    atl::Variable<double>::tape.Reset();
    //    std::cout << atl::Variable<double>::tape.first_order_derivtives[xx.info->id] << " ";

    //    atl::Variable<double> a(0);
    //    atl::Variable<double> b(0);
    //    atl::Variable<double> c = ad_min(a,b); //,b);
    //    //    atl::VariableMatrix<double> v(10);
    //    //    v = 10.0;
    //    //    for (int i = 0; i < v.GetRows(); i++) {
    //    //        v(i) = (double) i;
    //    }
    //    std::cout << v << "\n";
    //    atl::Variable<double> c;
    //    for (int i = 0; i < v.GetRows(); i++) {
    //        c = ad_min(b, a); //,b);
    //    }

    //    //    std::cout.precision(100);
    //    //    std::cout<<M_E<<"  "<<std::pow(1.0,2.0);
    //    //    exit(0);
    //    //    atl::Variable<double>::tape.
    //    size_t SIZE = 500;
    //    //    atl::Variable<double>::tape.stack.resize(5000000);
    //    std::cout << sizeof (atl::Variable<double>) << "\n";
    //    atl::VariableMatrix<double> a(SIZE, SIZE, 1.00012);
    //    atl::VariableMatrix<double > b(SIZE, SIZE, 22.0);
    //    atl::VariableMatrix<double > c; //(100, 100);
    //    //     atl::VariableMatrix<double > d;
    //    //     d.AssignConcurrent(a*b);
    //    atl::Variable<double> vv(3.1459);
    //    //    c.AssignConcurrent(/*((vv - 3.1459)*(vv - 3.1459))*((vv - 3.1459)*(vv - 3.1459))**/(a*b)*vv);
    //    //
    //    //    atl::Variable<double> sum;
    //    //    for (int i = 0; i < c.rows; i++) {
    //    //        for (int j = 0; j < c.columns; j++) {
    //    //            sum += c(i, j)*a(0,0);
    //    //        }
    //    //    }
    //
    //    //        for (int i = 0; i < 10; i++) {
    //    //            for (int j = 0; j < 10; j++) {
    //    //                for (int k = 0; k < 10; k++) {
    //    //                  
    //    //                c(i,j)= a(i,k)*b(k,j);
    //    //                }
    //    //            }
    //    //        }
    //
    //    atl::VariableVector<double> vec(10);
    //    vec = 1.0;
    //
    //    c.AssignConcurrent((atl::log(a) * b)*((vv - 3.1223)*(vv - 3.1223)) + a);
    //    //    c=c*((vv-3.1223)*(vv-3.1223))+a;
    //    atl::Variable<double> sum;
    //    atl::Variable<double> v2v =M_E;
    //
    //
    //    for (int i = 0; i < SIZE; i++) {
    //        for (int j = 0; j < SIZE; j++) {
    //            sum = sum + c(i, j) * a(i, j);
    //        }
    //    }
    //
    //
    //    sum *= atl::log(v2v);
    //    std::cout << std::fixed << "\n";
    //    std::cout << "sum = " << sum.GetValue() << "\n";
    //        std::cout<<c<<"\n";

    //    std::cout << atl::Variable<double>::tape.first_order_derivtives[a.info->id] << "\n";
    //    std::cout << atl::Variable<double>::tape.first_order_derivtives[b.info->id] << "\n";
    return 0;
}

