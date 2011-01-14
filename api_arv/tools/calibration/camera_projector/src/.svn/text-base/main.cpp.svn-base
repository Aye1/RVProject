/*  ---------------------------------------------------------------------------
*   Copyright (C) 2004 ENSIMAG, INPG.
*   The distribution policy is describe on the Copyright.txt furnish 
*    with this library.
*/ 
/*! ---------------------------------------------------------------------------
*
*   file ARVARToolkitTest.C
*   author Raphael Grasset
*   version 1.0
*   date 16/04/2004 
*   purpose test artoolkit and video together.
*/

#include <ApiArv/Camera.h>
#include <UProjective.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>

using namespace arv;

Camera *camera;

int calib_point;
float displaypoints[2][9];
int points[2][9];

float correctedPoints[2][9];

char *image;
float squareWidth;
float squareDistX;
float squareDistY;
int win;
int xVideo, yVideo;
void display(void);

int step = 1;

/**
 * Draws a square with a given width at the point (centrex, centrey)
 */
void drawCross(float x, float y, float width, int col) {
    glLineWidth(3.0);

    if (col == 1)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINES);
    glVertex2f(x, y - width);
    glVertex2f(x, y + width);
    glEnd();
    glBegin(GL_LINES);
    glVertex2f(x - width, y);
    glVertex2f(x + width, y);
    glEnd();
    }


void matrixPointProduct(double matrix[9], double xIn, double yIn, double* xOut, double* yOut) {
    double denum = matrix[6] * xIn + matrix[7] * yIn + matrix[8];

    *xOut = (matrix[0] * xIn + matrix[1] * yIn + matrix[2]) / denum;
    *yOut = (matrix[3] * xIn + matrix[4] * yIn + matrix[5]) / denum;
    }

void computeCalibration() {
    std::cerr << "compute calibration.." << std::endl;
    TProjective homo;
    double cpoints[4 * 9];
    double matrix[9];
    for (int i = 0; i < 9; i++) {
        cpoints[i * 4] = points[0][i];
        cpoints[i * 4 + 1] = points[1][i];
        cpoints[i * 4 + 2] = displaypoints[0][i];
        cpoints[i * 4 + 3] = displaypoints[1][i];
        }
    homo.Init();
    homo.Compute(cpoints, 9);
    homo.GetIntoDoublelist(matrix);
    std::cerr << "calibration..OK" << std::endl;
    std::
    cerr << matrix[0] << " " << matrix[1] << " " << matrix[2] << std::
    endl << matrix[3] << " " << matrix[4] << " " << matrix[5] << std::
    endl << matrix[6] << " " << matrix[7] << " " << matrix[8] << std::
    endl;
    std::ofstream camprojfile;
    camprojfile.open("camproj.calib");
    for (int i = 0; i < 9; i++)
        camprojfile << matrix << std::endl;
    camprojfile.close();
    std::cout << "parameters save in camproj.calib" << std::endl;


    std::cout << "screen points -> cliced points * mat" << std::endl;
    double xOut, yOut;
    for (int i = 0; i < 9; i++) {

        matrixPointProduct(matrix, (double)points[0][i], (double)points[1][i], &xOut, &yOut);
        correctedPoints[0][i] = (float)xOut;
        correctedPoints[1][i] = (float)yOut;
        std::cout << displaypoints[0][i] << " " << displaypoints[1][i] << " -> " << xOut << " " << yOut << std::endl;
        }
    }

void displayCam(void);
void displayGrid(void);
void mouseEvent(int button, int state, int x, int y);

void mouseEvent(int button, int state, int x, int y) {
    if ((button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)) {
        if (step == 4) {
            camera->close();
            exit(1);
            }

        if (step == 3) {
            points[0][calib_point] = x;
            points[1][calib_point] = 480 - y;
            calib_point++;
            if (calib_point >= 9) {
                computeCalibration();
                glutDestroyWindow(win);
                win = glutCreateWindow("ARV Live Video (in openGL)");
                glutFullScreen();
                glutDisplayFunc(displayGrid);
                glutIdleFunc(displayGrid);
                glutMouseFunc(mouseEvent);
                step++;
                }
            }


        if (step == 2) {
            camera->update();
            image = camera->getImage();
            glutDestroyWindow(win);
            glutInitWindowPosition(0, 0);
            glutInitWindowSize(640, 480);
            win = glutCreateWindow("ARV Live Video (in openGL)");
            glutDisplayFunc(displayCam);
            glutIdleFunc(displayCam);
            glutMouseFunc(mouseEvent);
            step++;
            }


        if (step == 1) {
            glutDestroyWindow(win);
            win = glutCreateWindow("ARV Live Video (in openGL)");
            glutFullScreen();
            glutDisplayFunc(displayGrid);
            glutIdleFunc(displayGrid);
            glutMouseFunc(mouseEvent);
            step++;
            }


        }
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        camera->close();
        exit(1);
        }


    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (step == 4) {
            step = 2;
            calib_point = 0;
            }
        }

    }



