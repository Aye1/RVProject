/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Camera.h>
#include <ApiArv/Tracker.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>

using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

// global variables :
//-------------------
    Camera *camera;
    Tracker *tracker;
    int drawMode = 0;
    int seethroughMode = 0;

void drawScene(double projMatrix[16], double modelMatrix[16]) {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projMatrix);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(modelMatrix);

    if (drawMode) {
        GLfloat mat_ambient[] = {0.0, 0.0, 1.0, 1.0};
        GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
        GLfloat mat_flash_shiny[] = {50.0};
        GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
        GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
        GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 0.1};

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMatrixMode(GL_MODELVIEW);

        glTranslatef( 0.0, 0.0, 25.0 );
        glutSolidCube(50.0);
        }
    else {
        glLineWidth(3);
        glBegin(GL_LINES);
            glColor3f(1.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 0.0); glVertex3d(50, 0.0, 0.0);
            glColor3f(0.0, 1.0, 0.0); glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 50, 0.0);
            glColor3f(0.0, 0.0, 1.0); glVertex3d(0.0, 0.0, 0.0); glVertex3d(0.0, 0.0, 50);
        glEnd();
        }
    }

void drawObjectOnMarker(int marker) {
    double projMatrix[16];
    double modelMatrix[16];
    tracker->getGLProjectionMatrix(1.0f, 1000.0f, projMatrix);
    tracker->getGLModelMatrix(marker, modelMatrix);
    drawScene(projMatrix, modelMatrix);
    }

/******************************************************************************************/

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //camera->update();
    Image image = camera->getImage();
    if (!seethroughMode)
        image.draw();

    camera->update();
    tracker->update(image);
    
    for(int i=0; i<tracker->numMarkers(); i++) {
        if (tracker->isMarkerVisible(i))
            drawObjectOnMarker(i);
        }

    glutSwapBuffers();
    }

/******************************************************************************************/

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if ((key == 27) || (key == 'q') || (key == 'Q')) {
        cout << "BYE" << endl;
        exit(0);
        }
    if ((key == 'd') || (key == 'D')) drawMode = 1 - drawMode;
    if ((key == 's') || (key == 'S')) seethroughMode = 1 - seethroughMode;
    }

/******************************************************************************************/

int main(int argc, char* argv[]) {
    cout << "Options:" << endl;
    cout << " -c 0 : to use Sony Motorized Camera (default parameter)." << endl;
    cout << " -c 1 : to use Philips TouCam Camera." << endl;
    cout << " -f : for fullscreen." << endl << endl;
    cout << "Press s to activate seethrough mode or not." << endl;
    cout << "Press Esc or q to quit.\n" << endl;

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
                cerr << argv[i] << " is not a supported option of " << argv[0] << endl;
            }
        }

    camera = Camera::create(static_cast<Camera::Type>(camType));
    if (camera == NULL) ERROR

    const int w = camera->width();
    const int h = camera->height();

    tracker = Tracker::create();
    if (tracker == NULL) ERROR
    tracker->setThreshold(100);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(w, h);
    glutCreateWindow("ARV Live Video (in openGL)");
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);

    if (fullScreen)
        glutFullScreen();

    camera->start();

    glutMainLoop();
    return 0;
    }

