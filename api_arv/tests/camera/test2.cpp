/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Camera.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <sys/time.h>

using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

// global variables :
//-------------------
    Camera *camera;
    int drawtype = 1;
    int printTime = 0;
    int selection = 0;
    int selectionLimits[4];
    
    GLfloat light_diffuse[] = { 1.0, 0.0, 0.0, 1.0 }; /* Red diffuse light. */
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; /* Infinite light location. */
    GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
                          { -1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
                          {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
                      };
    GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
                            {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
                            {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3}
                        };
    GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */


void drawSelection() {
    if (selection != 0) {
        if (selection == 1) {
            glColor3f(0.0, 1.0, 1.0);
            glBegin(GL_LINES);
                int crosssize = 5;
                glVertex2d(selectionLimits[0] + crosssize, selectionLimits[1] + crosssize);
                glVertex2d(selectionLimits[0] - crosssize, selectionLimits[1] - crosssize);
                glVertex2d(selectionLimits[0] - crosssize, selectionLimits[1] + crosssize);
                glVertex2d(selectionLimits[0] + crosssize, selectionLimits[1] - crosssize);
            glEnd();
            }
        else {
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
                glVertex2d(selectionLimits[0], selectionLimits[1]);
                glVertex2d(selectionLimits[0], selectionLimits[3]);
                glVertex2d(selectionLimits[2], selectionLimits[3]);
                glVertex2d(selectionLimits[2], selectionLimits[1]);
            glEnd();
            }
        }
    }

void draw2D() {
    glColor3f(0.0, 0.0, 1.0);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
        glVertex2d(50, 50);
        glVertex2d(50, 430);
        glVertex2d(590, 430);
        glVertex2d(590, 50);
    glEnd();

    int halfLength = 50;
    int borderLength = 10;
    glBegin(GL_LINES);
        glVertex2d(320 - halfLength, 240);
        glVertex2d(320 + halfLength, 240);
    
        glVertex2d(320, 240 - halfLength);
        glVertex2d(320, 240 + halfLength);
    
        glVertex2d(320 - halfLength, 240 - borderLength);
        glVertex2d(320 - halfLength, 240 + borderLength);
    
        glVertex2d(320 + halfLength, 240 - borderLength);
        glVertex2d(320 + halfLength, 240 + borderLength);
    
        glVertex2d(320 - borderLength, 240 - halfLength);
        glVertex2d(320 + borderLength, 240 - halfLength);
    
        glVertex2d(320 - borderLength, 240 + halfLength);
        glVertex2d(320 + borderLength, 240 + halfLength);
    glEnd();
    }


void draw3D() {
    int i;
    static float rot_x = 0;
    glEnable(GL_LIGHTING);

    /* Use depth buffering for hidden surface elimination. */
    glEnable(GL_DEPTH_TEST);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( /* field of view in degree */ 40.0,
            /* aspect ratio */ 1.0,
            /* Z near */ 1.0,  /* Z far */ 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,  /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,  /* center is at (0,0,0) */
              0.0, 1.0, 0.); /* up is in positive Y direction */

    /* Adjust cube position to be asthetic angle. */
    glTranslatef(0.0, 0.0, -4.0);
    rot_x += 3;
    if (rot_x > 360)
        rot_x = 0;
    glRotatef(rot_x, 1.0, 0.0, 0.0);
    glRotatef( -20, 0.0, 0.0, 1.0);

    for (i = 0; i < 6; i++) {
        glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glVertex3fv(&v[faces[i][0]][0]);
            glVertex3fv(&v[faces[i][1]][0]);
            glVertex3fv(&v[faces[i][2]][0]);
            glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
        }

    }

void initScene() {
    /* Setup cube vertex data. */
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

    /* Enable a single OpenGL light. */
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    }

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if ((key == 27) || (key == 'q') || (key == 'Q')) {
        fprintf(stderr, "BYE\n");
        exit(0);
        }

    if ((key == 'd') || (key == 'D')) {
        drawtype = (drawtype + 1) % 3;
        }

    if ((key == 't') || (key == 'T')) {
        printTime = 1 - printTime;
        }
    }

void mouse(int button, int state, int x, int y) {
    //GLUT_LEFT_BUTTON,GLUT_MIDDLE_BUTTON,GLUT_RIGHT_BUTTON,GLUT_UP,GLUT_DOWN);
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
        selection = 1;
        selectionLimits[0] = x;
        selectionLimits[1] = 480 - y;
        }

    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
        selection = 2;
        selectionLimits[2] = x;
        selectionLimits[3] = 480 - y;
        }

    if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) {
        selection = 0;
        }
    }

/*----------------------*/

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Image image = camera->getImage();
    image.draw();
    if (printTime) cout << "image time = " << image.time().tv_sec << " " << image.time().tv_usec << endl;;
    
    camera->update();

    drawSelection();

    if (drawtype == 1) draw2D();
    if (drawtype == 2) draw3D();
    
    glutSwapBuffers();
    }

int main(int argc, char *argv[]) {
    fprintf(stderr, "\nOptions: \n");
    fprintf(stderr, " -c 0 : to use Sony Motorized Camera (default parameter).\n");
    fprintf(stderr, " -c 1 : to use Philips TouCam Camera.\n");
    fprintf(stderr, " -f : for fullscreen.\n\n");

    fprintf(stderr, "You can create a selection by using the mouse:\n");
    fprintf(stderr, "Press the left button to mark the first corner \n");
    fprintf(stderr, "And release it to mark the opposite corner.\n");
    fprintf(stderr, "Press right button to destroy it.\n\n");

    fprintf(stderr, "Press d to change draw type (2d, 3d, no draw).\n");
    fprintf(stderr, "Press t to print image date.\n");
    fprintf(stderr, "Press Esc or q to quit.\n\n");

    int camType = 0;
    int fullScreen = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp("-f", argv[i]) == 0)
            fullScreen = 1;
        else {
            if (strcmp("-c", argv[i]) == 0) {
                i++;
                camType = atoi(argv[i]);
                }
            else
                fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
            }
        }

    camera = Camera::create(static_cast<Camera::Type>(camType));
    if (camera == NULL) ERROR
    const int w = camera->width();
    const int h = camera->height();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(w, h);
    glutCreateWindow("ARV Live Video (in openGL)");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    initScene();

    if (fullScreen) glutFullScreen();

    camera->start();

    glutMainLoop();
    return 0;
    }
