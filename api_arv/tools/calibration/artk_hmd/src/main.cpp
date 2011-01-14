/*  ---------------------------------------------------------------------------
*   Copyright (C) 2004 ENSIMAG, INPG.
*   The distribution policy is describe on the Copyright.txt furnish 
*    with this library.
*/ 
/*! ---------------------------------------------------------------------------
*
*   file ARVARTHMDCalib.C
*   author Raphael Grasset
*   version 1.0
*   date 16/04/2004 
*   purpose calibrate transformation between art tracking and eye in HMD.
*/

#include <ApiArv/Camera.h>
#include <ApiArv/Tracker.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>

using namespace arv;

Camera *camera;
Tracker *tracker;

#define  CALIB_POS1_NUM     5
#define  CALIB_POS2_NUM     2
static double calib_pos[CALIB_POS1_NUM][2] = { { 160, 120 },
        { 480, 120 },
        { 320, 240 },
        { 160, 360 },
        { 480, 360 } };
static ARParam hmd_param;


static double calib_pos2d[CALIB_POS1_NUM][CALIB_POS2_NUM][2];
static double calib_pos3d[CALIB_POS1_NUM][CALIB_POS2_NUM][3];

bool target_visible;
double patt_trans[3][4];
int targetmesureid;
int mesureid;
bool calibrate_mode;
bool seethrough_mode;
double gl_calib[16];

//translate ART struct to openGL matrix struct
static void ARTCalibToGL( double cparam[3][4], int width, int height, double gnear, double gfar, double m[16] ) {
    double icpara[3][4];
    double trans[3][4];
    double p[3][3], q[4][4];
    int i, j;

    if ( arParamDecompMat(cparam, icpara, trans) < 0 ) {
        std::cerr << "ERROR->ARTCALIBTOGL:gConvGLcpara: Parameter error!!" << std::endl;
        exit(0);
        }

    for ( i = 0; i < 3; i++ ) {
        for ( j = 0; j < 3; j++ ) {
            p[i][j] = icpara[i][j] / icpara[2][2];
            }
        }
    q[0][0] = (2.0 * p[0][0] / width);
    q[0][1] = (2.0 * p[0][1] / width);
    q[0][2] = ((2.0 * p[0][2] / width) - 1.0);
    q[0][3] = 0.0;

    q[1][0] = 0.0;
    q[1][1] = (2.0 * p[1][1] / height);
    q[1][2] = ((2.0 * p[1][2] / height) - 1.0);
    q[1][3] = 0.0;

    q[2][0] = 0.0;
    q[2][1] = 0.0;
    q[2][2] = (gfar + gnear) / (gfar - gnear);
    q[2][3] = -2.0 * gfar * gnear / (gfar - gnear);

    q[3][0] = 0.0;
    q[3][1] = 0.0;
    q[3][2] = 1.0;
    q[3][3] = 0.0;

    for ( i = 0; i < 4; i++ ) {
        for ( j = 0; j < 3; j++ ) {
            m[i + j*4] = q[i][0] * trans[0][j]
                         + q[i][1] * trans[1][j]
                         + q[i][2] * trans[2][j];
            }
        m[i + 3*4] = q[i][0] * trans[0][3]
                     + q[i][1] * trans[1][3]
                     + q[i][2] * trans[2][3]
                     + q[i][3];
        }
    }

//display line for align marker
void displayTarget(double x, double y) {
    glLineWidth( 3.0 );
    if (mesureid % CALIB_POS2_NUM == 0)
        glColor3f( 1.0, 1.0, 1.0 );
    else
        glColor3f( 1.0, 0.0, 0.0 );

    glBegin(GL_LINES);
    glVertex2f(x, 0.);
    glVertex2f(x, 480.);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(0., y);
    glVertex2f(640., y);
    glEnd();
    }

