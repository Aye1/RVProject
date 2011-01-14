#include <math.h>
#define NRANSI
#include "nrutil.h"

#define ROTATE(a,i,j,k,l) \
              g = a[i][j];\
              h = a[k][l];\
              a[i][j] = g - s * (h + g * tau);\
              a[k][l] = h + s * (g - h * tau);

template <class T> 
void tvNR_jacobi(T **a, int n, T *d, T **v, int *nrot)
{
 int j, iq, ip, i;
 T tresh, theta, tau, t, sm, s, h, g, c, *b, *z;

 //b = tvNR_vector(1, n);
 //z = tvNR_vector(1, n);
 tvNR_vector(b, 1, n);
 tvNR_vector(z, 1, n);

 for (ip = 1; ip <= n; ip++) {
  for (iq = 1; iq <= n; iq++) v[ip][iq] = (T) 0;
  v[ip][ip] = (T) 1;
 }

 for (ip = 1;ip <= n; ip++) {
  b[ip] = d[ip] = a[ip][ip];
  z[ip] = (T) 0;
 }

 *nrot = 0;

 for (i = 1; i <= 50; i++) {
  sm = (T) 0;
  for (ip = 1; ip <= n - 1; ip++) {
   for (iq = ip + 1; iq <= n; iq++)
    sm += tvABS(a[ip][iq]);
  }

  if (sm == (T) 0) {
   tvNR_free_vector(z, 1, n);
   tvNR_free_vector(b, 1, n);
   return;
  }

  if (i < 4) {
   tresh = (T) (0.2 * sm / (n * n));
  }
  else {
   tresh = (T) 0;
  }

  for (ip = 1; ip <= n - 1; ip++) {
   for (iq = ip + 1; iq <= n; iq++) {
    g = (T) (100.0 * tvABS(a[ip][iq]));

    if (i > 4 && (T)(tvABS(d[ip]) + g) == (T) tvABS(d[ip]) 
              && (T)(tvABS(d[iq]) + g) == (T) tvABS(d[iq])) {
     a[ip][iq] = (T) 0;
    } else if (tvABS(a[ip][iq]) > tresh) {
            h = d[iq] - d[ip];

            if ((T)(tvABS(h) + g) == (T) tvABS(h)) {
             t =(a[ip][iq]) / h;
            } else {
             theta = (T) (0.5 * h / (a[ip][iq]));
             t = (T) (1.0 / (tvABS(theta) + tvSQRT(1.0 + theta * theta)));
             if (theta < (T) 0) t = -t;
            }

            c = (T) (1.0 / tvSQRT(1.0 + t * t));
            s = t * c;
            tau = (T) (s / (1.0 + c));
            h = t * a[ip][iq];
            z[ip] -= h;
            z[iq] += h;
            d[ip] -= h;
            d[iq] += h;
            a[ip][iq] = (T) 0;

            for (j = 1; j <= ip - 1; j++) { ROTATE(a,j,ip,j,iq) }
            for (j = ip + 1; j <= iq - 1; j++) { ROTATE(a,ip,j,j,iq) }
            for (j = iq + 1; j <= n; j++) { ROTATE(a,ip,j,iq,j) }
            for (j = 1; j <= n; j++) { ROTATE(v,j,ip,j,iq) }

            ++(*nrot);
    }
   }
  }

  for (ip = 1; ip <= n; ip++) {
   b[ip] += z[ip];
   d[ip] = b[ip];
   z[ip] = (T) 0;
  }
 }
 tvNR_nrerror("Too many iterations in routine jacobi");
}

#undef ROTATE
#undef NRANSI
