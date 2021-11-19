#ifndef _CS_SPARSE_HPP
#define _CS_SPARSE_HPP


#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <cmath>
#include <stdio.h>
#include <stddef.h>
#include <thread>
#include <vector>

//#include "../Variable.hpp"

#define CS_VER 3                    /* CSparse Version */
#define CS_SUBVER 1
#define CS_SUBSUB 7
#define CS_DATE "Feb 1, 2016"       /* CSparse release date */
#define CS_COPYRIGHT "Copyright (c) Timothy A. Davis, 2006-2016"

/**
 * Edited by Matthew Supernaw to allow for template arguments.
 */

//using namespace atl;

#ifdef G
#undef G
#endif

#ifndef csi
#define csi ptrdiff_t
#endif

/* --- primary CSparse routines and data structures ------------------------- */
template<typename Type>
struct cs_sparse /* matrix in compressed-column or triplet form */ {
    csi nzmax; /* maximum number of entries */
    csi m; /* number of rows */
    csi n; /* number of columns */
    csi *p; /* column pointers (size n+1) or col indices (size nzmax) */
    csi *i; /* row indices, size nzmax */
    Type *x; /* numerical values, size nzmax */
    csi nz; /* # of entries in triplet matrix, -1 for compressed-col */
};



template<typename Type>
cs_sparse<Type> *cs_add(const cs_sparse<Type> *A, const cs_sparse<Type> *B, Type alpha, Type beta);
template<typename Type>
csi cs_cholsol(csi order, const cs_sparse<Type> *A, Type *b);
template<typename Type>
cs_sparse<Type> *cs_compress(const cs_sparse<Type> *T);
template<typename Type>
csi cs_dupl(cs_sparse<Type> *A);
template<typename Type>
csi cs_entry(cs_sparse<Type> *T, csi i, csi j, const Type& x);
template<typename Type>
csi cs_gaxpy(const cs_sparse<Type> *A, const Type *x, Type *y);
template<typename Type>
cs_sparse<Type> *cs_load(FILE *f);
template<typename Type>
csi cs_lusol(csi order, const cs_sparse<Type> *A, Type *b, Type tol);
template<typename Type>
cs_sparse<Type> *cs_multiply(const cs_sparse<Type> *A, const cs_sparse<Type> *B);
template<typename Type>
Type cs_norm(const cs_sparse<Type> *A);
template<typename Type>
csi cs_print(const cs_sparse<Type> *A, csi brief);
template<typename Type>
csi cs_qrsol(csi order, const cs_sparse<Type> *A, Type *b);
template<typename Type>
cs_sparse<Type> *cs_transpose(const cs_sparse<Type> *A, csi values);
/* utilities */
void *cs_calloc(csi n, size_t size);
void *cs_free(void *p);
void *cs_realloc(void *p, csi n, size_t size, csi *ok);
template<typename Type>
cs_sparse<Type> *cs_spalloc(csi m, csi n, csi nzmax, csi values, csi triplet);
template<typename Type>
cs_sparse<Type> *cs_spfree(cs_sparse<Type> *A);
template<typename Type>
csi cs_sprealloc(cs_sparse<Type> *A, csi nzmax);
void *cs_malloc(csi n, size_t size);

template<typename Type>
csi cs_lsolve_x(const cs_sparse<Type> *L, Type *x, int i);

/* solve L'x=b where x and b are dense.  x=b on input, solution on output. */
template<typename Type>
csi cs_ltsolve_x(const cs_sparse<Type> *L, Type *x, int i);

/* --- secondary CSparse routines and data structures ----------------------- */
template<typename Type>
struct cs_symbolic /* symbolic Cholesky, LU, or QR analysis */ {
    csi *pinv; /* inverse row perm. for QR, fill red. perm for Chol */
    csi *q; /* fill-reducing column permutation for LU and QR */
    csi *parent; /* elimination tree for Cholesky and QR */
    csi *cp; /* column pointers for Cholesky, row counts for QR */
    csi *leftmost; /* leftmost[i] = min(find(A(i,:))), for QR */
    csi m2; /* # of rows for QR, after adding fictitious rows */
    Type lnz; /* # entries in L for LU or Cholesky; in V for QR */
    Type unz; /* # entries in U for LU; in R for QR */
};

