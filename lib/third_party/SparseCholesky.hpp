/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   SparseCholesky.hpp
 * Author: mattadmin
 *
 * Created on December 13, 2019, 4:30 PM
 */

#ifndef SPARSECHOLESKY_HPP
#define SPARSECHOLESKY_HPP

typedef size_t csi;

//all this code was ripped off and modified from the csparse library to work with atl::Variable types


namespace atl {

    template<typename Type>
    struct Sparse /* matrix in compressed-column or triplet form */ {
        csi nzmax; /* maximum number of entries */
        csi m; /* number of rows */
        csi n; /* number of columns */
        std::vector<csi> p; /* column pointers (size n+1) or col indices (size nzmax) */
        std::vector<csi> i; /* row indices, size nzmax */
        std::vector<Type> x; /* numerical values, size nzmax */
        csi nz; /* # of entries in triplet matrix, -1 for compressed-col */
    };

    template<typename Type>
    struct cs_symbolic /* symbolic Cholesky, LU, or QR analysis */ {
        std::vector<csi> pinv; /* inverse row perm. for QR, fill red. perm for Chol */
        std::vector<csi> q; /* fill-reducing column permutation for LU and QR */
        std::vector<csi> parent; /* elimination tree for Cholesky and QR */
        std::vector<csi> cp; /* column pointers for Cholesky, row counts for QR */
        std::vector<csi> leftmost; /* leftmost[i] = min(find(A(i,:))), for QR */
        csi m2; /* # of rows for QR, after adding fictitious rows */
        Type lnz; /* # entries in L for LU or Cholesky; in V for QR */
        Type unz; /* # entries in U for LU; in R for QR */
    };

    /*
     
     
     template<typename Type>
csi cs_entry(cs_sparse<Type> *T, csi i, csi j, const Type& x) {
    if (!CS_TRIPLET(T) || i < 0 || j < 0) return (0); /* check inputs 
    if (T->nz >= T->nzmax && !cs_sprealloc(T, 2 * (T->nzmax))) return (0);
    if (T->x) T->x [T->nz] = x;
    T->i [T->nz] = i;
    T->p [T->nz++] = j;
    T->m = CS_MAX(T->m, i + 1);
    T->n = CS_MAX(T->n, j + 1);
    return (1);
}
     */
    template<typename Type>
    void cs_entry(Sparse<Type>& T, csi i, csi j, const Type& x) {

        if (T.nz >= T.nzmax) {
            T.i.resize(2 * T.nzmax);
            T.p.resize(2 * T.nzmax);
            T.x.resize(2 * T.nzmax);
            T.nzmax = T.x.size();
        }

        T.x [T.nz] = x;
        T.i [T.nz] = i;
        T.p [T.nz++] = j;
        T.m = std::max(T.m, i + 1);
        T.n = std::max(T.n, j + 1);
    }



}




#endif /* SPARSECHOLESKY_HPP */

