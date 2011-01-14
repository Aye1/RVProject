/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Camera.h>
#include <ApiArv/Tracker.h>
#include <ApiGraphics/SceneOBJ.h>
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/quaternion.h>
#include <QGLViewer/vec.h>
#include <QFileDialog>
#include <QApplication>
#include <QKeyEvent>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h> // RAND_MAX

using namespace std;
using namespace arv;
using namespace apig;
using qglviewer::Vec;
class Particle;

#define ERROR { cerr << "- an error happened, ending test." << endl; exit(1); }

/******************************************************************************************/

// variables globales :
//---------------------
    Camera *cam;
    Tracker *tracker;
    
    int camType = 0;
    bool seeThrough = false;
    bool hmdMode = false;
    bool hmdView = false;
    double hmdProjMatrix[16];

    int nbPart_;
    Particle* particle_;
    
    
/******************************************************************************************/

class DrawableScene {
    public:
        DrawableScene(char* /*name*/) {}
        void render() {}
    };

/******************************************************************************************/

class Particle {
    public :
        Particle();
    
        void init();
        void draw(int patch);
        void animate();
    
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

class Viewer : public QGLViewer {
    public:
        Viewer(QWidget* parent = NULL, const QGLWidget* shareWidget = 0, Qt::WFlags flags = 0);
    protected :
        void drawScene(int patch);
        void drawVideo(char *image);
        virtual void draw();
        virtual void init();
        virtual void animate();
        virtual void keyPressEvent(QKeyEvent *e);
        SceneOBJ *scene;
    };

/******************************************************************************************/

Viewer::Viewer(QWidget* parent, const QGLWidget* shareWidget, Qt::WFlags flags): QGLViewer(parent, shareWidget, flags) {
    }

void Viewer::keyPressEvent(QKeyEvent *e) {
    const bool nomodif = (e->modifiers() == Qt::NoModifier);
    if ((e->key() == Qt::Key_B) && nomodif)
        seeThrough = !seeThrough;
    else if ((e->key() == Qt::Key_V) && nomodif && hmdMode)
        hmdView = !hmdView;
    else
        QGLViewer::keyPressEvent(e);
    }

void Viewer::drawScene(int patch) {
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    // setup the projection matrix :
    double matrix[16];
    glMatrixMode(GL_PROJECTION);
    if (hmdMode && hmdView)
        glLoadMatrixd(hmdProjMatrix);
    else {
        tracker->getGLProjectionMatrix(1.0f, 1000.0f, matrix);
        glLoadMatrixd(matrix);
        }

    // setup the modelview matrix :
    glMatrixMode(GL_MODELVIEW);
    tracker->getGLModelMatrix(patch, matrix);
    glLoadMatrixd(matrix);
    glPushMatrix();

    if (patch == 2) {
        const GLfloat pos1[4] = {0.5, 0.5, 0.0, 1.0};
        const GLfloat spot_dir1[3] = { -1.0, -1.0, 0.0};
        glLightfv(GL_LIGHT1, GL_POSITION, pos1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir1);

        const GLfloat pos2[4] = { -0.5, 0.5, 0.0, 1.0};
        glLightfv(GL_LIGHT2, GL_POSITION, pos2);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        
        scene->render();

        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHTING);
        }
    else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);

        double halfwidth = tracker->markerSize(patch) / 2.0;
        glColor3f(0.5, 0.0, 0.5);
        glBegin(GL_QUADS);
            glVertex3d(-halfwidth, halfwidth, 0.0);
            glVertex3d( halfwidth, halfwidth, 0.0);
            glVertex3d( halfwidth,-halfwidth, 0.0);
            glVertex3d(-halfwidth,-halfwidth, 0.0);
        glEnd();

        glPointSize(3.0f);
        glScalef(200.0f, 200.0f, 200.0f);
        glBegin(GL_POINTS);
        for (int i=0; i<nbPart_; i++) {
            particle_[i].animate();
            particle_[i].draw(patch);
            }
        glEnd();
        }

    glPopMatrix();

    }

void Viewer::draw() {
    cam->update();
    Image image = cam->getImage();

    if (!seeThrough)
        image.draw();

    tracker->update(image);
    for (int i=0; i<tracker->numMarkers(); i++) {
        if (tracker->isMarkerVisible(i))
            drawScene(i);
        }
    }

void Viewer::animate() {
    }

void Viewer::init() {
    //init the viewer
    setFixedSize(640, 480);
    move(0, 0);
    setBackgroundColor(QColor(0,0,0));
    setAnimationPeriod(40);
    startAnimation();
    setAxisIsDrawn(false);

    // initialization of the camera :
    cam = Camera::create(static_cast<Camera::Type>(camType));
    if (cam == NULL) ERROR
    int w = cam->width();
    int h = cam->height();

    // initialization of the tracker :
    tracker = Tracker::create("object_data", w, h);
    if (tracker == NULL) ERROR
    tracker->setThreshold(100);

    // initialization of the viewer :
    setSceneRadius(50.0f);
    showEntireScene();

    //extrinsic parameters
    camera()->setPosition(Vec(0,0,1));
    camera()->lookAt(Vec(0,0,0));
    camera()->setUpVector(Vec(0,1,0));

    //intrinsic parameters
    camera()->setHorizontalFieldOfView(30.0f / 57.3f);

    // Light1 is a spot light
    glEnable(GL_LIGHT1);
    const GLfloat light_ambient[4] = {0.5, 0.5, 0.5, 1.0};
    const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_diffuse[4] = {3.0, 3.0, 3.0, 1.0};

    glLightf( GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);
    glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 50.0);
    glLightf( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf( GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
    glLightf( GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 1.5);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);

    // Light2 is a classical directionnal light
    glEnable(GL_LIGHT2);
    const GLfloat light_ambient2[4] = {0.5, 0.5, 0.5, 1.0};
    const GLfloat light_specular2[4] = {1.0, 0.0, 0.0, 1.0};
    const GLfloat light_diffuse2[4] = {3.0, 3.0, 3.0, 1.0};

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);

    //@ voir : translater scene?
    scene = new SceneOBJ("models/teapot.obj");
    scene->init();

    nbPart_ = 20000;
    particle_ = new Particle[nbPart_];

    cam->start();
    }

/******************************************************************************************/

int main(int argc, char** argv) {
    fprintf(stderr, "\nOptions: \n");
    fprintf(stderr, " -c 0 : to use Sony Motorized Camera (default parameter).\n");
    fprintf(stderr, " -c 1 : to use Philips TouCam Camera.\n");
    fprintf(stderr, " -hmd hmdCalibFile : to run hmd mode.\n\n");

    fprintf(stderr, "Press b to draw the background image coming from camera.\n");
    fprintf(stderr, "Press v if hmd mode is loaded (-hmd) to switch between hmd and classical mod.\n");
    fprintf(stderr, "Press Esc to quit.\n\n");

    for (int i=1; i<argc; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            i++;
            camType = atoi(argv[i]);
            }
        else {
            if (strcmp("-hmd", argv[i]) == 0) {
                hmdMode = true;
                hmdView = true;
                seeThrough = true;

                //load matrix
                std::ifstream artfileRight;
                i++;
                artfileRight.open(argv[i], std::ifstream::in);
                for (int j = 0; j < 16; j++)
                    artfileRight >> hmdProjMatrix[j];
                artfileRight.close();
                std::cout << argv[i] << " loaded" << std::endl;
                }
            else
                fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
            }
        }


    QApplication application(argc, argv);
    Viewer viewer(NULL, NULL, Qt::FramelessWindowHint);
    viewer.show();
    return application.exec();
    }

