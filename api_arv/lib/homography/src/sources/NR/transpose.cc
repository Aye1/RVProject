#ifndef __tvNR_transpose__
#define __tvNR_transpose__

// res[1..c][1..r] returns the transposed matrix of a[1..r][1..c]

template <class T>
void tvNR_transpose (T** a, T** res, int r, int c)
{
	int i, j;
	
	for (i = 1; i <= r; i++)
		 for (j = 1; j <= c; j++)
		 	res[j][i] = a[i][j];
}

#endif
