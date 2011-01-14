/*******************************************************
 *
 * Author: Hirokazu Kato
 *
 *         kato@sys.im.hiroshima-cu.ac.jp
 *
 * Revision: 3.1
 * Date: 01/12/07
 *
*******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <AR/ar.h>

#define WORK_SIZE   1024*32

static ARInt16      l_imageL[640*500];
static ARInt16      l_imageR[640*500];
static int          workL[WORK_SIZE];
static int          workR[WORK_SIZE];
static int          work2L[WORK_SIZE*7];
static int          work2R[WORK_SIZE*7];

static int          wlabel_numL;
static int          wlabel_numR;
static int          wareaL[WORK_SIZE];
static int          wareaR[WORK_SIZE];
static int          wclipL[WORK_SIZE*4];
static int          wclipR[WORK_SIZE*4];
static double       wposL[WORK_SIZE*2];
static double       wposR[WORK_SIZE*2];

static ARInt16 *labeling2( ARUint8 *image, int thresh,
                           int *label_num, int **area, double **pos, int **clip,
                           int **label_ref, int LorR );
static ARInt16 *labeling3( ARUint8 *image, int thresh,
                           int *label_num, int **area, double **pos, int **clip,
                           int **label_ref, int LorR );
static void     put_zero( ARUint8 *p, int size );

void arGetImgFeature( int *num, int **area, int **clip, double **pos )
{
    *num  = wlabel_numL;
    *area = wareaL;
    *clip = wclipL;
    *pos  = wposL;

    return;
}

ARInt16 *arLabeling( ARUint8 *image, int thresh,
                     int *label_num, int **area, double **pos, int **clip,
                     int **label_ref )
{
    if( arDebug ) {
        return( labeling3(image, thresh, label_num,
                          area, pos, clip, label_ref, 1) );
    }
    else {
        return( labeling2(image, thresh, label_num,
                          area, pos, clip, label_ref, 1) );
    }
}

void arsGetImgFeature( int *num, int **area, int **clip, double **pos, int LorR )
{
    if( LorR ) {
        *num  = wlabel_numL;
        *area = wareaL;
        *clip = wclipL;
        *pos  = wposL;
    }
    else {
        *num  = wlabel_numR;
        *area = wareaR;
        *clip = wclipR;
        *pos  = wposR;
    }

    return;
}

ARInt16 *arsLabeling( ARUint8 *image, int thresh,
                      int *label_num, int **area, double **pos, int **clip,
                      int **label_ref, int LorR )
{
    if( arDebug ) {
        return( labeling3(image, thresh, label_num,
                          area, pos, clip, label_ref, LorR) );
    }
    else {
        return( labeling2(image, thresh, label_num,
                          area, pos, clip, label_ref, LorR) );
    }
}

static ARInt16 *labeling2( ARUint8 *image, int thresh,
                           int *label_num, int **area, double **pos, int **clip,
                           int **label_ref, int LorR )
{
    ARUint8   *pnt;                     /*  image pointer       */
    ARInt16   *pnt1, *pnt2;             /*  image pointer       */
    int       *wk;                      /*  pointer for work    */
    int       wk_max;                   /*  work                */
    int       m,n;                      /*  work                */
    int       i,j,k;                    /*  for loop            */
    int       lxsize, lysize;
    int       poff;
    ARInt16   *l_image;
    int       *work, *work2;
    int       *wlabel_num;
    int       *warea;
    int       *wclip;
    double    *wpos;

    if( LorR ) {
        l_image = &l_imageL[0];
        work    = &workL[0];
        work2   = &work2L[0];
        wlabel_num = &wlabel_numL;
        warea   = &wareaL[0];
        wclip   = &wclipL[0];
        wpos    = &wposL[0];
    }
    else {
        l_image = &l_imageR[0];
        work    = &workR[0];
        work2   = &work2R[0];
        wlabel_num = &wlabel_numR;
        warea   = &wareaR[0];
        wclip   = &wclipR[0];
        wpos    = &wposR[0];
    }

    thresh *= 3;
    if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) {
        lxsize = arImXsize / 2;
        lysize = arImYsize / 2;
    }
    else {
        lxsize = arImXsize;
        lysize = arImYsize;
    }

    pnt1 = &l_image[0];
    pnt2 = &l_image[(lysize-1)*lxsize];
    for(i = 0; i < lxsize; i++) {
        *(pnt1++) = *(pnt2++) = 0;
    }

    pnt1 = &l_image[0];
    pnt2 = &l_image[lxsize-1];
    for(i = 0; i < lysize; i++) {
        *pnt1 = *pnt2 = 0;
        pnt1 += lxsize;
        pnt2 += lxsize;
    }

    wk_max = 0;
    pnt2 = &(l_image[lxsize+1]);
    if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) {
        pnt = &(image[(arImXsize*2+2)*AR_PIX_SIZE]);
        poff = AR_PIX_SIZE*2;
    }
    else {
        pnt = &(image[(arImXsize+1)*AR_PIX_SIZE]);
        poff = AR_PIX_SIZE;
    }
    for(j = 1; j < lysize-1; j++, pnt+=poff*2, pnt2+=2) {
        for(i = 1; i < lxsize-1; i++, pnt+=poff, pnt2++) {
#ifdef AR_PIX_FORMAT_ABGR
            if( *(pnt+1) + *(pnt+2) + *(pnt+3) <= thresh ) {
#endif
#ifdef AR_PIX_FORMAT_BGRA
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
#endif
#ifdef AR_PIX_FORMAT_BGR
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
#endif
#ifdef AR_PIX_FORMAT_RGBA
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
#endif
#ifdef AR_PIX_FORMAT_RGB
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
#endif
                pnt1 = &(pnt2[-lxsize]);
                if( *pnt1 > 0 ) {
                    *pnt2 = *pnt1;
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    work2[((*pnt2)-1)*7+6] = j;
                }
                else if( *(pnt1+1) > 0 ) {
                    if( *(pnt1-1) > 0 ) {
                        m = work[*(pnt1+1)-1];
                        n = work[*(pnt1-1)-1];
                        if( m > n ) {
                            *pnt2 = n;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == m ) *wk = n;
                                wk++;
                            }
                        }
                        else if( m < n ) {
                            *pnt2 = m;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == n ) *wk = m;
                                wk++;
                            }
                        }
                        else *pnt2 = m;
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                        work2[((*pnt2)-1)*7+6] = j;
                    }
                    else if( *(pnt2-1) > 0 ) {
                        m = work[*(pnt1+1)-1];
                        n = work[*(pnt2-1)-1];
                        if( m > n ) {
                            *pnt2 = n;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == m ) *wk = n;
                                wk++;
                            }
                        }
                        else if( m < n ) {
                            *pnt2 = m;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == n ) *wk = m;
                                wk++;
                            }
                        }
                        else *pnt2 = m;
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                    }
                    else {
                        *pnt2 = *(pnt1+1);
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                        if( work2[((*pnt2)-1)*7+3] > i ) work2[((*pnt2)-1)*7+3] = i;
                        work2[((*pnt2)-1)*7+6] = j;
                    }
                }
                else if( *(pnt1-1) > 0 ) {
                    *pnt2 = *(pnt1-1);
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    if( work2[((*pnt2)-1)*7+4] < i ) work2[((*pnt2)-1)*7+4] = i;
                    work2[((*pnt2)-1)*7+6] = j;
                }
                else if( *(pnt2-1) > 0) {
                    *pnt2 = *(pnt2-1);
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    if( work2[((*pnt2)-1)*7+4] < i ) work2[((*pnt2)-1)*7+4] = i;
                }
                else {
                    wk_max++;
                    if( wk_max > WORK_SIZE ) {
                        return(0);
                    }
                    work[wk_max-1] = *pnt2 = wk_max;
                    work2[(wk_max-1)*7+0] = 1;
                    work2[(wk_max-1)*7+1] = i;
                    work2[(wk_max-1)*7+2] = j;
                    work2[(wk_max-1)*7+3] = i;
                    work2[(wk_max-1)*7+4] = i;
                    work2[(wk_max-1)*7+5] = j;
                    work2[(wk_max-1)*7+6] = j;
                }
            }
            else {
                *pnt2 = 0;
            }
        }
        if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) pnt += arImXsize*AR_PIX_SIZE;
    }

    j = 1;
    wk = &(work[0]);
    for(i = 1; i <= wk_max; i++, wk++) {
        *wk = (*wk==i)? j++: work[(*wk)-1];
    }
    *label_num = *wlabel_num = j - 1;
    if( *label_num == 0 ) {
        return( l_image );
    }

    put_zero( (ARUint8 *)warea, *label_num *     sizeof(int) );
    put_zero( (ARUint8 *)wpos,  *label_num * 2 * sizeof(double) );
    for(i = 0; i < *label_num; i++) {
        wclip[i*4+0] = lxsize;
        wclip[i*4+1] = 0;
        wclip[i*4+2] = lysize;
        wclip[i*4+3] = 0;
    }
    for(i = 0; i < wk_max; i++) {
        j = work[i] - 1;
        warea[j]    += work2[i*7+0];
        wpos[j*2+0] += work2[i*7+1];
        wpos[j*2+1] += work2[i*7+2];
        if( wclip[j*4+0] > work2[i*7+3] ) wclip[j*4+0] = work2[i*7+3];
        if( wclip[j*4+1] < work2[i*7+4] ) wclip[j*4+1] = work2[i*7+4];
        if( wclip[j*4+2] > work2[i*7+5] ) wclip[j*4+2] = work2[i*7+5];
        if( wclip[j*4+3] < work2[i*7+6] ) wclip[j*4+3] = work2[i*7+6];
    }

    for( i = 0; i < *label_num; i++ ) {
        wpos[i*2+0] /= warea[i];
        wpos[i*2+1] /= warea[i];
    }

    *label_ref = work;
    *area      = warea;
    *pos       = wpos;
    *clip      = wclip;
    return( l_image );
}

