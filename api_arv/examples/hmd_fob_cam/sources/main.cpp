/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Fob.h>
#include <ApiArv/Camera.h>
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/quaternion.h>
#include <QGLViewer/vec.h>
#include <QImage>
#include <QApplication>
#include <QKeyEvent>
#include <iostream> 
#include <fstream>
#include <GL/glut.h>
#include <math.h>

using namespace arv;
using namespace std;
//using namespace qglviewer;
using qglviewer::Frame;
using qglviewer::Vec;
using qglviewer::Quaternion;

/******************************************************************************************/

// variables globales :
//---------------------
    Camera *cam;
    Fob *fob;
    int camType = 0;
    bool disp = false;
    float ratio, u_max, v_max;
    Frame *W2T;     // transfo : world --> tracker
    Frame *T2E;     // transfo : tracker --> eye
    Vec calibPos;

/******************************************************************************************/

class Viewer : public QGLViewer {
    public:
        Viewer(QWidget* parent = NULL, const QGLWidget* shareWidget = 0, Qt::WFlags flags = 0);
        
    protected :
        virtual void draw();
        virtual void init();
        virtual void animate();
        virtual void mousePressEvent(QMouseEvent* e);
        virtual void keyPressEvent(QKeyEvent* e);

    private:
        void loadTexture();
    };

/******************************************************************************************/

Viewer::Viewer(QWidget* parent, const QGLWidget* shareWidget, Qt::WFlags flags)
        : QGLViewer(parent, shareWidget, flags) {
    }

void Viewer::loadTexture() {
    
    if (disp) {
        QString name = "data/qglviewer.logo.png";
        float ratio, u_max, v_max;

        // In case of Cancel
        if (name.isEmpty()) return ;

        QImage img(name);
        if (img.isNull()) {
            cerr << "Unable to load " << name.toLatin1().data() << ", unsupported file format" << endl;
            return ;
            }

        cout << "Loading " << name.toLatin1().data() << ", " << img.width() << "x" << img.height() << " pixels" << endl;

        // 1E-3 needed. Just try with width=128 and see !
        int newWidth  = 1 << (int)(1 + log(img.width() - 1 + 1E-3) / log(2.0));
        int newHeight = 1 << (int)(1 + log(img.height() - 1 + 1E-3) / log(2.0));

        u_max = img.width()  / (float)newWidth;
        v_max = img.height() / (float)newHeight;

        if ((img.width() != newWidth) || (img.height() != newHeight)) {
            cout << "Image size set to " << newWidth << "x" << newHeight << " pixels" << endl;
            img = img.copy(0, 0, newWidth, newHeight);
            }

        ratio = newWidth / float(newHeight);

        QImage glImg = QGLWidget::convertToGLFormat(img);  // flipped 32bit RGBA

        // Bind the img texture...
        glTexImage2D(GL_TEXTURE_2D, 0, 4, glImg.width(), glImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImg.bits());
        }
    else {
        Image image = cam->getImage();
        image.loadTexture();
        /*
        char pipo[1024*512*4];

        for (int i = 0; i < 480; i++)
            for (int j = 0; j < 640; j++) {
                pipo[(1024*i + j)*4 + 0] = image[(640 * i + j) * 3 + 2];
                pipo[(1024*i + j)*4 + 1] = image[(640 * i + j) * 3 + 1];
                pipo[(1024*i + j)*4 + 2] = image[(640 * i + j) * 3 + 0];
                pipo[(1024*i + j)*4 + 3] = 255;
                }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, pipo);

        //glTexImage2D(GL_TEXTURE_2D, 0, GL_BGR, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        */
        }
    

    cam->update();
    }

void Viewer::keyPressEvent(QKeyEvent* e) {
    //key modified calibrated position and update the CS associated
    switch (e->key()) {
        case Qt::Key_Q :
            //exit(1);
            close();
            break;

        case Qt::Key_A :
            disp = !disp;
            break;
            
        default:
            QGLViewer::keyPressEvent(e);
        }
    }


void Viewer::mousePressEvent(QMouseEvent* e) {
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::NoModifier)) {
        std::cout << "press button..OK" << std::endl;
        }
    }

void Viewer::draw() {
    loadTexture();

    glNormal3f(0.0, 0.0, -1.0);

    //const float u = 640.0 / 1024.0;
    //const float v = 480.0 / 512.0;
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(-6.4f, 4.8f, 0);
        glTexCoord2f(1, 0); glVertex3f( 6.4f, 4.8f, 0);
        glTexCoord2f(1, 1); glVertex3f( 6.4f,-4.8f, 0);
        glTexCoord2f(0, 1); glVertex3f(-6.4f,-4.8f, 0);
    glEnd();

    cam->update();
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
    Quaternion quato(quat[0], quat[1], quat[2], quat[3]);
    //rotate in x 90, and in y -90 : this the combine transformation
    Quaternion quatotrackerpos(0.499998, -0.499998, -0.498574, 0.501426);

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
    camera()->setOrientation(Quaternion(quat[0], quat[1], quat[2], quat[3]));
    */
    
    //update and read the fob
    fob->update();
    Vec pos = fob->position();;
    Quaternion quat = fob->orientation();

    W2T->setPosition(pos);
    W2T->setOrientation(quat);

    //get transformed position : Position and Orientation in T2E CS
    pos = T2E->position();
    quat = T2E->orientation();

    // update the viewpoint :
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
    //setAxisIsDrawn(true);

    //init the fob :
    fob = new Fob(Fob::HEMI_BACKWARD, PORT0);
    fob->init();

    //init the real camera
    cam = Camera::create(static_cast<Camera::Type>(camType));
    cam->start();

    //init camera and viewer
    setSceneRadius(50.0f);
    showEntireScene();

    //extrinsic parameters
    camera()->setPosition(Vec(0,0,1));
    camera()->lookAt(Vec(0,0,0));
    camera()->setUpVector(Vec(0,1,0));

    //intrinsic parameters
    camera()->setHorizontalFieldOfView(30.0f / 57.3f);
    /*
        //init gl scene
        GLfloat   mat_ambient[]     = {0.5, 0.5, 0.5, 1.0};
        GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
        GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
        GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
     
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor); 
        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    */


    // Enable GL textures
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    // Nice texture coordinate interpolation
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    //init the calibration parameters
    W2T = new Frame();
    T2E = new Frame();
    T2E->setReferenceFrame(W2T);

    //my best value read from initial calibration file
    ifstream fobfile;
    fobfile.open("data/fobhmd.calib");
    fobfile >> calibPos[0];
    fobfile >> calibPos[1];
    fobfile >> calibPos[2];
    fobfile.close();
    T2E->setTranslation(calibPos[0], calibPos[1], calibPos[2]);
    T2E->setRotation(0.0f, 0.0f, 0.0f, 1.0f);

    cam->update();
    loadTexture();
    }

/******************************************************************************************/

int main(int argc, char** argv) {
    // parse command-line args :
    //--------------------------
    fprintf(stderr, "Options: \n");
    fprintf(stderr, " -c 0 : to use Sony Motorized Camera (default parameter).\n");
    fprintf(stderr, " -c 1 : to use Philips TouCam Camera.\n");
    fprintf(stderr, "Press Esc or q to quit.\n\n");
    for (int i=1; i<argc; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            i++;
            camType = atoi(argv[i]);
            }
        else
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

    // display viewer :
    //-----------------
    QApplication application(argc, argv);
    Viewer viewer(NULL, NULL, Qt::FramelessWindowHint);
    viewer.show();
    return application.exec();
    }

