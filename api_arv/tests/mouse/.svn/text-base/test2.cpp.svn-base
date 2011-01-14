/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Mouse.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

Mouse *mouse;

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        /* field of view in degree */ 40.0,
        /* aspect ratio */ 1.0,
        /* Z near */ 1.0,  /* Z far */ 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0,   /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,       /* center is at (0,0,0) */
              0.0, 1.0, 0.);      /* up is in positive Y direction */

    mouse->update();
    const float x = mouse->xPos();
    const float y = mouse->yPos();
    bool button0 = mouse->isPressed(Mouse::LEFT);
    bool button1 = mouse->isPressed(Mouse::MIDDLE);
    bool button2 = mouse->isPressed(Mouse::RIGHT);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glColor3f(0.5, 0.5, 0.5);
    if (button0)            glColor3f(1,0,0);
    if (button1)            glColor3f(0,1,0);
    if (button2)            glColor3f(0,0,1);
    if (button0 && button1) glColor3f(1,1,0);
    if (button1 && button2) glColor3f(0,1,1);
    if (button0 && button2) glColor3f(1,0,1);
    if (button0 && button1 && button2) glColor3f(1,1,1);

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(x / 80.0f - 11., -y / 80.0f + 11., -25.0);
    glutSolidSphere(1.0, 20, 20);

    glutSwapBuffers();
    }

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if ((key == 27) || (key == 'q') || (key == 'Q')) {
        fprintf(stderr, "BYE\n");
        exit(0);
        }
    }

int main(int argc, char* argv[]) {
    fprintf(stderr, "Options: \n");
    fprintf(stderr, " -f : for fullscreen.\n");
    fprintf(stderr, "Press Esc or q to quit.\n");

    bool fullScreen = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp("-f", argv[i]) == 0)
            fullScreen = true;
        else
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Mouse GL Test");
    if (fullScreen)
        glutFullScreen();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    mouse = new Mouse();
    if (!mouse->init()) ERROR;

    glutMainLoop();
    return 0;
    }
