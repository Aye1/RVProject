#ifndef __NR__
#define __NR__

template <class T>
void tvNR_multMatVect(T** m, T* v, T* res, int r, int c);

template <class T>
void tvNR_multMatrice (T** a, T** b, T** res, int r, int c);

template <class T>
void tvNR_multMatVect(T** m, T* v, T* res, int r, int c);

template <class T>
void tvNR_multCstVect(T k, T* v, T* res, int l);

template <class T>
void tvNR_multCstMat (T k, T** m, T** res, int r, int c);

template <class T>
void tvNR_transpose (T** a, T** res, int r, int c);

template <class T>
void tvNR_tred2 (T** a, int n, T d[], T e[]);

template <class T>
void tvNR_tqli (T d[], T e[], int n, T** z);

#include "multMatVect.cc"
#include "multMatrice.cc"
#include "multCstVect.cc"
#include "multCstMat.cc"
#include "transpose.cc"
#include "nrutil.cc"
#include "tqli.cc"
#include "tred2.cc"

#endif