/*----------------------*/


void displayGrid(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -2, 2, -1.5, 1.5, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the drawing color
    glColor3f(1.0, 1.0, 1.0);

    // Draw the 9 squares
    for (int i = 0; i < 9; i++) {
        drawCross(displaypoints[0][i], displaypoints[1][i], squareWidth, 1);
        }


    if (step == 4) {
        for (int i = 0; i < 9; i++) {
            drawCross(correctedPoints[0][i], correctedPoints[1][i], squareWidth, 0);
            }
        }

    glutPostRedisplay();
    glutSwapBuffers();
    }

void displayCam(void) {

    int sx, sy;

    GLfloat zoom;
    if (step == 1) {
        camera->update();
        image = camera->getImage();
        }
    zoom = 1.;
    sx = 0;
    sy = yVideo - 1;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -0.5, xVideo - 0.5, -0.5, yVideo - 0.5, -1.0, 1.0);
    glViewport(0, 0, xVideo, yVideo);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glPixelZoom(zoom, -zoom);
    glRasterPos3i(sx, sy, 0);
    glDrawPixels(xVideo, yVideo, GL_BGR, GL_UNSIGNED_BYTE, image);
    if (step == 3) {
        glDepthMask(GL_TRUE);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.0, 1.0, 1.0);
        for (int i = 0; i < calib_point; i++) {
            drawCross((float)points[0][i], (float)points[1][i], 20, 0);
            }
        }
    glutSwapBuffers();
    }

int main(int argc, char *argv[]) {

    fprintf(stderr, "\nUse: \n");
    fprintf(stderr, " Step 1: place the screen as you want in the return video view and press the left button of the mouse to  continue.\n");
    fprintf(stderr, " Step 2: the grid of calibration appears, press the left button of the mouse to make a capture.\n");
    fprintf(stderr, " Step 3: clic crosses on the view in the following order:\n");
    fprintf(stderr, " 1 2 3\n");
    fprintf(stderr, " 4 5 6\n");
    fprintf(stderr, " 7 8 9\n");
    fprintf(stderr, " Step 4: you see the result, press left button to quit, right button to return to step 2.\n");
    fprintf(stderr, " The matrix of the homography is saved in camproj.calib.\n");

    fprintf(stderr, "\nOptions: \n");
    fprintf(stderr, " -c 0 : to use Sony Motorized Camera (default parameter).\n");
    fprintf(stderr, " -c 1 : to use Philips TouCam Camera.\n");

    int camType = 0;

    for (int i = 1; i < argc; i++) {

        if (strcmp("-c", argv[i]) == 0) {
            i++;
            camType = atoi(argv[i]);
            }
        else {
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
            }
        }

    camera = new Camera(camType);
    camera->init();
    camera->getSize(xVideo, yVideo);
    calib_point = 0;
    squareWidth = 0.07;
    squareDistX = 1.8;
    squareDistY = 1.3;
    displaypoints[0][0] = squareDistX;
    displaypoints[1][0] = squareDistY;
    displaypoints[0][1] = 0;
    displaypoints[1][1] = squareDistY;
    displaypoints[0][2] = -squareDistX;
    displaypoints[1][2] = squareDistY;
    displaypoints[0][3] = squareDistX;
    displaypoints[1][3] = 0;
    displaypoints[0][4] = 0;
    displaypoints[1][4] = 0;
    displaypoints[0][5] = -squareDistX;
    displaypoints[1][5] = 0;
    displaypoints[0][6] = squareDistX;
    displaypoints[1][6] = -squareDistY;
    displaypoints[0][7] = 0;
    displaypoints[1][7] = -squareDistY;
    displaypoints[0][8] = -squareDistX;
    displaypoints[1][8] = -squareDistY;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    win = glutCreateWindow("ARV Live Video (in openGL)");
    glutDisplayFunc(displayCam);
    glutIdleFunc(displayCam);
    glutMouseFunc(mouseEvent);
    camera->start();
    glutMainLoop();
    return 0;
    }