//compute the calibration
void computeCalibration() {
    //init parameters struct
    hmd_param.xsize = 640;
    hmd_param.ysize = 480;
    hmd_param.dist_factor[0] = 320;
    hmd_param.dist_factor[1] = 240;
    hmd_param.dist_factor[2] = 0.0;
    hmd_param.dist_factor[3] = 1.0;

    //resolve AX=B, with A 3D points, B 2D points and X transformation matrix
    if ( arParamGet( (double (*)[3])calib_pos3d, (double (*)[2])calib_pos2d, CALIB_POS1_NUM*CALIB_POS2_NUM, hmd_param.mat) < 0 ) {
        std::cerr << "ERROR->COMPUTECALIBRATION : can't find calibration matrix !! (bad measured values ?)" << std::endl;
        }
    else {
        std::cout << "camera calibrate.. OK" << std::endl;

        //transform result to GL matrix
        ARTCalibToGL(hmd_param.mat, hmd_param.xsize, hmd_param.ysize, 100., 100000., gl_calib);
        //save matrix
        std::ofstream artfile;
        artfile.open("output/arthmd.calib");
        for (int i = 0;i < 16;i++)
            artfile << gl_calib[i] << std::endl;
        artfile.close();
        std::cout << "parameters saves in output/arthmd.calib" << std::endl;

        //reset all for new calibration
        targetmesureid = 0;
        mesureid = 0;
        calibrate_mode = false;
        }

    }

void mouseEvent(int button, int state, int /*x*/, int /*y*/) {
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
        if (target_visible) {
            std::cout << "new measure.." << std::endl;
            //   fprintf(stderr,"%f %f %f, %f %f\n",patt_trans[0][3],patt_trans[1][3],patt_trans[2][3],patt_trans[3][3],calib_pos[targetmesureid][0],calib_pos[targetmesureid][1]);
            calib_pos3d[targetmesureid][mesureid][0] = patt_trans[0][3];
            calib_pos3d[targetmesureid][mesureid][1] = patt_trans[1][3];
            calib_pos3d[targetmesureid][mesureid][2] = patt_trans[2][3];
            calib_pos2d[targetmesureid][mesureid][0] = calib_pos[targetmesureid][0];
            calib_pos2d[targetmesureid][mesureid][1] = calib_pos[targetmesureid][1];
            mesureid++;
            if ( mesureid == CALIB_POS2_NUM ) {         // calibrate with next value
                targetmesureid++;
                mesureid = 0;
                std::cout << "new position.." << std::endl;
                }
            if ( targetmesureid == CALIB_POS1_NUM ) {   // that's finished : compute calibration
                std::cerr << "calibrate.." << std::endl;
                computeCalibration();
                }
            }
        // otherwise do nothing
        }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
        calibrate_mode = !calibrate_mode;
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN )
        seethrough_mode = !seethrough_mode;
    }

void drawScene(double projection[16], double model[16]) {

    GLfloat mat_ambient[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_flash_shiny[] = {50.0};
    GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
    GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
    GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 0.1};

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    //setup with ART tracking
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(projection);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(model);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    glMatrixMode(GL_MODELVIEW);

    //display base on marker
    glPushMatrix();
    glScalef(1.1, 1.1, 0.02);
    glutSolidCube(80.0);
    glPopMatrix();

    //put cube on marker
    glTranslatef( 0.0, 0.0, 40.0 );
    glutSolidCube(80.0);
    }


