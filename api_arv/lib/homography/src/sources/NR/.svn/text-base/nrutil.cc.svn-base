

template <class T>
void tvNR_vector (T*& v, int nl, int nh)
{
	v = (T*)malloc ((unsigned)(nh-nl+1)*sizeof(T));
	if (v == (T*)NULL)
	  return;
	else
    v -= nl;
}


template <class T>
void tvNR_matrix (T**& m, int nrl, int nrh, int ncl, int nch)
{
	int     i;

	m = (T**)malloc ((unsigned)(nrh-nrl+1)*sizeof(T*));

	if (m == (T**)NULL)
	  return;

	m -= nrl;

	for (i = nrl; i <= nrh; i++)
	  m[i] = (T*)NULL;

	for (i = nrl; i <= nrh; i++) {
		m[i] = (T*)malloc ((unsigned)(nch-ncl+1)*sizeof(T));

		if (m[i] != (T*)NULL)
  	  m[i] -= ncl;
  	else {

		  tvNR_free_matrix (m, nrl, nrh, ncl, nch);
		  return;
		}
	}
}



template <class T>
void tvNR_free_vector (T*& v, int nl, int /*nh*/)
{
	free((char*)(v+nl));
	v = (T*)NULL;
}


template <class T>
void tvNR_free_matrix (T**& m, int nrl, int nrh, int ncl, int /*nch*/)
{
	int i;

  if (m != (T**)NULL) {
  	for (i = nrh; i >= nrl; i--)
  	  if (m[i] != (T*)NULL)
  	    free ((char*)(m[i]+ncl));

  	free ((char*)(m+nrl));
  	m = (T**)NULL;
  }
}