template<typename Type>
struct cs_numeric /* numeric Cholesky, LU, or QR factorization */ {
    cs_sparse<Type> *L; /* L for LU and Cholesky, V for QR */
    cs_sparse<Type> *U; /* U for LU, R for QR, not used for Cholesky */
    csi *pinv; /* partial pivoting for LU */
    Type *B; /* beta [0..n-1] for QR */
};

template<typename Type>
struct cs_dmperm_results /* cs_dmperm or cs_scc output */ {
    csi *p; /* size m, row permutation */
    csi *q; /* size n, column permutation */
    csi *r; /* size nb+1, block k is rows r[k] to r[k+1]-1 in A(p,q) */
    csi *s; /* size nb+1, block k is cols s[k] to s[k+1]-1 in A(p,q) */
    csi nb; /* # of blocks in fine dmperm decomposition */
    csi rr [5]; /* coarse row decomposition */
    csi cc [5]; /* coarse column decomposition */
};

template<typename Type>
csi *cs_amd(csi order, const cs_sparse<Type> *A);
template<typename Type>
cs_numeric<Type> *cs_chol(const cs_sparse<Type> *A, const cs_symbolic<Type> *S);
template<typename Type>
cs_dmperm_results<Type> *cs_dmperm(const cs_sparse<Type> *A, csi seed);
template<typename Type>
csi cs_droptol(cs_sparse<Type> *A, Type tol);
template<typename Type>
csi cs_dropzeros(cs_sparse<Type> *A);
template<typename Type>
csi cs_happly(const cs_sparse<Type> *V, csi i, Type beta, Type *x);
template<typename Type>
csi cs_ipvec(const csi *p, const Type *b, Type *x, csi n);
template<typename Type>
csi cs_lsolve(const cs_sparse<Type> *L, Type *x);
template<typename Type>
csi cs_ltsolve(const cs_sparse<Type> *L, Type *x, bool concurrent = false);
template<typename Type>
cs_numeric<Type> *cs_lu(const cs_sparse<Type> *A, const cs_symbolic<Type> *S, Type tol);
template<typename Type>
cs_sparse<Type> *cs_permute(const cs_sparse<Type> *A, const csi *pinv, const csi *q, csi values);

csi *cs_pinv(const csi *p, csi n);
template<typename Type>
csi cs_pvec(const csi *p, const Type *b, Type *x, csi n);
template<typename Type>
cs_numeric<Type> *cs_qr(const cs_sparse<Type> *A, const cs_symbolic<Type> *S);
template<typename Type>
cs_symbolic<Type> *cs_schol(csi order, const cs_sparse<Type> *A);
template<typename Type>
csi cs_cholsol(csi order, const cs_sparse<Type> *A, Type *b, cs_symbolic<Type> *S);
template<typename Type>
csi cs_cholsol(csi order, const cs_sparse<Type> *A, Type *b, cs_numeric<Type> *N, cs_symbolic<Type> *S);
template<typename Type>
csi cs_cholsol_x(csi order, const cs_sparse<Type> *A, Type *b, cs_numeric<Type> *N, cs_symbolic<Type> *S, Type *x, int i);
template<typename Type>
cs_symbolic<Type> *cs_sqr(csi order, const cs_sparse<Type> *A, csi qr);
template<typename Type>
cs_sparse<Type> *cs_symperm(const cs_sparse<Type> *A, const csi *pinv, csi values);
template<typename Type>
csi cs_updown(cs_sparse<Type> *L, csi sigma, const cs_sparse<Type> *C, const csi *parent);
template<typename Type>
csi cs_usolve(const cs_sparse<Type> *U, Type *x);
template<typename Type>
csi cs_utsolve(const cs_sparse<Type> *U, Type *x);
/* utilities */
template<typename Type>
cs_symbolic<Type> *cs_sfree(cs_symbolic<Type> *S);
template<typename Type>
cs_numeric<Type> *cs_nfree(cs_numeric<Type> *N);
template<typename Type>
cs_dmperm_results<Type> *cs_dfree(cs_dmperm_results<Type> *D);

