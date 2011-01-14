#ifndef __tvNR_tqli__
#define __tvNR_tqli__

#include "nrutil.h"

template <class T>
inline
T SIGN (T a, T b)
{
  return b < 0 ? -tvABS (a) : tvABS (a);
}


// determine the eigenvalues and eigenvectors of a real symmetric, 
// tridiagonal matrix, or of real, symmetric matrix previously reduced 
// by tvNR_tred2. On input, d[1..n] contains the diagonal elelments of the 
// tridiagonal matrix, On output, it returns the eigenvalues. The vector
// e[1..n] inputs the subdiagonal elements of the tridiagonal matrix, 
// with e[1] arbitrary. On output e is destroyed. z is input as the
// matrix output by tvNR_tred2. The kth column of z returns the normalised
// eigenvector corresponding to d[k]. 

template <class T>
void tvNR_tqli (T d[], T e[], int n, T** z)
{
	int     m, l, iter, i, k;
	T       s, r, p, g, f, dd, c, b;

	for (i = 2; i <= n; i++)
	  e[i-1] = e[i];
	e[n] = 0;

	for (l=1;l<=n;l++) {
		iter = 0;
		do {
			for (m=l;m<=n-1;m++) {
				dd = tvABS (d[m]) + tvABS (d[m+1]);
				if (tvABS (e[m]) + dd == dd) break;
			}
			if (m != l) {
				if (iter++ == 30)
				  tvNR_nrerror("Too many iterations in tvNR_tred2");
				g = (d[l+1]-d[l]) / (2.0 * e[l]);
				r = tvSQRT ((g*g) + 1);
				g = d[m] - d[l] + e[l] / (g + SIGN(r,g));
				s = c = 1;
				p = 0;
				for (i = m-1; i >= l; i--) {
					f = s * e[i];
					b = c * e[i];
					if (tvABS(f) >= tvABS(g)) {
						c       = g / f;
						r       = tvSQRT ((c * c) + 1);
						e[i+1]  = f * r;
						c      *= (s = 1.0 / r);
					} else {
						s = f / g;
						r = tvSQRT((s*s) + 1);
						e[i+1] = g * r;
						s *= (c = (T)(1.0 / r));
					}
					g = d[i+1] - p;
					r = (d[i]-g) * s + 2.0 * c * b;
					p = s * r;
					d[i+1] = g + p;
					g = c * r - b;
					/* Next loop can be omitted if eigenvectors not wanted */
					for ( k = 1; k <= n; k++) {
						f = z[k][i+1];
						z[k][i+1] = s * z[k][i] + c * f;
						z[k][i] = c * z[k][i] - s * f;
					}
				}
				d[l] = d[l] - p;
				e[l] = g;
				e[m] = 0.0;
			}
		} while (m != l);
	}
}

#endif
