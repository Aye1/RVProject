// File       : UProjective.h
// Creation   : 03/1999
// Creator    : M. PRODHOMME
// Goal       : Declares the TProjective class, a class for calculating the
//                projective transformation between two planar references.
//
// Tab        : 2

#ifndef __Projective__
#define __Projective__


// project includes
#include "UErrors.h"

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                        typedefs, consts & globals                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// TProjective        errors ranges from 2110 to 2119
const ErrorNumber kProjectErr = 2110;



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                          Projective class declaration                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class TProjective
{
  public:
    
    ErrorNumber         Init ();
    // allocates and initializes resources

    void                Dispose ();
    // frees resources

    ErrorNumber         Compute (double* points, int n);
    // <points> is an array of corresponding points.
    //  points in the array are sorted as follow:
    //  x0 y0 xp0 yp0 x1 y1 xp1 yp1 ...
    //  where (xn, yn) are the coordinates of point n in the initial reference,
    //  and (xpn, ypn) are the corrdinates of point n in the destination
    //  reference.
    // <n> is the number of couple of points, i.e. <points> is an
    //  array of 4*<n> elements.
    
    void                Transform (double *initialPoint, double *destPoint);
    // Compute the coordinates of a point <destPoint> in the destination
    //  reference, given its coordinates <initialPoint> in the initial
    //  reference.
    // <initialPoint> and <destPoint> are both arrays of 2 doubles (x and y)

    void                InvTransform (double *destPoint, double *initialPoint);
    // Compute the coordinates of a point <initPoint> in the initial
    //  reference, given its coordinates <destPoint> in the destination
    //  reference.
    // <initialPoint> and <destPoint> are both arrays of 2 doubles (x and y)
  

    // SetFromDoublelist --
    //
    //  Sets the projective transformation parameters from a list of 9 double
    //  resolution floating point numbers.
    //  Memory for the double list must be allocated by the caller.
    //  May return an error in case of singularity in computing the inverse
    //  of the projection matrix.
    
    ErrorNumber         SetFromDoublelist (double* params);


    // GetIntoDoublelist --
    //
    //  Gets the projective transformation parameters and store them in
    //  a list of 9 double resolution floating point numbers.
    //  Memory for the double list must be allocated by the caller. 
    
    void                GetIntoDoublelist (double* params);

  protected:

    double **fProjectionMatrix; 
    double **fInvProjectionMatrix;
    double **fA;
    double **fAt;
    double **fAtA;
    double *fd;
    double *fe;
    int fn;
    
    // LeastEigenValue --
    //
    //  Search in d[1..9] for the index of the eigenvalue having
    //  the lowest absolute value.

    int LeastEigenValue(double *d, int l);
    
    // InvertProjMatrix --
    //
    //  Inverts the projection matrix and stores the result.
    
    ErrorNumber InvertProjMatrix ();
};


#endif



