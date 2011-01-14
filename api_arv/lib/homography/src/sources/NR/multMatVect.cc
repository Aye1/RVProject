#ifndef __tvNR_multMatVect__
#define __tvNR_multMatVect__

// res[1..r] returns the result of the multiplication between the matrix 
// m[1..r][1..c] with the vector [1..c].

template <class T>
void tvNR_multMatVect(T** m, T* v, T* res, int r, int c)
{
	int     i, j;
	T       som;
	
	for ( i = 1; i <= r; i++) {
		som = (T)0;

		for (j = 1; j <= c; j++)
			som += m[i][j] * v[j];
		res[i]=som;
	}
}

#endif
