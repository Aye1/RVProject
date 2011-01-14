/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Yoke.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;
using namespace arv;

Yoke *yoke;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( /* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0,  /* Z far */ 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 50.0,   /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,    /* center is at (0,0,0) */
              0.0, 1.0, 0.);    /* up is in positive Y direction */

    yoke->update();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(0.5, 0.5, 0.5);
    if (yoke->buttonPressed(0))  glColor3f(1, 0, 0);
    if (yoke->buttonPressed(1))  glColor3f(0, 1, 0);
    if (yoke->buttonPressed(2))  glColor3f(0, 0, 1);
    if (yoke->buttonPressed(3))  glColor3f(1, 1, 0);
    if (yoke->buttonPressed(8))  glColor3f(0, 1, 1);
    if (yoke->buttonPressed(9))  glColor3f(1, 0, 1);
    if (yoke->buttonPressed(10)) glColor3f(1, 1, 1);
    if (yoke->buttonPressed(11)) glColor3f(0, 0, 0);

    glMatrixMode(GL_MODELVIEW);
    
    // get yoke axes state :
    const float
        dx = yoke->axisValue(0) / 1000.0f,
        dy = yoke->axisValue(1) / 1000.0f,
        sx = yoke->axisValue(2) / 1000.0f,
        sy = yoke->axisValue(3) / 1000.0f,
        sz = yoke->axisValue(4) / 1000.0f,
        rz = yoke->axisValue(5),
        rx = yoke->axisValue(6);

    glTranslatef(dx, dy, -25);
    glRotatef(rz, 0, 0, 1);
    glRotatef(rx, 1, 0, 0);
    glScalef(sx+1, sy+1, sz+1);

    glutSolidCube(4.0);

    glutSwapBuffers();
    }


int main(int argc, char* argv[]) {
    yoke = Yoke::create();
    if (yoke == NULL) return 1;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Yoke GL Test");
    //   glutFullScreen();
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutMainLoop();
    return 0;
    }

