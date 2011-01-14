#ifndef __tvNR_multCstMat__
#define __tvNR_multCstMat__

// res[1..r][1..c] returns the result of the multiplication between the matrix 
// m[1..r][1..c] with the real k.

template <class T>
void tvNR_multCstMat (T k, T** m, T** res, int r, int c)
{
	int   i, j;
	
	for (i = 1; i <= r; i++)
		for (j = 1; j <= c; j++)
			res[i][j] = m[i][j] * k;
}

#endif

