/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Keyboard.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace arv;

Keyboard *keyboard;
float pos_x = 0;
float pos_y = 0;

Keyboard::Code Key_Up;
Keyboard::Code Key_Down;
Keyboard::Code Key_Left;
Keyboard::Code Key_Right;
Keyboard::Code Key_T;
Keyboard::Code Key_V;
Keyboard::Code Key_F;
Keyboard::Code Key_G;
Keyboard::Code Key_I;
Keyboard::Code Key_Q;


void display(void) {
    // keyboard management :
    //----------------------
    keyboard->update();
    if (keyboard->justPressed(Key_Up))       pos_y += 0.5;
    if (keyboard->justPressed(Key_Down))     pos_y -= 0.5;
    if (keyboard->justPressed(Key_Left))     pos_x -= 0.5;
    if (keyboard->justPressed(Key_Right))    pos_x += 0.5;

    if (keyboard->isPressed(Key_T))  pos_y += 0.02;
    if (keyboard->isPressed(Key_V))  pos_y -= 0.02;
    if (keyboard->isPressed(Key_F))  pos_x -= 0.02;
    if (keyboard->isPressed(Key_G))  pos_x += 0.02;

    if (keyboard->justPressed(Key_I)) {
        pos_x = 0.0;
        pos_y = 0.0;
        }

    if (keyboard->justPressed(Key_Q)) {
        keyboard->close();
        exit(1);
        }
    
    // display of the cube :
    //----------------------
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    // setup the view :
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        40.0,       // field of view (in degrees)
        1.0,        // aspect ratio
        1.0,        // z-near
        100.0);     // z-far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0,   // eye position
              0.0, 0.0,  0.0,   // center position
              0.0, 1.0,  0.0);  // up-vector (y direction)

    glMatrixMode(GL_MODELVIEW);
    glTranslatef(pos_x, pos_y, -25.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glutSolidTeapot(1.0);
    glutSolidSphere(1.0, 20, 20);

    glutSwapBuffers();
    }


int main(int argc, char *argv[]) {
    fprintf(stderr, "\nOptions: \n");
    fprintf(stderr, " -f : for fullscreen.\n\n");
    fprintf(stderr, "Move the cube with arrows or keys t,v,f,g.\n");
    fprintf(stderr, "Press i to center the cube.\n");
    fprintf(stderr, "Press q to quit.\n\n");

    bool fullScreen;
    for (int i=1; i<argc; i++) {
        if (strcmp("-f", argv[i]) == 0)
            fullScreen = true;
        else
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Keyboard GL Test");
    if (fullScreen)
        glutFullScreen();
    glutDisplayFunc(display);
    glutIdleFunc(display);

    keyboard = new Keyboard();
    keyboard->init();

    Key_Up      = keyboard->keyCode("Up"); 
    Key_Down    = keyboard->keyCode("Down");
    Key_Left    = keyboard->keyCode("Left");
    Key_Right   = keyboard->keyCode("Right");
    Key_T       = keyboard->keyCode("T");
    Key_V       = keyboard->keyCode("V");
    Key_F       = keyboard->keyCode("F");
    Key_G       = keyboard->keyCode("G");
    Key_I       = keyboard->keyCode("I");
    Key_Q       = keyboard->keyCode("Q");

    glutMainLoop();
    return 0;
    }
