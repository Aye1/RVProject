/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Fob.h>
#include <ApiArv/Glove.h>
#include <ApiArv/Sound.h>
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/quaternion.h>
#include <QGLViewer/vec.h>
#include <QGLViewer/frame.h>
#include <QApplication>
#include <QKeyEvent>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace arv;
using namespace qglviewer;
using namespace std;

// global variables :
//-------------------
    Glove *glove;
    Fob *fob;
    Sound *sound;
    int calib_value[18];
    float calib_fob[7] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0 };
    int mon180 = 0;
    bool fullscreen = false;

// this class represents a source of sound, localized in space :
class Source {
    public:
        Source(Vec pos, float radius, const char *audioFileName, Sound *sound)
              : pos(pos),
                radius(radius),
                active(false) {
            sample = sound->add(audioFileName);
            }
    
        bool contains(Vec p) const {
            return (p - pos).norm() < radius;
            }
        
        void play() {
            sample->stop();
            sample->play();
            }
        
        void draw() {
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            if (active)
                glColor3f(0, 0.5, 0.5);
            else
                glColor3f(0, 0.0, 0.5);
            glutSolidCube(4.0);
            glPopMatrix();
            }
        
    public:
        Vec pos;
        float radius;
        bool active;
        Sound::Sample *sample;
    };

class Viewer : public QGLViewer {
    public:
        Viewer(const QGLFormat& format);
    protected :
        virtual void draw();
        virtual void init();
        virtual void animate();
        virtual void updateState(Vec p);
        virtual void keyPressEvent(QKeyEvent *e);
        
    private:
        bool initialized;
        bool initGlove;
        bool initFob;
        Vec pos;
        Vec axis;
        float angle;
        QList<Source> sources;
    };


Viewer::Viewer(const QGLFormat& format) : QGLViewer(format), initialized(false) {
    if (format.stereo())
        setStereoDisplay(true);
    }

void Viewer::keyPressEvent(QKeyEvent *e) {
    bool nobutton = (e->modifiers() == Qt::NoModifier);
    
    if ((e->key() == Qt::Key_I) && nobutton) {
        initGlove = true;
        cerr << "init Glove.." << endl;
        }
    else if ((e->key() == Qt::Key_P) && nobutton) {
        initFob = true;
        cerr << "init Fob.." << endl;
        }
    else if ((e->key() == Qt::Key_M) && nobutton) {
        mon180++;
        if (mon180 == 6) mon180 = 0;
        cerr << "init Fob.." << mon180 << endl;
        }
    else
        QGLViewer::keyPressEvent(e);
    }


