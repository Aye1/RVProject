// File       : UTransform.cc
// Creation   : 03/1999
// Creator    : M. PRODHOMME
// Goal       : defines the Transform class, see "UProjective.h".
//
// Tab        : 2


// project includeProj
#include "UProjective.h"
#include <stdlib.h>

#include <math.h>
#include "NR/nr.h"
#include "NR/nrutil.h"



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        typedefs, consts & globals                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        TProjective class definition                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//
//    Init
//
ErrorNumber TProjective::Init ()
{
  // init data members
  fProjectionMatrix     = (double**)NULL;
  fInvProjectionMatrix  = (double**)NULL;
  fA                    = (double**)NULL;
  fAt                   = (double**)NULL;
  fAtA                  = (double**)NULL;
  fd                    = (double*)NULL;
  fe                    = (double*)NULL;
  fn                    = 0;
  
  // allocate fixed size arrays
  tvNR_matrix (fProjectionMatrix, 1,3,1,3);
  tvNR_matrix (fInvProjectionMatrix, 1,3,1,3);
  tvNR_matrix (fAtA, 1,9,1,9);
  tvNR_vector (fd, 1,9);
  tvNR_vector (fe, 1,9);

  if ((fProjectionMatrix == (double**)NULL)    || 
      (fInvProjectionMatrix == (double**)NULL) ||
      (fAtA == (double**)NULL)                 ||
      (fd == (double*)NULL)                    ||
      (fe == (double*)NULL)) {

    this->Dispose ();
    return kErrMemFull;

  } else
    return kErrNoErr;
}



//
//    Dispose
//
void TProjective::Dispose ()
{
  if (fA != (double**)NULL) {
    tvNR_free_matrix (fA,1,2*fn,1,9);
    fA=(double**)NULL;
  }
  if (fAt != (double**)NULL) {
    tvNR_free_matrix (fAt,1,9,1,2*fn);
    fAt=(double**)NULL;
  }
  fn = 0;
  if (fAtA != (double**)NULL) {
    tvNR_free_matrix (fAtA,1,9,1,9);
    fAtA=(double**)NULL;
  }
  if (fProjectionMatrix != (double**)NULL) {
    tvNR_free_matrix (fProjectionMatrix,1,3,1,3);
    fProjectionMatrix=(double**)NULL;
  }
  if (fInvProjectionMatrix != (double**)NULL) {
    tvNR_free_matrix (fInvProjectionMatrix,1,3,1,3);
    fInvProjectionMatrix=(double**)NULL;
  }
  if (fd != (double*)NULL) {
    tvNR_free_vector (fd,1,9);
  }
  if (fe != (double*)NULL) {
    tvNR_free_vector (fe,1,9);
  }
}

//
//    Compute
//
ErrorNumber TProjective::Compute (double *points, int n)
{
  int i, indexEigenValue;
  
  if (n>fn) {

    if (fA != (double**)NULL)
      tvNR_free_matrix (fA,1,2*fn,1,9);
    tvNR_matrix (fA, 1,2*n,1,9);

    if (fAt != (double**)NULL)
      tvNR_free_matrix (fAt,1,9,1,2*fn);
    tvNR_matrix (fAt, 1,9,1,2*n);

    if ((fA == (double**)NULL) || (fA == (double**)NULL))
      return kErrMemFull;
    fn=n;
  }

  // creation of the matrix fA
  for (i=1; i<=n; i++) {
    fA[2*i-1][1]=points[4*i-4];
    fA[2*i-1][2]=points[4*i-3];
    fA[2*i-1][3]=1;
    fA[2*i-1][4]=0;
    fA[2*i-1][5]=0;
    fA[2*i-1][6]=0;
    fA[2*i-1][7]=-points[4*i-2]*points[4*i-4];
    fA[2*i-1][8]=-points[4*i-2]*points[4*i-3];
    fA[2*i-1][9]=-points[4*i-2];
    
    
    fA[2*i][1]=0;
    fA[2*i][2]=0;
    fA[2*i][3]=0;
    fA[2*i][4]=points[4*i-4];
    fA[2*i][5]=points[4*i-3];
    fA[2*i][6]=1;
    fA[2*i][7]=-points[4*i-1]*points[4*i-4];
    fA[2*i][8]=-points[4*i-1]*points[4*i-3];
    fA[2*i][9]=-points[4*i-1];    
  }
  
  // calcul of fAt*fA
  tvNR_transpose (fA,fAt,2*n,9);
  tvNR_multMatrice (fAt,fA,fAtA,9,2*n);   
  
  // reduction of AtA to tridiagonal f||m
  tvNR_tred2 (fAtA, 9, fd, fe);

  // calcul of eigenvalues and eigenvect||s of AtA
  tvNR_tqli (fd, fe, 9, fAtA);
  
  indexEigenValue= this->LeastEigenValue(fd,9);
  
  //  calcul of the projection matrix
//  fProjectionMatrix=tvNR_matrix(1,3,1,3);
//  fInvProjectionMatrix=tvNR_matrix(1,3,1,3);
  
  fProjectionMatrix[1][1]=fAtA[1][indexEigenValue];
  fProjectionMatrix[1][2]=fAtA[2][indexEigenValue];
  fProjectionMatrix[1][3]=fAtA[3][indexEigenValue];
  fProjectionMatrix[2][1]=fAtA[4][indexEigenValue];
  fProjectionMatrix[2][2]=fAtA[5][indexEigenValue];
  fProjectionMatrix[2][3]=fAtA[6][indexEigenValue];
  fProjectionMatrix[3][1]=fAtA[7][indexEigenValue];
  fProjectionMatrix[3][2]=fAtA[8][indexEigenValue];
  fProjectionMatrix[3][3]=fAtA[9][indexEigenValue]; 
    
  return this->InvertProjMatrix ();
}



