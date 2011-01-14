/*  ---------------------------------------------------------------------------
*   Copyright (C) 2004 ENSIMAG, INPG.
*   The distribution policy is describe on the Copyright.txt furnish 
*    with this library.
*/ 
/*! ---------------------------------------------------------------------------
*
*   file ARVFobHMCalib.C
*   author Raphael Grasset
*   version 1.0
*   date 16/04/2004 
*   purpose calibrate transformation between fob and eye in HMD.
*/

#include <ApiArv/Fob.h> 
//#include <ApiArv/Util.h>
#include <iostream>
#include <fstream>

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/quaternion.h>
#include <QGLViewer/vec.h>

#include <QApplication>
#include <GL/glut.h>
#include <math.h>

using namespace arv;
using namespace std;
using namespace qglviewer;

Fob *fob;

bool calibrate_mode;
Frame *W2T;     // world to tracker transform
Frame *T2E;     // tracker to eye transform
Vec calibPos;

//Viewer interface
class Viewer : public QGLViewer {
    public:
        Viewer(QWidget* parent = NULL, const QGLWidget* shareWidget = 0, Qt::WFlags flags = 0);
    protected :
        virtual void draw();
        virtual void init();
        virtual void animate();
        virtual QString helpString() const;
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);
    };

//Viewer code

Viewer::Viewer(QWidget* parent, const QGLWidget* shareWidget, Qt::WFlags flags): QGLViewer(parent, shareWidget, flags) {
    }

void Viewer::keyPressEvent(QKeyEvent* e) {
    //key modified calibrated position and update the CS associated
    switch (e->key()) {
        case Qt::Key_A :
            calibPos[0] += 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        case Qt::Key_Q :
            calibPos[0] -= 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        case Qt::Key_Z :
            calibPos[1] += 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        case Qt::Key_S :
            calibPos[1] -= 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        case Qt::Key_E :
            calibPos[2] += 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        case Qt::Key_D :
            calibPos[2] -= 0.1;
            T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
            break;
        }
    }


void Viewer::mousePressEvent(QMouseEvent* e) {
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::NoModifier)) {
        //left button validate calibration, save and exit
        std::cout << "calibration..OK" << std::endl;
        std::cout << calibPos[0] << " " << calibPos[1] << " " << calibPos[2] << std::endl;
        std::ofstream fobfile;
        fobfile.open("fobhmd.calib");
        fobfile << calibPos[0] << std::endl;
        fobfile << calibPos[1] << std::endl;
        fobfile << calibPos[2] << std::endl;
        std::cout << "parameters save in fobhmd.calib" << std::endl;
        exit(1);
        }
    }

void Viewer::draw() {
    //draw the emittor (in good scale)
    glPushMatrix();
    glTranslatef(0., 4., 0.);
    glutSolidCube(8.0);
    glPopMatrix();

    //and a small cube in front (for oriented the user)
    glPushMatrix();
    glTranslatef(0., 0., 10.);
    glutSolidCube(1.0);
    glPopMatrix();

    }

void Viewer::animate() {
    /*
    float pos[3];
    float quat[4];

    float newquat[4];

    //update and read the fob
    fob->update();
    fob->getPosRot(pos, quat);

    //rotate the fob to match position on HMD : only in orientation
    qglviewer::Quaternion quato(quat[0], quat[1], quat[2], quat[3]);
    //rotate in x 90, and in y -90 : this the combine transformation
    qglviewer::Quaternion quatotrackerpos(0.499998, -0.499998, -0.498574, 0.501426);

    //update the rotation
    // quato*=quatotrackerpos;

    newquat[0] = quato[0];
    newquat[1] = quato[1];
    newquat[2] = quato[2];
    newquat[3] = quato[3];

    W2T->setPosition(pos[0], pos[1], pos[2]);
    W2T->setOrientation(qglviewer::Quaternion(newquat[0], newquat[1], newquat[2], newquat[3]));

    //get transformed position : Position and Orientation in T2E CS
    T2E->getPosition(pos[0], pos[1], pos[2]);
    T2E->getOrientation(quat[0], quat[1], quat[2], quat[3]);

    //update the viewpoint
    camera()->setPosition(pos[0], pos[1], pos[2]);
    camera()->setOrientation(qglviewer::Quaternion(quat[0], quat[1], quat[2], quat[3]));
    */
    Vec pos;
    Quaternion quat;

    //update and read the fob
    fob->update();
    fob->getPosRot(pos, &(quat[0]));

    W2T->setPosition(pos);
    W2T->setOrientation(quat);

    //get transformed position : Position and Orientation in T2E CS
    pos  = T2E->position();
    quat = T2E->orientation();

    //update the viewpoint
    camera()->setPosition(pos);
    camera()->setOrientation(quat);
    }


void Viewer::init() {
    //init the viewer
    setFixedSize(800, 600);
    move(0, 0);
    setBackgroundColor(QColor(0,0,0));
    setAnimationPeriod(5);
    startAnimation();
    setAxisIsDrawn(true);

    //init the fob
    //ARV_UPPER
    fob = new Fob(Fob::HEMI_FORWARD, 1);
    fob->init();
    fob->start();

    //init camera and viewer
    setSceneRadius(50.);
    showEntireScene();

    //extrinsic parameters
    camera()->setPosition(Vec(0,0,1));
    camera()->lookAt(Vec(0,0,0));
    camera()->setUpVector(Vec(0,1,0));

    //intrinsic parameters
    camera()->setHorizontalFieldOfView(30 / 57.3);

    //init gl scene
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
    GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
    GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 0.1};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    //init the calibration parameters
    calibrate_mode = true;

    W2T = new Frame();
    T2E = new Frame();
    T2E->setReferenceFrame(W2T);

    //my best value read from initial calibration file
    ifstream fobfile;
    fobfile.open("fobhmd.calib");
    fobfile >> calibPos[0];
    fobfile >> calibPos[1];
    fobfile >> calibPos[2];
    fobfile.close();
    T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
    T2E->setRotation(0., 0., 0., 1.);

    //display help
    help();
    }

QString Viewer::helpString() const {
    QString text("<h2>ARVFobHMDCalib</h2>");
    text += "You need to align a 3D virtual model of the emittor FOB with the real representation (8 cm cube). For this :<BR>";
    text += "0.Switch you current video config to 640x480 (CTRL+ALT++ on LINUX)<BR>";
    text += "1.Put the HMD on your head and look for see the 3D virtual model of the emittor<BR>";
    text += "1.Now, align orientation of the virtual and real emittor by rotating fob receptor on HMD (you have degre of freedom with the 'pate a gomme') : the two models will be have the same orientation.<BR>";
    text += "2.Align after the position with these keys:<BR>";
    text += "press <B>A/Q</B> for up/down X fob position<BR> ";
    text += "press <B>Z/S</B> for up/down Y fob position<BR> ";
    text += "press <B>E/D</B> for up/down Z fob position<BR> ";
    text += "press left button when registration will be OK<BR>";
    text += "3.You can fix more rigidly the fob.";
    return text;
    }

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    //Viewer viewer(NULL, "ARV Viewer", 0, Qt::WStyle_Customize | Qt::WStyle_NoBorderEx);
    Viewer viewer(NULL, NULL, Qt::FramelessWindowHint);
    viewer.show();
    return application.exec();
    }

