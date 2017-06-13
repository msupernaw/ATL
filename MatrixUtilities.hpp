/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MatrixUtilities.hpp
 * Author: matthewsupernaw
 *
 * Created on May 25, 2017, 10:01 AM
 */

#ifndef MATRIXUTILITIES_HPP
#define MATRIXUTILITIES_HPP
#include "Matrix.hpp"

namespace atl {

    template<typename REAL_T>
    struct VariableMatrixRow : atl::ExpressionBase<REAL_T, VariableMatrixRow<REAL_T> > {
        atl::VariableMatrix<REAL_T>& mat;
        size_t row;

        VariableMatrixRow(atl::VariableMatrix<REAL_T>& mat, size_t row) :
        mat(mat), row(row) {
        }

        atl::Variable<REAL_T>& operator()(size_t i) {
            return mat(row, i);
        }

        
        inline const REAL_T GetValue() const {
            return mat(this->row,0).GetValue();
        }

        inline const REAL_T GetValue(size_t i, size_t j = 0) const {
            return mat(this->row,j).GetValue();
        }

        inline bool IsNonlinear() const {
            return false;
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids)const {
            mat(this->row,0).PushIds(ids);
        }

        inline void PushNLIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, bool nl = false)const {
            mat(this->row,0).PushIds(ids, nl);
        }

        inline void PushIds(typename atl::StackEntry<REAL_T>::vi_storage& ids, size_t i, size_t j = 0)const {
            mat(this->row,j).PushIds(ids, row, j);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, REAL_T h = 1e-20) const {
            return mat(this->row,0).ComplexEvaluate(x, h);
        }

        std::shared_ptr<DynamicExpressionBase<REAL_T> > ToDynamic() const {
            return mat(this->row,0).ToDynamic();
        }

        inline REAL_T EvaluateDerivative(uint32_t x) const {
            return mat(this->row,0).EvaluateDerivative(x);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y) const {
            return mat(this->row,0).EvaluateDerivative(x, y);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z) const {
            return mat(this->row,0).EvaluateDerivative(x, y, z);
        }

        inline const std::complex<REAL_T> ComplexEvaluate(uint32_t x, size_t i, size_t j = 0, REAL_T h = 1e-20) const {
            return mat(this->row,0).ComplexEvaluate(x, i, j, h);
        }

        inline const REAL_T Taylor(uint32_t degree) const {
            return mat(this->row,0).Taylor(degree);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, size_t i, size_t j = 0) const {
            return mat(this->row,j).EvaluateDerivative(x, row, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, size_t i, size_t j = 0) const {
            return mat(this->row,j).EvaluateDerivative(x, y, row, j);
        }

        inline REAL_T EvaluateDerivative(uint32_t x, uint32_t y, uint32_t z, size_t i, size_t j = 0) const {
            return mat(this->row,j).EvaluateDerivative(x, y, z, row, j);
        }

       
        size_t GetColumns() const {
            return mat.GetColumns();
        }

        size_t GetRows() const {
            return 1;
        }

        bool IsScalar()const {
            return false;
        }

        /**
         * Create a string representation of this expression template. 
         * @return 
         */
        std::string ToExpressionTemplateString() const {
            return "row_vector";
        }


    };

    template<typename REAL_T>
    struct VariableMatrixRow<REAL_T> Row(const atl::VariableMatrix<REAL_T>& mat, size_t row) {
        return VariableMatrixRow<REAL_T>(mat, row);
    }

    template<typename REAL_T>
    struct VariableMatrixColumn : atl::ExpressionBase<REAL_T, VariableMatrixColumn<REAL_T> > {
        atl::VariableMatrix<REAL_T>& mat;
        size_t column;

        VariableMatrixColumn(atl::VariableMatrix<REAL_T>& mat, size_t column) :
        mat(mat), column(column) {
        }

        atl::Variable<REAL_T>& operator()(size_t i) {
            return mat(i, column);
        }


    };

    template<typename REAL_T>
    struct VariableMatrixColumn<REAL_T> Column(const atl::VariableMatrix<REAL_T>& mat, size_t col) {
        return VariableMatrixColumn<REAL_T>(mat, col);
    }


}


#endif /* MATRIXUTILITIES_HPP */