static ARInt16 *labeling3( ARUint8 *image, int thresh,
                           int *label_num, int **area, double **pos, int **clip,
                           int **label_ref, int LorR )
{
    ARUint8   *pnt;                     /*  image pointer       */
    ARInt16   *pnt1, *pnt2;             /*  image pointer       */
    int       *wk;                      /*  pointer for work    */
    int       wk_max;                   /*  work                */
    int       m,n;                      /*  work                */
    int       i,j,k;                    /*  for loop            */
    int       lxsize, lysize;
    int       poff;
    ARUint8   *dpnt;
    ARInt16   *l_image;
    int       *work, *work2;
    int       *wlabel_num;
    int       *warea;
    int       *wclip;
    double    *wpos;

    thresh *= 3;
    if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) {
        lxsize = arImXsize / 2;
        lysize = arImYsize / 2;
    }
    else {
        lxsize = arImXsize;
        lysize = arImYsize;
    }

    if( LorR ) {
        l_image = &l_imageL[0];
        work    = &workL[0];
        work2   = &work2L[0];
        wlabel_num = &wlabel_numL;
        warea   = &wareaL[0];
        wclip   = &wclipL[0];
        wpos    = &wposL[0];
        if( arImageL == NULL ) {
            int texXsize = 1;
            int texYsize = 1;
            while( texXsize < arImXsize ) texXsize *= 2;
            if( texXsize > 512 ) texXsize = 512;
            while( texYsize < arImYsize ) texYsize *= 2;
            arMalloc( arImageL, ARUint8, texXsize*texYsize*AR_PIX_SIZE );
            /* arMalloc( arImageL, ARUint8, arImXsize*arImYsize*AR_PIX_SIZE ); */
            put_zero( arImageL, lxsize*lysize*AR_PIX_SIZE );
            arImage = arImageL;
        }
    }
    else {
        l_image = &l_imageR[0];
        work    = &workR[0];
        work2   = &work2R[0];
        wlabel_num = &wlabel_numR;
        warea   = &wareaR[0];
        wclip   = &wclipR[0];
        wpos    = &wposR[0];
        if( arImageR == NULL ) {
            int texXsize = 1;
            int texYsize = 1;
            while( texXsize < arImXsize ) texXsize *= 2;
            if( texXsize > 512 ) texXsize = 512;
            while( texYsize < arImYsize ) texYsize *= 2;
            arMalloc( arImageR, ARUint8, texXsize*texYsize*AR_PIX_SIZE );
            /* arMalloc( arImageR, ARUint8, arImXsize*arImYsize*AR_PIX_SIZE ); */
            put_zero( arImageR, lxsize*lysize*AR_PIX_SIZE );
        }
    }

    pnt1 = &l_image[0];
    pnt2 = &l_image[(lysize-1)*lxsize];
    for(i = 0; i < lxsize; i++) {
        *(pnt1++) = *(pnt2++) = 0;
    }

    pnt1 = &l_image[0];
    pnt2 = &l_image[lxsize-1];
    for(i = 0; i < lysize; i++) {
        *pnt1 = *pnt2 = 0;
        pnt1 += lxsize;
        pnt2 += lxsize;
    }

    wk_max = 0;
    pnt2 = &(l_image[lxsize+1]);
    if( LorR ) dpnt = &(arImageL[(lxsize+1)*AR_PIX_SIZE]);
    else       dpnt = &(arImageR[(lxsize+1)*AR_PIX_SIZE]);
    if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) {
        pnt = &(image[(arImXsize*2+2)*AR_PIX_SIZE]);
        poff = AR_PIX_SIZE*2;
    }
    else {
        pnt = &(image[(arImXsize+1)*AR_PIX_SIZE]);
        poff = AR_PIX_SIZE;
    }
    for(j = 1; j < lysize-1; j++, pnt+=poff*2, pnt2+=2, dpnt+=AR_PIX_SIZE*2) {
        for(i = 1; i < lxsize-1; i++, pnt+=poff, pnt2++, dpnt+=AR_PIX_SIZE) {
#ifdef AR_PIX_FORMAT_ABGR
            if( *(pnt+1) + *(pnt+2) + *(pnt+3) <= thresh ) {
                *(dpnt+1) = *(dpnt+2) = *(dpnt+3) = 255;
#endif
#ifdef AR_PIX_FORMAT_BGRA
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 255;
#endif
#ifdef AR_PIX_FORMAT_BGR
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 255;
#endif
#ifdef AR_PIX_FORMAT_RGBA
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 255;
#endif
#ifdef AR_PIX_FORMAT_RGB
            if( *(pnt+0) + *(pnt+1) + *(pnt+2) <= thresh ) {
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 255;
#endif
                pnt1 = &(pnt2[-lxsize]);
                if( *pnt1 > 0 ) {
                    *pnt2 = *pnt1;
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    work2[((*pnt2)-1)*7+6] = j;
                }
                else if( *(pnt1+1) > 0 ) {
                    if( *(pnt1-1) > 0 ) {
                        m = work[*(pnt1+1)-1];
                        n = work[*(pnt1-1)-1];
                        if( m > n ) {
                            *pnt2 = n;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == m ) *wk = n;
                                wk++;
                            }
                        }
                        else if( m < n ) {
                            *pnt2 = m;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == n ) *wk = m;
                                wk++;
                            }
                        }
                        else *pnt2 = m;
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                        work2[((*pnt2)-1)*7+6] = j;
                    }
                    else if( *(pnt2-1) > 0 ) {
                        m = work[*(pnt1+1)-1];
                        n = work[*(pnt2-1)-1];
                        if( m > n ) {
                            *pnt2 = n;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == m ) *wk = n;
                                wk++;
                            }
                        }
                        else if( m < n ) {
                            *pnt2 = m;
                            wk = &(work[0]);
                            for(k = 0; k < wk_max; k++) {
                                if( *wk == n ) *wk = m;
                                wk++;
                            }
                        }
                        else *pnt2 = m;
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                    }
                    else {
                        *pnt2 = *(pnt1+1);
                        work2[((*pnt2)-1)*7+0] ++;
                        work2[((*pnt2)-1)*7+1] += i;
                        work2[((*pnt2)-1)*7+2] += j;
                        if( work2[((*pnt2)-1)*7+3] > i ) work2[((*pnt2)-1)*7+3] = i;
                        work2[((*pnt2)-1)*7+6] = j;
                    }
                }
                else if( *(pnt1-1) > 0 ) {
                    *pnt2 = *(pnt1-1);
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    if( work2[((*pnt2)-1)*7+4] < i ) work2[((*pnt2)-1)*7+4] = i;
                    work2[((*pnt2)-1)*7+6] = j;
                }
                else if( *(pnt2-1) > 0) {
                    *pnt2 = *(pnt2-1);
                    work2[((*pnt2)-1)*7+0] ++;
                    work2[((*pnt2)-1)*7+1] += i;
                    work2[((*pnt2)-1)*7+2] += j;
                    if( work2[((*pnt2)-1)*7+4] < i ) work2[((*pnt2)-1)*7+4] = i;
                }
                else {
                    wk_max++;
                    if( wk_max > WORK_SIZE ) {
                        return(0);
                    }
                    work[wk_max-1] = *pnt2 = wk_max;
                    work2[(wk_max-1)*7+0] = 1;
                    work2[(wk_max-1)*7+1] = i;
                    work2[(wk_max-1)*7+2] = j;
                    work2[(wk_max-1)*7+3] = i;
                    work2[(wk_max-1)*7+4] = i;
                    work2[(wk_max-1)*7+5] = j;
                    work2[(wk_max-1)*7+6] = j;
                }
            }
            else {
                *pnt2 = 0;
#ifdef AR_PIX_FORMAT_ABGR
                *(dpnt+1) = *(dpnt+2) = *(dpnt+3) = 0;
#endif
#ifdef AR_PIX_FORMAT_BGRA
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 0;
#endif
#ifdef AR_PIX_FORMAT_BGR
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 0;
#endif
#ifdef AR_PIX_FORMAT_RGBA
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 0;
#endif
#ifdef AR_PIX_FORMAT_RGB
                *(dpnt+0) = *(dpnt+1) = *(dpnt+2) = 0;
#endif
            }
        }
        if( arImageProcMode == AR_IMAGE_PROC_IN_HALF ) pnt += arImXsize*AR_PIX_SIZE;
    }

    j = 1;
    wk = &(work[0]);
    for(i = 1; i <= wk_max; i++, wk++) {
        *wk = (*wk==i)? j++: work[(*wk)-1];
    }
    *label_num = *wlabel_num = j - 1;
    if( *label_num == 0 ) {
        return( l_image );
    }

    put_zero( (ARUint8 *)warea, *label_num *     sizeof(int) );
    put_zero( (ARUint8 *)wpos,  *label_num * 2 * sizeof(double) );
    for(i = 0; i < *label_num; i++) {
        wclip[i*4+0] = lxsize;
        wclip[i*4+1] = 0;
        wclip[i*4+2] = lysize;
        wclip[i*4+3] = 0;
    }
    for(i = 0; i < wk_max; i++) {
        j = work[i] - 1;
        warea[j]    += work2[i*7+0];
        wpos[j*2+0] += work2[i*7+1];
        wpos[j*2+1] += work2[i*7+2];
        if( wclip[j*4+0] > work2[i*7+3] ) wclip[j*4+0] = work2[i*7+3];
        if( wclip[j*4+1] < work2[i*7+4] ) wclip[j*4+1] = work2[i*7+4];
        if( wclip[j*4+2] > work2[i*7+5] ) wclip[j*4+2] = work2[i*7+5];
        if( wclip[j*4+3] < work2[i*7+6] ) wclip[j*4+3] = work2[i*7+6];
    }

    for( i = 0; i < *label_num; i++ ) {
        wpos[i*2+0] /= warea[i];
        wpos[i*2+1] /= warea[i];
    }

    *label_ref = work;
    *area      = warea;
    *pos       = wpos;
    *clip      = wclip;
    return( l_image );
}

static void     put_zero( ARUint8 *p, int size )
{
    while( (size--) > 0 ) *(p++) = 0;
}