//
//    Transform
//
void  TProjective::Transform (double *initialPoint, double *destPoint)
{
  double initialPt[3],destPt[3];
  
  initialPt[0]=initialPoint[0];
  initialPt[1]=initialPoint[1];
  initialPt[2]=1;
  tvNR_multMatVect(fProjectionMatrix, (initialPt-1), (destPt-1), 3,3);
  tvNR_multCstVect(1/destPt[2],(destPt-1),(destPt-1),3);
  destPoint[0]=destPt[0];
  destPoint[1]=destPt[1];
}

//
//    InvTransform
//

void  TProjective::InvTransform (double *destPoint, double *initialPoint)
{
  double destPt[3],initialPt[3];
  
  destPt[0]=destPoint[0];
  destPt[1]=destPoint[1];
  destPt[2]=1;
  tvNR_multMatVect(fInvProjectionMatrix, (destPt-1), (initialPt-1), 3,3);
  tvNR_multCstVect(1/initialPt[2],(initialPt-1),(initialPt-1),3);
  initialPoint[0]=initialPt[0];
  initialPoint[1]=initialPt[1];
}




// SetFromDoublelist --

ErrorNumber TProjective::SetFromDoublelist (double* params)
{
  int i, j, k;

  k = 0;
  for (i = 1; i < 4; i++)
    for (j = 1; j < 4; j++)
      fProjectionMatrix[i][j] = params[k++];

  return this->InvertProjMatrix ();
}




// GetIntoDoublelist --

void TProjective::GetIntoDoublelist (double* params)
{
  int i, j, k;

  k = 0;
  for (i = 1; i < 4; i++)
    for (j = 1; j < 4; j++)
      params[k++] = fProjectionMatrix[i][j];
}



//
//    LeastEigenValue
//
int TProjective::LeastEigenValue(double *d, int l)
{
  int index=1, i; 
  double min=d[1];
  
  for (i=2; i<=l; i++){
    if (tvABS (d[i]) < min){
      min = tvABS (d[i]);
      index=i;
    }
  }
  return index;
}


// InvertProjMatrix --
//
//    Inverts the projection matrix and stores the result.

ErrorNumber TProjective::InvertProjMatrix ()
{
  double det =
    fProjectionMatrix[1][1]*fProjectionMatrix[2][2]*fProjectionMatrix[3][3] -
    fProjectionMatrix[1][1]*fProjectionMatrix[2][3]*fProjectionMatrix[3][2] -
    fProjectionMatrix[2][1]*fProjectionMatrix[1][2]*fProjectionMatrix[3][1] +
    fProjectionMatrix[2][1]*fProjectionMatrix[1][2]*fProjectionMatrix[3][2] +
    fProjectionMatrix[3][1]*fProjectionMatrix[1][2]*fProjectionMatrix[2][3] -
    fProjectionMatrix[3][1]*fProjectionMatrix[1][3]*fProjectionMatrix[2][2];

  if (!det)
    return kProjectErr;
    
  fInvProjectionMatrix[1][1] = (fProjectionMatrix[2][2]*fProjectionMatrix[3][3] - 
          fProjectionMatrix[2][3]*fProjectionMatrix[3][2]);

  fInvProjectionMatrix[1][2] = (- fProjectionMatrix[1][2]*fProjectionMatrix[3][3] +
          fProjectionMatrix[1][3]*fProjectionMatrix[3][2]);

  fInvProjectionMatrix[1][3] = (fProjectionMatrix[1][2]*fProjectionMatrix[2][3] -
          fProjectionMatrix[1][3]*fProjectionMatrix[2][2]);

  fInvProjectionMatrix[2][1] = (- fProjectionMatrix[2][1]*fProjectionMatrix[3][3] + 
          fProjectionMatrix[2][3]*fProjectionMatrix[3][1]);

  fInvProjectionMatrix[2][2] = (fProjectionMatrix[1][1]*fProjectionMatrix[3][3] - 
          fProjectionMatrix[1][3]*fProjectionMatrix[3][1]);

  fInvProjectionMatrix[2][3] = (- fProjectionMatrix[1][1]*fProjectionMatrix[2][3] + 
          fProjectionMatrix[1][3]*fProjectionMatrix[2][1]);

  fInvProjectionMatrix[3][1] = (fProjectionMatrix[2][1]*fProjectionMatrix[3][2] - 
          fProjectionMatrix[2][2]*fProjectionMatrix[3][1]);

  fInvProjectionMatrix[3][2] = (- fProjectionMatrix[1][1]*fProjectionMatrix[3][2] + 
          fProjectionMatrix[1][2]*fProjectionMatrix[3][1]);

  fInvProjectionMatrix[3][3] = (fProjectionMatrix[1][1]*fProjectionMatrix[2][2] - 
          fProjectionMatrix[1][2]*fProjectionMatrix[2][1]);

  tvNR_multCstMat(1/det,fInvProjectionMatrix,fInvProjectionMatrix,3,3);

  return kErrNoErr;
}






