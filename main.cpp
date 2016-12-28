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
using namespace std;

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

///**
// * Returns the maximum between a and b in a continuous manner using:
// * 
// * (a + b + |a.GetValue() - b.GetValue()|) / 2.0;
// * 
// * @param a
// * @param b
// * @return 
// */
//template <typename T>
//inline const atl::Variable<T> damax(const atl::Variable<T>& a, const atl::Variable<T>& b) {
//    return (a + b + atl::fabs(a - b)) / (2.0);
//}
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

inline const atl::Variable<double> ad_norm_and_sum(std::vector<atl::Variable<double> >& v) {
    atl::Variable<double> maxv;
    for (int i = 0; i < v.size(); i++) {
        maxv = atl::max(maxv, v[i]);
    }

    for (int i = 0; i < v.size(); i++) {
        v[i] /= maxv;
    }

    atl::Variable<double> sum;
    for (int i = 0; i < v.size(); i++) {

        sum += v[i];
    }

    return sum;

}

inline const atl::Variable<double> ad_min_max_test(int nvar, std::vector<atl::Variable<double> >& x) {
    std::vector<atl::Variable<double> > X(nvar);
    for (int i = 0; i < x.size(); i++) {
        X[i] = atl::Variable<double>(x[i] * x[i]);
    }
    return ad_norm_and_sum(X);

}

/*
 * 
 */
int main(int argc, char** argv) {


    std::vector<atl::Variable<double> > x(10000);
    for (int i = 0; i < x.size(); i++) {
        x[i] = (double) (i + 1);
    }

    atl::Variable<double> ret = ad_min_max_test(x.size(), x);

    atl::Variable<double>::tape.Accumulate();
    std::cout << "Gradient:\n";
    for (int i = 0; i < x.size(); i++) {
        std::cout << atl::Variable<double>::tape.first_order_derivtives[x[i].info->id] << " ";
    }


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

