#include <OpenGL.h>
#include "Viewer.h"
#include <ApiGraphics/SceneOBJ.h>
#include <ApiGraphics/Renderable.h>
#include <ApiGraphics/Message.h>
#include <QKeyEvent>
#include <math.h>

using namespace apig;

//using namespace std;
using qglviewer::Vec;
using qglviewer::Camera;


const QList<QColor> Viewer::backgrounds = QList<QColor>() << QColor(45,45,45) << QColor(255,255,255) << QColor(0,0,0);

/**************************************************************************************************/

Viewer::Viewer(QString fileName, QWidget *parent)
      : QGLViewer(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer | QGL::Rgba | QGL::AlphaChannel | QGL::StencilBuffer), parent),
        object(NULL),
        wireframe(false),
        bgrIndex(0)
        {
    SceneOBJ* obj = new SceneOBJ(fileName);
    if (!obj->loaded()) {
        Message::error("cannot load specified object");
        exit(1);
        }
    object = obj;
    
    setSnapshotFormat("PNG");           // format par défaut pour les images sauvegardées
    setAnimationPeriod(0);              // pas de limitation pour les FPS
    setStateFileName(QString::null);    // pas de sauvegarde d'état dans un fichier xml
    }

Viewer::~Viewer() {
    if (object != NULL) delete object;
    }

/**************************************************************************************************/

void Viewer::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_O:     // changement entre mode ortho et mode perspective :
            camera()->setType(camera()->type() == Camera::ORTHOGRAPHIC ? Camera::PERSPECTIVE : Camera::ORTHOGRAPHIC);
            updateGL();
            break;
            
        case Qt::Key_W:
            wireframe = !wireframe;
            updateGL();
            break;
            
        case Qt::Key_Backspace :
            setBackgroundColor(backgrounds[bgrIndex = ((bgrIndex + 1) % backgrounds.size())]);
            updateGL();
            break;
            
        default:
            QGLViewer::keyPressEvent(e);
        }
    }

/**************************************************************************************************/

void Viewer::init() {
    OpenGL::init();
    
    object->init();
    setSceneCenter(Vec(object->center()));
    setSceneRadius(object->radius());
    showEntireScene();
    
    setBackgroundColor(backgrounds[bgrIndex]);
    }

/**************************************************************************************************/


void Viewer::draw() {
    glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    glColor3f(0.8f, 0.8f, 0.8f);
    object->render();
    }

