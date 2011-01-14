/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Tracker.h"
#include <iostream>
#include <AR/gsub.h>

using namespace std;

namespace arv {

#define ERROR_RETURN(msg, ret) { std::cerr << "- error in arv::Tracker : " << msg << "." << std::endl; return ret; }

/*********************************************************************************************/

// use this native ARToolkit function for parsing object list file :
static char *get_buff(char *buf, int n, FILE *fp) {
    char *ret;
    for (;;) {
        ret = fgets( buf, n, fp );
        if (ret == NULL)
            return NULL;
        if ( buf[0] != '\n' && buf[0] != '#' )
            return ret;
        }
    }

/*********************************************************************************************/

Tracker* Tracker::create(const char* markerFile, int width, int height, const char* cameraFile, int threshold) {
    ARParam cameraParam;
    Marker* markers = NULL;
    int markerNum = 0;

    // set the camera parameter :
    //---------------------------
        ARParam wparam;
        if (arParamLoad(const_cast<char*>(cameraFile), 1, &wparam) < 0) ERROR_RETURN("camera parameter load error", false);
        arParamChangeSize(&wparam, width, height, &cameraParam);    // match to the camera video or display size
        arInitCparam(&cameraParam);

    // load pattern config file :
    //---------------------------
    if (markerFile == NULL) {   // load "patt.hiro" by default
        markerNum = 1;
        markers = new Marker[1];
        strcpy(markers->name, "patterns/patt.hiro");

        markers->width = 80.;
        markers->center[0] = 0.;
        markers->center[1] = 0.;

        if (arLoadPatt(markers->name) < 0) {
            delete[] markers;
            ERROR_RETURN("problem for loading " << markers->name << " pattern", NULL);
            }
        }
    else {  // parse config file
        FILE* fp = fopen(markerFile, "r");
        if (fp == NULL)
            ERROR_RETURN("problem while loading marker file", NULL);
            
        const int B = 256;
        char *buf = new char[B];
        get_buff(buf, B, fp);
        if (sscanf(buf, "%d", &markerNum) != 1) {
            fclose(fp);
            delete[] buf;
            ERROR_RETURN("in marker file : pattern number error", NULL);
            }
        
        markers = new Marker[markerNum];

        for (int i=0; i<markerNum; i++) {
            // read marker config :
            //---------------------
                #define ERROR_PARSING(msg) {                                                \
                    fclose(fp);                                                             \
                    delete[] markers;                                                       \
                    delete[] buf;                                                           \
                    ERROR_RETURN("in marker file : error while parsing " << msg, NULL);     \
                    }
                
                get_buff(buf, B, fp); if ( sscanf(buf, "%i",  &markers[i].id)        != 1 )  ERROR_PARSING("id")
                get_buff(buf, B, fp); if ( sscanf(buf, "%s",   markers[i].name)      != 1 )  ERROR_PARSING("name")
                get_buff(buf, B, fp); if ( sscanf(buf, "%lf", &markers[i].width)     != 1 )  ERROR_PARSING("width")
                get_buff(buf, B, fp); if ( sscanf(buf, "%lf", &markers[i].center[0]) != 1 )  ERROR_PARSING("center.x")
                get_buff(buf, B, fp); if ( sscanf(buf, "%lf", &markers[i].center[1]) != 1 )  ERROR_PARSING("center.y")

            // now try to load the pattern :
            if (arLoadPatt(markers[i].name) < 0) {
                fclose(fp);
                delete[] markers;
                delete[] buf;
                ERROR_RETURN("while loading '" << markers[i].name << "' pattern", NULL)
                }
            
            markers[i].visible = false;
            }
    
        delete[] buf;
        fclose(fp);
        }

    return new Tracker(markers, markerNum, cameraParam, width, height, threshold);
    }

Tracker::~Tracker() {
    delete[] markers;
    }

/*********************************************************************************************/

void Tracker::setThreshold(int threshold) {
    this->threshold = threshold;
    }

void Tracker::info() {
    cout << "> camera parameters :" << endl;
    arParamDisp( &cameraParam);
    cout << "> patterns parameters :" << endl;
    for (int i=0; i<markerNum; i++)
        cout << "  - pattern " << i << " : " << markers[i].id << " " << markers[i].name
             << " width = " << markers[i].width << " center = " << markers[i].center[0] << " " << markers[i].center[1] << endl;
    cout << "> tracking parameters :" << endl;
    cout << "  - threshold = " << threshold << endl;
    }

/*********************************************************************************************/

bool Tracker::update(const Image& image) {
    ARMarkerInfo *result;   // an array of detected markers
    int resultSize;         // number of detected markers 
    if (arDetectMarker((ARUint8*)(image.data), threshold, &result, &resultSize) < 0 )
        ERROR_RETURN("> problem during detection (e.g. make sure that image size matches given tracker size)", false)
    for (int i=0; i<markerNum; i++) {
        int k = -1;
        for (int j=0; j<resultSize; j++)
            if (i == result[j].id && (k == -1 || result[k].cf < result[j].cf))   // first matching marker or better confidence
                k = j;
        if (k != -1) {
            markers[i].visible = true;
            arGetTransMat(result+k, markers[i].center, markers[i].width, markers[i].pos);
            }
        else
            markers[i].visible = false;
        }
    
    return true;
    }

bool Tracker::isMarkerVisible(int m) {
    return markers[m].visible;
    }

/*********************************************************************************************/

#define COPY_MATRIX(dst, src) for (int i=0; i<3; i++) for (int j=0; j<4; j++) dst[i][j] = src[i][j];


void Tracker::getPos(int m, double patt_trans[3][4]) {
    //TODO: add check is visible precond !!
    COPY_MATRIX(patt_trans, markers[m].pos)
    }

void Tracker::getInversePos(int m, double patt_trans[3][4]) {
    double wmat[3][4];
    //TODO: add check is visible precond !!
    COPY_MATRIX(wmat, markers[m].pos)
    arUtilMatInv(wmat, patt_trans);
    }

void Tracker::getRelativePos(int marker, int markerRef, double patt_trans[3][4]) {
    double wmat1[3][4];
    double iwmat1[3][4];
    double wmat2[3][4];
    //TODO: add check is visible precond !!
    COPY_MATRIX(wmat1, markers[markerRef].pos)
    COPY_MATRIX(wmat2, markers[marker].pos)
    arUtilMatInv(wmat1, iwmat1);
    arUtilMatMul(iwmat1, wmat2, patt_trans);
    }

void Tracker::getGLModelMatrix(int m, double gl_para[16]) {
    double patt_trans[3][4];
    getPos(m, patt_trans);

    // transform to gl representation :
    for (int j=0; j<3; j++) for (int i=0; i<4; i++)
        gl_para[i*4 + j] = patt_trans[j][i];
    gl_para[0*4 + 3] = 0.0;
    gl_para[1*4 + 3] = 0.0;
    gl_para[2*4 + 3] = 0.0;
    gl_para[3*4 + 3] = 1.0;
    }


void Tracker::getGLProjectionMatrix(float gnear, float gfar, double matrix[16]) {
    ARParam glparam = cameraParam;
    for (int i=0; i<4; i++)
        glparam.mat[1][i] = (glparam.ysize - 1) * (glparam.mat[2][i]) - glparam.mat[1][i];
    argConvGLcpara(&glparam, gnear, gfar, matrix);    // defined in <AR/gsub.h>
    }

}

