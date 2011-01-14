/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Glove.h>
#include <QGLViewer/qglviewer.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <iostream>
#include <GL/glut.h>

using namespace arv;
using namespace std;
using namespace qglviewer;

#define ERROR { cerr << "- an error happened, ending test." << endl; close(); }

#define GLOVE_ACTIVATED

/******************************************************************************************/

/*
        void draw() {
            glColor3fv(color);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glutSolidTeapot(1.0);
            glPopMatrix();
            }
*/

namespace {
    void drawBox(float x, float y, float z) {
        glPushMatrix();
            glTranslatef(0, y/2, 0);
            glScalef(x, y, z);
            glutSolidCube(1.0);
        glPopMatrix();
        }
    }

/******************************************************************************************/

class Viewer : public QGLViewer {
    public:
        Viewer(QWidget* parent = NULL);
        ~Viewer();

    protected:
        virtual void init();
        virtual void draw();
        virtual void keyPressEvent(QKeyEvent *);

    private:
        Glove *glove;
        Glove::State rest;      // glove state at rest (for calibration)
        Glove::State state;     // current glove state
    };

/******************************************************************************************/

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    Viewer viewer;

    // adjusting of the window in center of screen :
    QDesktopWidget *desktop = QApplication::desktop();
    int ws = desktop->width(),
        hs = desktop->height();
    viewer.move((ws - viewer.width())/2, (hs - viewer.height())/2);

    viewer.show();
    return application.exec();
    }

/******************************************************************************************/
// Viewer implementation :

Viewer::Viewer(QWidget* parent) :
        QGLViewer(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::StencilBuffer), parent),
        glove(NULL) {
    }

Viewer::~Viewer() {
    if (glove != NULL) delete glove;
    }

void Viewer::init() {
    #ifdef GLOVE_ACTIVATED
        if (glove == NULL) {
            glove = Glove::create();
            if (glove == NULL) ERROR
            glove->update(rest);
            }
    #endif
    
    setSceneRadius(12.0f);
    showEntireScene();

    float ambient[4] = { 0, 0, 0, 1 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glEnable(GL_RESCALE_NORMAL);
    }

void Viewer::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_I:
            if (glove != NULL) {
                glove->update(rest);
                updateGL();
                }
            break;

        default:
            QGLViewer::keyPressEvent(e);
        }
    }

void Viewer::draw() {
    if (glove != NULL) glove->update(state);
    // glove state, relative to rest position :
    state.substract(rest);


    // wrist :
    //--------
    glTranslatef(0., -10., 0.);
    glutSolidSphere(2.0, 20, 20);
    glRotatef(-state(16), 1,0,0); // pitch
    glRotatef(-state(17), 0,0,1); // yaw
    drawBox(8, 9, 2);

    // thumb :
    //--------
    glPushMatrix();
        glTranslatef( -3.5, 0., 0.);
        glRotatef(-state(0)*1.4, 0,1,0);

        glTranslatef( -2.5, 6., -1.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(3)*0.5, 0,0,1);
        glRotatef( -45, 0., 1., 0.);
        glRotatef(-state(1), 1,0,0);

        drawBox(0.8, 4, 0.8);

        glTranslatef(0., 3.5, 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(2), 1,0,0);

        drawBox(0.6, 3, 0.6);
    glPopMatrix();


    //INDEX
    glPushMatrix();
        glTranslatef( -4., 10., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(8)*0.7, 0,0,1);
        glRotatef(-state(4), 1,0,0);

        drawBox(1,5,1);

        glTranslatef(0., 4., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(5), 1,0,0);

        drawBox(1,5,1);
    glPopMatrix();

    //MIDDLE
    glPushMatrix();
        glTranslatef( -1., 10.5, 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(6), 1,0,0);

        drawBox(1,5,1);

        glTranslatef(0., 4., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(7), 1,0,0);

        drawBox(1,5,1);
    glPopMatrix();

    //RING
    glPushMatrix();
        glTranslatef(2., 10., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(state(11), 0,0,1);
        glRotatef(-state(9), 1,0,0);

        drawBox(1,5,1);

        glTranslatef(0., 4., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(10), 1,0,0);

        drawBox(1,5,1);
    glPopMatrix();

    //PINKIE
    glPushMatrix();
        glTranslatef(4., 9., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(state(14)*0.7, 0,0,1);
        glRotatef(-state(12), 1,0,0);

        drawBox(0.8, 4, 0.8);

        glTranslatef(0., 3., 0.);
        glutSolidSphere(1., 10, 10);
        glRotatef(-state(13), 1,0,0);
        drawBox(0.8, 4, 0.8);
    glPopMatrix();
    }

