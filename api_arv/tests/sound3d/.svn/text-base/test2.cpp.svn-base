/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Sound3D.h>
#include <QGLViewer/qglviewer.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QTime>
#include <GL/glut.h>
#include <iostream>
#include <time.h>
#include <math.h>

using namespace arv;
using namespace std;
using namespace qglviewer;

#define ERROR { cerr << "- an error happened, ending test." << endl; close(); }

/******************************************************************************************/

class Source {
    public:
        Source(Sound3D* s, const char* fileName, Vec pos, Vec color) : pos(pos), color(color) {
            sound = s->addSource(fileName, pos, Vec(0,0,0));
            if (sound != NULL) {
                sound->setLoopMode(true);
                sound->play();
                }
            else
                cerr << "- error while loading '" << fileName << "'." << endl;
            }
        
        bool valid() const { return sound != NULL; }
        
        void draw() {
            glColor3fv(color);
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glutSolidTeapot(1.0);
            glPopMatrix();
            }
        
        void updateState() {
            sound->setState(pos);
            }
        
    public:
        Sound3D::Source *sound;
        Vec pos;
        Vec color;
    };

/******************************************************************************************/

class Viewer : public QGLViewer {
    public:
        Viewer(QWidget* parent = NULL);
        ~Viewer();
    
    protected:
        virtual void init();
        virtual void draw();
        
    private:
        Sound3D *sound;
        QList<Source> sources;
        Vec oldPos;     // last position of camera
        QTime time;     // time since last update
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
        sound(NULL) {
    }

Viewer::~Viewer() {
    if (sound == NULL) delete sound;
    }
    
void Viewer::init() {
    if (sound == NULL) {
        sound = Sound3D::create();
        if (sound == NULL) ERROR;
        cout << "> sound device initialized." << endl;
        
        sources << Source(sound, "samples/bass.wav",   Vec( 10,0,0), Vec(1,0,0));
        sources << Source(sound, "samples/guitar.wav", Vec(-10,0,0), Vec(0,0,1));
        }
    
    setSceneRadius(10.0f);
    showEntireScene();
    
    oldPos = camera()->position();
    time.start();
    setAnimationPeriod(40);
    }

void Viewer::draw() {
    Vec v = (camera()->position() - oldPos) / (time.elapsed() / 1000.0f);
    oldPos = camera()->position();
    time.restart();
    
    v *= 1e-3;  // we scale speed to adjust doppler effect
    
    sound->setListener(
        camera()->position(),       // listener's position
        v,                          // listener's velocity
        camera()->viewDirection(),  // listener's front vector
        camera()->upVector()        // listener's up vector
        );
        
    foreach(Source src, sources)
        if (src.valid()) src.draw();
    }

