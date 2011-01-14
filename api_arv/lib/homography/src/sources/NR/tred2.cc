#ifndef __tvNR_tred2__
#define __tvNR_tred2__

#include "nrutil.h"

// Housholder reduction of a real symmetric matrix a[1..n][1..n]. On 
// output, a is replaced by the orthogonal matrix Q effecting the
// transformation. d[1..n] returns the diagonal elements of the 
// tridiagonal matrix, and e[1..n] the off-diagonal elements, with e[1]=0.

template <class T>
void tvNR_tred2 (T** a, int n, T d[], T e[])
{
	int     l, k, j, i;
	T       scale, hh, h, g, f;

	for (i = n; i >= 2; i--) {
		l = i-1;
		h = scale = 0;
		if (l > 1) {
			for (k = 1; k <= l; k++)
				scale += tvABS (a[i][k]);
			if (scale == 0)
				e[i] = a[i][l];
			else {
				for (k = 1; k <= l; k++) {
					a[i][k] /= scale;
					h += a[i][k] * a[i][k];
				}
				f = a[i][l];
				g = f > 0 ? -tvSQRT (h) : tvSQRT (h);
				e[i] = scale * g;
				h -= f * g;
				a[i][l] = f - g;
				f = 0;
				for (j = 1; j <= l; j++) {
				  /* Next statement can be omitted if eigenvectors not wanted */
					a[j][i] = a[i][j] / h;
					g = 0;
					for (k = 1; k <= j; k++)
						g += a[j][k] * a[i][k];
					for (k = j + 1; k <= l; k++)
						g += a[k][j] * a[i][k];
					e[j] = g / h;
					f += e[j] * a[i][j];
				}
				hh = f / (h+h);
				for (j = 1; j <= l; j++) {
					f = a[i][j];
					e[j] = g = e[j] - hh * f;
					for (k = 1; k <= j; k++)
						a[j][k] -= (f * e[k] + g * a[i][k]);
				}
			}
		} else
			e[i] = a[i][l];
		d[i] = h;
	}
	/* Next statement can be omitted if eigenvectors not wanted */
	d[1] = 0;
	e[1] = 0;
	/* Contents of this loop can be omitted if eigenvectors not
			wanted except for statement d[i]=a[i][i]; */
	for (i = 1; i <= n; i++) {
		l = i - 1;
		if (d[i]) {
			for (j = 1; j <= l; j++) {
				g = 0;
				for (k = 1; k <= l; k++)
					g += a[i][k] * a[k][j];
				for (k = 1; k <= l; k++)
					a[k][j] -= g * a[k][i];
			}
		}
		d[i] = a[i][i];
		a[i][i] = 1;
		for (j = 1; j <= l; j++)
		  a[j][i] = a[i][j] = 0;
	}
}

#endif
