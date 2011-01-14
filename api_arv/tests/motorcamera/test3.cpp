/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/MotorizedCamera.h>
#include <QGLViewer/qglviewer.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QKeyEvent>
#include <iostream>

using namespace arv;
using namespace std;
using namespace qglviewer;

#define ERROR { cerr << "- an error happened, ending test." << endl; exit(1); }

/******************************************************************************************/

namespace {
    int inputInt(int defVal, int minVal, int maxVal, QString name, int inc) {
        bool choiceDone;
        int val = QInputDialog::getInteger(NULL, "Valeur", name + ":", defVal, minVal, maxVal, inc, &choiceDone);
        return choiceDone ? val : defVal;
        }
    
    float inputFloat(float defVal, float minVal, float maxVal, QString name, int decimals) {
        bool choiceDone;
        float val = (float)(QInputDialog::getDouble(NULL, "Valeur", name + ":", (double)defVal, (double)minVal, (double)maxVal, decimals, &choiceDone));
        return choiceDone ? val : defVal;
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
        MotorizedCamera *cam;
        bool printTime;
        long ps;    // pan speed
        long ts;    // tilt speed
        long zs;    // zoom speed
    };

/******************************************************************************************/

int main(int argc, char** argv) {
    QApplication application(argc, argv);
    Viewer viewer;
    viewer.show();
    return application.exec();
    }

/******************************************************************************************/
// Viewer implementation :

Viewer::Viewer(QWidget* parent) :
        QGLViewer(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::StencilBuffer), parent),
        cam(NULL),
        printTime(false) {
    ps = 5;
    ts = 2;
    zs = 7;
    }

Viewer::~Viewer() {
    if (cam != NULL) {
        cam->stop();
        delete cam;
        }
    }

void Viewer::init() {
    if (cam == NULL) {
        cam = MotorizedCamera::create();
        if (cam == NULL) ERROR
        cam->start();
        //resize(cam->width(), cam->height());
        
        cout << "- PAN_MIN = " << cam->PAN_MIN << endl;
        cout << "- PAN_MAX = " << cam->PAN_MAX << endl;
        cout << "- PAN_SPEED_MIN = " << cam->PAN_SPEED_MIN << endl;
        cout << "- PAN_SPEED_MAX = " << cam->PAN_SPEED_MAX << endl;

        cout << "- TILT_MIN = " << cam->TILT_MIN << endl;
        cout << "- TILT_MAX = " << cam->TILT_MAX << endl;
        cout << "- TILT_SPEED_MIN = " << cam->TILT_SPEED_MIN << endl;
        cout << "- TILT_SPEED_MAX = " << cam->TILT_SPEED_MAX << endl;

        cout << "- ZOOM_MIN = " << cam->ZOOM_MIN << endl;
        cout << "- ZOOM_MAX = " << cam->ZOOM_MAX << endl;
        cout << "- ZOOM_SPEED_MIN = " << cam->ZOOM_SPEED_MIN << endl;
        cout << "- ZOOM_SPEED_MAX = " << cam->ZOOM_SPEED_MAX << endl;
        }
    
    setAnimationPeriod(0);
    startAnimation();
    }

void Viewer::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_I:
            printTime = !printTime;
            break;
            
        case Qt::Key_P:
            ps = (long)inputInt(ps, -10000, 10000, "Pan speed", 1);
            break;
        
        case Qt::Key_T:
            ts = (long)inputInt(ts, -10000, 10000, "Tilt speed", 1);
            break;
        
        case Qt::Key_Z:
            zs = (long)inputInt(zs, -10000, 10000, "Zoom speed", 1);
            break;
        
        case Qt::Key_6: cam->setPanTiltSpeed(+ps, 0); break;
        case Qt::Key_4: cam->setPanTiltSpeed(-ps, 0); break;
        case Qt::Key_8: cam->setPanTiltSpeed(0, +ts); break;
        case Qt::Key_2: cam->setPanTiltSpeed(0, -ts); break;
        
        case Qt::Key_1: cam->setPanTiltSpeed(-ps,-ts); break;
        case Qt::Key_3: cam->setPanTiltSpeed(+ps,-ts); break;
        case Qt::Key_9: cam->setPanTiltSpeed(+ps,+ts); break;
        case Qt::Key_7: cam->setPanTiltSpeed(-ps,+ts); break;
            
        case Qt::Key_5:
            cam->resetPanTilt();
            cam->zoom(0);
            break;
        
        case Qt::Key_Plus:  cam->setZoomSpeed(+zs); break;
        case Qt::Key_Minus: cam->setZoomSpeed(-zs); break;
        
        case Qt::Key_0 :
	    cam->setPanTiltSpeed(0,0);
	    cam->setZoomSpeed(0);
            break;
        
        default:
            QGLViewer::keyPressEvent(e);
        }
    }

void Viewer::draw() {
    Image image = cam->getImage();
    image.draw();
    cam->update();
    
    if (printTime)
        cout << "> update time = " << image.time().tv_sec << " " << image.time().tv_usec << endl;
    }