void Viewer::draw() {
    //const float nbSteps = 500.0;
    const GLfloat pos2[4] = { -0.5, 0.5, 0.0, 1.0};
    const GLfloat pos1[4] = {0.5, 0.5, 0.0, 1.0};
    const GLfloat spot_dir1[3] = { -1.0, -1.0, 0.0};

    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};

    GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_flash_shiny[] = {50.0};
    GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
    GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
    GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 0.1};

    int rot[18];

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir1);

    glLightfv(GL_LIGHT2, GL_POSITION, pos2);

    fob->getPosAxisRot(pos, angle, axis);
    glove->getRot(rot);

    if (initGlove) {
        for (int i = 0; i < 18;i++) calib_value[i] = rot[i];
        initGlove = false;
        }

    if (initFob) {
        for (int i = 0; i < 3;i++) calib_fob[i] = pos[i];
        for (int i = 0; i < 3;i++) calib_fob[i + 3] = axis[i];
        calib_fob[6] = angle;
        initFob = false;
        }

    glPushMatrix();
    glTranslatef(pos[0] - calib_fob[0], pos[1] - calib_fob[1], pos[2] - calib_fob[2]);
    glRotatef(angle, axis.x, axis.y, axis.z);

    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    float articulation[] = {0.0, 0.7, 0.0};
    float membre[] = {0.0, 0.4, 0.0};
    glColor3fv(articulation);

    glRotatef( -90., 1., 0., 0.);
    glutSolidSphere(2.0, 10, 10);

    //WRIST
    glRotatef(calib_value[16] - rot[16], 1., 0., 0.); //pitch
    glRotatef(calib_value[17] - rot[17], 0., 0., 1.); //yaw

    glPushMatrix();
    glTranslatef(0., 4.5, 0.);

    glScalef(0.8, 0.9, 0.2);
    glColor3fv(membre);
    glutSolidCube(10.0);
    glPopMatrix();

    //THUMB
    glPushMatrix();

    glTranslatef( -3.5, 0., 0.);
    glRotatef((calib_value[0] - rot[0])*1.4, 0., 1., 0.); //pitch

    glTranslatef( -2.5, 6., -1.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef((calib_value[3] - rot[3])*0.5, 0., 0., 1.); //pitch
    glRotatef( -45, 0., 1., 0.); //pitch
    glRotatef((calib_value[1] - rot[1]), 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2., 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(4.0);
    glPopMatrix();

    glTranslatef(0., 3.5, 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[2] - rot[2], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2., 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(3.);
    glPopMatrix();

    glPopMatrix();


    //INDEX
    glPushMatrix();
    glTranslatef( -4., 10., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef((calib_value[8] - rot[8])*0.7, 0., 0., 1.); //pitch
    glRotatef(calib_value[4] - rot[4], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2.5, 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();

    glTranslatef(0., 4., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[5] - rot[5], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2.5, 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();

    glPopMatrix();

    //MIDDLE
    glPushMatrix();
    glTranslatef( -1., 10.5, 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[6] - rot[6], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2.5, 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();

    glTranslatef(0., 4., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[7] - rot[7], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0.0, 2.5, 0.0);
    glScalef(0.2, 1.0, 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();

    glPopMatrix();

    //RING
    glPushMatrix();
    glTranslatef(2., 10., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef((rot[11] - calib_value[11]), 0., 0., 1.); //pitch
    glRotatef(calib_value[9] - rot[9], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2.5, 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();

    glTranslatef(0., 4., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[10] - rot[10], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2.5, 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(5.0);
    glPopMatrix();


    glPopMatrix();

    //PINKIE
    glPushMatrix();
    glTranslatef(4., 9., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef((rot[14] - calib_value[14])*0.7, 0., 0., 1.); //pitch
    glRotatef(calib_value[12] - rot[12], 1., 0., 0.); //pitch

    glPushMatrix();
    glTranslatef(0., 2., 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(4.0);
    glPopMatrix();

    glTranslatef(0., 3., 0.);
    glColor3fv(articulation);
    glutSolidSphere(1., 10, 10);
    glRotatef(calib_value[13] - rot[13], 1., 0., 0.); //pitch
    //    cerr<<calib_value[13]-rot[13]<<endl;
    glPushMatrix();
    glTranslatef(0., 2., 0.);
    glScalef(0.2, 1., 0.2);
    glColor3fv(membre);
    glutSolidCube(4.0);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

    Frame fobMeasured;
    fobMeasured.setPosition(pos[0] - calib_fob[0], pos[1] - calib_fob[1], pos[2] - calib_fob[2]);
    fobMeasured.setOrientation(qglviewer::Quaternion(axis, angle / 57.4));

    Frame fobCorrection;
    fobCorrection.setReferenceFrame(&fobMeasured);
    fobCorrection.setRotation(qglviewer::Quaternion(Vec(0,1,0), 180. / 57.4));

    Frame handCorrection;
    handCorrection.setReferenceFrame(&fobCorrection);
    handCorrection.setRotation(qglviewer::Quaternion(Vec(1,0,0), -90. / 57.4));

    Frame wristPitch;
    wristPitch.setReferenceFrame(&handCorrection);
    wristPitch.setRotation(qglviewer::Quaternion(Vec(1,0,0), (calib_value[16] - rot[16]) / 57.4));

    Frame wristYaw;
    wristYaw.setReferenceFrame(&wristPitch);
    wristYaw.setRotation(qglviewer::Quaternion(Vec(0,0,1), (calib_value[17] - rot[17]) / 57.4));

    Frame indexTranslate;
    indexTranslate.setReferenceFrame(&wristYaw);
    indexTranslate.setTranslation( -4, 10, 0);

    Frame indexPitch;
    indexPitch.setReferenceFrame(&indexTranslate);
    indexPitch.setRotation(qglviewer::Quaternion(Vec(0, 0, 1), ((calib_value[8] - rot[8])*0.7) / 57.4));

    Frame indexYaw;
    indexYaw.setReferenceFrame(&indexPitch);
    indexYaw.setRotation(qglviewer::Quaternion(Vec(1, 0, 0), (calib_value[4] - rot[4]) / 57.4));

    Frame middleIndexTranslate;
    middleIndexTranslate.setReferenceFrame(&indexYaw);
    middleIndexTranslate.setTranslation(0,4,0);

    Frame middleIndexPitch;
    middleIndexPitch.setReferenceFrame(&middleIndexTranslate);
    middleIndexPitch.setRotation(qglviewer::Quaternion(Vec(1,0,0), (calib_value[5] - rot[5]) / 57.4)); //pitch

    Frame tipIndexTranslate;
    tipIndexTranslate.setReferenceFrame(&middleIndexPitch);
    tipIndexTranslate.setTranslation(0,6,0);

    tipIndexTranslate.getPosition(pos[0], pos[1], pos[2]);
    // cerr<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
    glPushMatrix();
    glMultMatrixd(tipIndexTranslate.worldMatrix());
    glColor3f(0.6, 0.0, 0.0);
    glutSolidCube(1.5);
    glPopMatrix();

    updateState(pos);

    // display the sound sources :
    foreach (Source s, sources) s.draw();
    }

void Viewer::updateState(Vec p) {
    foreach (Source s, sources) {
        if (s.contains(p)) {
            cerr << "play sound.." << endl;
            if (!s.active) {
                s.active = true;
                s.play();
                }
            }
        else
            s.active = false;
        }
    }

void Viewer::animate() {
    fob->update();
    glove->update();
    }

void Viewer::init() {
    // to avoid initialization to be performed several times :
    if (initialized) return;
    initialized = true;
    
    if (fullscreen) setFullScreen(true);
    setBackgroundColor(QColor(0,0,0));
    setAnimationPeriod(40);
    startAnimation();
    setAxisIsDrawn(false);

    // init the fob (serial port 0) and the glove (serial port 1) :
    //-------------------------------------------------------------
        fob = new Fob(Fob::HEMI_FORWARD, PORT0);
        fob->init();
    
        glove = new Glove(PORT1);
        glove->init();
    
        glove->start();
        glove->update();
        glove->getRot(calib_value);

    // init the sounds set :
    sound = new Sound();
    if (!sound->init()) {
        cerr << "> problem during Sound::init(), abort." << endl;
        close();
        }

    sources << Source(Vec(-20,  0, 0), 3.0f, "data/jungle.wav",     sound);
    sources << Source(Vec( 20,  0, 0), 3.0f, "data/guitar.wav",     sound);
    sources << Source(Vec(  0, 20, 0), 3.0f, "data/drum.wav",       sound);
    sources << Source(Vec(  0,-20, 0), 3.0f, "data/bass.wav",       sound);
    sources << Source(Vec(  0,  0, 0), 3.0f, "data/percussion.wav", sound);


    //init camera and viewer
    setSceneRadius(100.0f);
    showEntireScene();

    //extrinsic parameters
    camera()->setPosition(Vec(0, 0, 50));
    camera()->lookAt(Vec(0, 0, 0));
    camera()->setUpVector(Vec(0, 1, 0));
    /*
    camera()->setDistanceToScreen(5.0);
    camera()->setPhysicalScreenWidth(3.5);
    camera()->setDistanceToZeroParallaxPlane(20.0);
    */
    camera()->setPhysicalDistanceToScreen(5.0);   // }
    camera()->setPhysicalScreenWidth(3.5);        // } //@@ a voir
    camera()->setFocusDistance(20.0);             // }

    //intrinsic parameters

    // Light1 is a spot light
    glEnable(GL_LIGHT1);
    const GLfloat light_ambient[4]  = {0.5, 0.5, 0.5, 1.0};
    const GLfloat light_specular[4] = {1.0, 1.0, 1.0, 1.0};
    const GLfloat light_diffuse[4]  = {3.0, 3.0, 1.0, 1.0};

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
    const GLfloat light_ambient2[4]  = { 0.5, 0.5, 0.5, 1.0 };
    const GLfloat light_specular2[4] = { 1.0, 0.0, 0.0, 1.0 };
    const GLfloat light_diffuse2[4]  = { 0.0, 3.0, 0.0, 1.0 };

    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);

    initGlove = false;
    initFob = false;
    }

int main(int argc, char** argv) {
    // parse arguments :
    //------------------
        fprintf(stderr, "\nCommands: \n");
        fprintf(stderr, " Press i to put the virtual hand in the basic position.\n");
        fprintf(stderr, " Press p to center the position of glove.\n");
        fprintf(stderr, " Press Esc to quit.\n\n");
    
        fprintf(stderr, "\nOptions: \n");
        fprintf(stderr, " -s : for stereo vision.\n");
        fprintf(stderr, " -f : for fullscreen.\n\n");
    
        bool stereoVision = false;
    
        for (int i=1; i<argc; i++) {
            if (strcmp("-s", argv[i]) == 0)
                stereoVision = true;
            else {
                if (strcmp("-f", argv[i]) == 0)
                    fullscreen = true;
                else
                    fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
                }
            }

    // display viewer :
    //-----------------
        QApplication application(argc, argv);
    
        QGLFormat format;
        format.setStereo(stereoVision);
        Viewer viewer(format);
        viewer.show();
    
        return application.exec();
    }