void display(void) {

    int sx, sy;
    GLfloat zoom;
    int xVideo, yVideo;

    xVideo = 640;
    yVideo = 480;

    zoom = 1.;
    sx = 0;
    sy = yVideo - 1;

    char*image = camera->getImage();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -0.5, xVideo - 0.5, -0.5, yVideo - 0.5, -1.0, 1.0);
    glViewport(0, 0, xVideo, yVideo);

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );

    glDepthMask(GL_FALSE);
    //     // copy video image with draw pixels
    glPixelZoom( zoom, -zoom);
    glRasterPos3i( sx, sy, 0 );

    if (!seethrough_mode)
        glDrawPixels(xVideo, yVideo, GL_BGR, GL_UNSIGNED_BYTE, image );
    glDepthMask(GL_TRUE);

    camera->update();

    tracker->update(image);
    target_visible = 0;
    if (tracker->isMarkerVisible(0)) {
        target_visible = 1;
        tracker->getPos(0, patt_trans);
        double gl_para[16];
        tracker->getGLModelMatrix(0, gl_para);
        if (calibrate_mode)
            displayTarget(calib_pos[targetmesureid][0], calib_pos[targetmesureid][1]);
        else {
            arFittingMode = AR_FITTING_TO_IDEAL;
            drawScene(gl_calib, gl_para);
            }
        }

    glutSwapBuffers();
    }


void keyboard(unsigned char key, int /*x*/, int /*y*/) {

    if (key == 27) {
        fprintf(stderr, "BYE\n");
        exit(0);
        }

    }


int main(int argc, char* argv[]) {

    //init camera
    camera = new Camera(1); //USB camera by default
    camera->init();
    int x, y;
    camera->getSize(x, y);

    //init art
    tracker = new Tracker("data/calibart.dat"); //calibrate pattern
    tracker->init();
    tracker->setThreshold(100);

    //init calibration parameters
    targetmesureid = 0;
    mesureid = 0;
    calibrate_mode = true;
    seethrough_mode = true;

    hmd_param.xsize = 640;
    hmd_param.ysize = 480;
    hmd_param.dist_factor[0] = 320;
    hmd_param.dist_factor[1] = 240;
    hmd_param.dist_factor[2] = 0.0;
    hmd_param.dist_factor[3] = 1.0;

    hmd_param.mat[0][0] = 745.28133;
    hmd_param.mat[0][1] = -115.01106;
    hmd_param.mat[0][2] = 125.39974;
    hmd_param.mat[0][3] = 57978.82635;

    hmd_param.mat[1][0] = -52.95057;
    hmd_param.mat[1][1] = -742.93424;
    hmd_param.mat[1][2] = 21.78003;
    hmd_param.mat[1][3] = 71782.45592;

    hmd_param.mat[2][0] = 0.02867;
    hmd_param.mat[2][1] = -0.11825;
    hmd_param.mat[2][2] = 0.42484 ;
    hmd_param.mat[2][3] = 100.00000;

    ARTCalibToGL(hmd_param.mat, hmd_param.xsize, hmd_param.ysize, 10., 1000., gl_calib);

    //init viewer
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Live Video (in openGL)");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutMouseFunc(mouseEvent);
    glutKeyboardFunc(keyboard);
    glutFullScreen();

    //start camera :
    camera->start();

    std::cout << "ARVARTHMDCalib" << std::endl;
    std::cout << "to calibrate :" << std::endl;
    std::cout << "1. Put the HMD on your head" << std::endl;
    std::cout << "2. Press MIDDLE mouse button, to check if the camera and the eye have 'similar' view (except scale and some translation error). Press at new right button" << std::endl;
    std::cout << "3. Take the calibration tool (with two white cubes marker)" << std::endl;
    std::cout << "4. Align the center between 2 virtual white cubes with the intersection of two lines. Do it each time at short distance and at long distance (maximum arm reach). Press LEFT mouse button when the registration is OK. If the two lines disappear, the pattern is not visible by the camera (change orientation, or lighting condition)." << std::endl;
    std::cout << "4. Repeat this sequence 5 times (left bottom, right bottom, center, left up, right up" << std::endl;
    std::cout << "5. A cube must appear above the marker : check that registration is good" << std::endl;
    std::cout << "6. If not press RIGHT mouse button for retry" << std::endl;
    std::cout << "5. Press ESCAPE on keyboard when you are satisfy" << std::endl;
    glutMainLoop();
    return 0;
    }
