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
#include <math.h>
#include <stdlib.h> // RAND_MAX
//#include <kbde/kbde.h>

using namespace arv;
using namespace std;


#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }
class Particle;

// global variables :
//-------------------
    Camera *camera;
    Tracker *tracker;
    int seethroughMode = 0;
    int nP;                 // number of particles
    Particle* particles;    // list of particles


/******************************************************************************************/

class Particle {
    public :
        Particle();
    
        void init();
        void draw(int patch);
        void animate();
    
    private :
        
        struct Vec {
            union {
                struct { float x, y, z; };
                float v[3];
                };
            Vec(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
            inline operator const float* () const { return v; }     // cast en const float*
            inline operator       float* ()       { return v; }     // cast en float*
            Vec& operator+=(const Vec& p) { x += p.x; y += p.y; z += p.z; return *this; }
            friend Vec operator*(float a, const Vec& p) { return Vec(a * p.x, a * p.y, a * p.z); }
            };
    
    private :
        Vec pos, speed;
        int age, ageMax;
    };

/******************************************************************************************/

Particle::Particle() {
    init();
    }

void Particle::animate() {
    speed.z -= 0.05f;
    pos += 0.1f * speed;
    if (pos.z < 0.0f) {
        speed.z *= -0.8f;
        pos.z = 0.0f;
        }

    if (++age == ageMax) init();
    }

void Particle::draw(int patch) {
    const float c = 1 - (age / (float)ageMax);
    switch (patch) {
        case 0 :
            glColor3f(c,c,0);
            break;
        case 1 :
            glColor3f(0,c,c);
            break;
        default :
            glColor3f(1-c, 1, 1-c);
        }
    glVertex3fv(pos);
    }

void Particle::init() {
    pos = Vec(0,0,0);
    const float angle = 2.0f * M_PI * rand() / RAND_MAX;
    const float norm = 0.04f * rand() / RAND_MAX;
    const float dz = rand() / static_cast<float>(RAND_MAX);
    speed = Vec(
        norm * cosf(angle),
        norm * sinf(angle),
        dz);
    age = 0;
    ageMax = 50 + static_cast<int>(100.0f * rand() / RAND_MAX);
    }

/******************************************************************************************/

void drawObjectOnMarker(int marker) {
    double projMatrix[16];
    double modelMatrix[16];
    tracker->getGLProjectionMatrix(1.0f, 1000.0f, projMatrix);
    tracker->getGLModelMatrix(marker, modelMatrix);
        
    glEnable(GL_LIGHTING);

    // Use depth buffering for hidden surface elimination :
    glEnable(GL_DEPTH_TEST);

    // Setup the view of the cube :
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(projMatrix);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(modelMatrix);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    double halfwidth = tracker->markerSize(marker) / 2.0;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_QUADS);
        glVertex3d(-halfwidth, halfwidth, 0.0);
        glVertex3d( halfwidth, halfwidth, 0.0);
        glVertex3d( halfwidth,-halfwidth, 0.0);
        glVertex3d(-halfwidth,-halfwidth, 0.0);
    glEnd();
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);

    glScalef(200.0f, 200.0f, 200.0f);
    glBegin(GL_POINTS);
    for (int i=0; i<nP; i++) {
        particles[i].animate();
        particles[i].draw(0);
        }
    glEnd();
    }

void initScene() {
    nP = 20000;
    particles = new Particle[nP];
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
 	    //system("kbde -p ArrowL -r ArrowL -b");
        }

    glutSwapBuffers();
    }

/******************************************************************************************/

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if ((key == 27) || (key == 'q') || (key == 'Q')) {
        cout << "BYE" << endl;
        //system("kbde -s 1000 -p ArrowL -r ArrowL -b");    // stop sending key.
        exit(0);
        }

    if ((key == 's') || (key == 'S')) {
        seethroughMode = 1 - seethroughMode;
        }
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

    initScene();

    camera->start();

    glutMainLoop();
    return 0;
    }