/* --- tertiary CSparse routines -------------------------------------------- */
template<typename Type>
csi *cs_counts(const cs_sparse<Type> *A, const csi *parent, const csi *post, csi ata);
template<typename Type>
Type cs_cumsum(csi *p, csi *c, csi n);
template<typename Type>
csi cs_dfs(csi j, cs_sparse<Type> *G, csi top, csi *xi, csi *pstack, const csi *pinv);
template<typename Type>
csi cs_ereach(const cs_sparse<Type> *A, csi k, const csi *parent, csi *s, csi *w);
template<typename Type>
csi *cs_etree(const cs_sparse<Type> *A, csi ata);
template<typename Type>
csi cs_fkeep(cs_sparse<Type> *A, csi(*fkeep) (csi, csi, Type, void *), void *other);
template<typename Type>
Type cs_house(Type *x, Type *beta, csi n);
template<typename Type>
csi cs_leaf(csi i, csi j, const csi *first, csi *maxfirst, csi *prevleaf,
        csi *ancestor, csi *jleaf);
template<typename Type>
csi *cs_maxtrans(const cs_sparse<Type> *A, csi seed);
template<typename Type>
csi *cs_post(const csi *parent, csi n);
template<typename Type>
csi *cs_randperm(csi n, csi seed);
template<typename Type>
csi cs_reach(cs_sparse<Type> *G, const cs_sparse<Type> *B, csi k, csi *xi, const csi *pinv);
template<typename Type>
csi cs_scatter(const cs_sparse<Type> *A, csi j, Type beta, csi *w, Type *x, csi mark,
        cs_sparse<Type> *C, csi nz);
template<typename Type>
cs_dmperm_results<Type> *cs_scc(cs_sparse<Type> *A);
template<typename Type>
csi cs_spsolve(cs_sparse<Type> *G, const cs_sparse<Type> *B, csi k, csi *xi, Type *x,
        const csi *pinv, csi lo);

csi cs_tdfs(csi j, csi k, csi *head, const csi *next, csi *post,
        csi *stack);
/* utilities */
template<typename Type>
cs_dmperm_results<Type> *cs_dalloc(csi m, csi n);
template<typename Type>
cs_dmperm_results<Type> *cs_ddone(cs_dmperm_results<Type> *D, cs_sparse<Type> *C, void *w, csi ok);
template<typename Type>
cs_sparse<Type> *cs_done(cs_sparse<Type> *C, void *w, void *x, csi ok);
template<typename Type>
csi *cs_idone(csi *p, cs_sparse<Type> *C, void *w, csi ok);
template<typename Type>
cs_numeric<Type> *cs_ndone(cs_numeric<Type> *N, cs_sparse<Type> *C, void *w, void *x, csi ok);
template<typename Type>
Type cs_det(cs_sparse<Type>* A);
template<typename Type>
Type cs_log_det(cs_sparse<Type>* A);

template<typename Type>
csi sparseinv /* returns -1 on error, or flop count if OK */
(
        /* inputs, not modified on output: */
        csi n, /* L, U, D, and Z are n-by-n */

        csi *Lp, /* L is sparse, lower triangular, stored by column */
        csi *Li, /* the row indices of L must be sorted */
        Type *Lx, /* diagonal of L, if present, is ignored */

        Type *d, /* diagonal of D, of size n */

        csi *Up, /* U is sparse, upper triangular, stored by row */
        csi *Uj, /* the column indices of U need not be sorted */
        Type *Ux, /* diagonal of U, if present, is ignored */

        csi *Zp, /* Z is sparse, stored by column */
        csi *Zi, /* the row indices of Z must be sorted */

        /* output, not defined on input: */
        Type *Zx,

        /* workspace: */
        Type *z, /* size n, zero on input, restored as such on output */
        csi *Zdiagp, /* size n */
        csi *Lmunch /* size n */
        );


#define CS_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define CS_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define CS_FLIP(i) (-(i)-2)
#define CS_UNFLIP(i) (((i) < 0) ? CS_FLIP(i) : (i))
#define CS_MARKED(w,j) (w [j] < 0)
#define CS_MARK(w,j) { w [j] = CS_FLIP (w [j]) ; }
#define CS_CSC(A) (A && (A->nz == -1))
#define CS_TRIPLET(A) (A && (A->nz >= 0))


#endif

