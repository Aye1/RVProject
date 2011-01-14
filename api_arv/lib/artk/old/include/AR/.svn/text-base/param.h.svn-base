/*******************************************************
 *
 * Author: Takeshi Mita, Shinsaku Hiura, Hirokazu Kato
 *
 *         tmita@inolab.sys.es.osaka-u.ac.jp
 *         shinsaku@sys.es.osaka-u.ac.jp
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 4.1
 * Date: 01/12/07
 *
*******************************************************/
#ifndef AR_PARAM_H
#define AR_PARAM_H
#ifdef __cplusplus
extern "C" {
#endif

#include <AR/config.h>

typedef struct {
    int      xsize, ysize;
    double   mat[3][4];
    double   dist_factor[4];
} ARParam;



int  arParamGet( double global[][3], double screen[][2], int data_num,
                 double mat[3][4] );

int  arParamDecomp( ARParam *source, ARParam *icpara, double trans[3][4] );

int  arParamDecompMat( double source[3][4], double cpara[3][4], double trans[3][4] );


int arParamIdeal2Observ( double dist_factor[4], double ix, double iy,
                         double *ox, double *oy );

int arParamObserv2Ideal( double dist_factor[4], double ox, double oy,
                         double *ix, double *iy );

int arParamChangeSize( ARParam *source, int xsize, int ysize, ARParam *newparam );

int    arParamSave( char *filename, int num, ARParam *param, ...);
int    arParamLoad( char *filename, int num, ARParam *param, ...);
int    arParamDisp( ARParam *param );

#ifdef __cplusplus
}
#endif
#endif
