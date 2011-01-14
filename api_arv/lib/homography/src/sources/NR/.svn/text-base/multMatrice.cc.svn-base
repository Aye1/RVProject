#ifndef __tvNR_multMatrice__
#define __tvNR_multMatrice__

// res[1..r][1..r] returns the result of the multiplication between the
// matrix a[1..r][1..c] with the natrix b[1..c][1..r].

template <class T>
void tvNR_multMatrice (T** a, T** b, T** res, int r, int c)
{
	int     i, j, k;
	T       som;
	
	for (i = 1; i <= r; i++)
		for (j = 1; j <= r; j++) {
			som = (T)0;
			for (k = 1; k <= c; k++)
				som += a[i][k] * b[k][j];
			res[i][j] = som;
		}
}

#endif

