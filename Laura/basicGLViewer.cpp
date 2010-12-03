#include "basicGLViewer.h"
#include "glViewerManager.h"

#include <iostream>
using namespace std;
#include <cstdlib>
#include <GL/glu.h>


// init of static variables
const float BasicGLViewer::black[]   = {0.0, 0.0, 0.0};
const float BasicGLViewer::red[]     = {1.0, 0.0, 0.0};
const float BasicGLViewer::green[]   = {0.0, 1.0, 0.0};
const float BasicGLViewer::yellow[]  = {1.0, 1.0, 0.0};
const float BasicGLViewer::blue[]    = {0.0, 0.0, 1.0};
const float BasicGLViewer::magenta[] = {1.0, 0.0, 1.0};
const float BasicGLViewer::cyan[]    = {0.0, 1.0, 1.0};
const float BasicGLViewer::white[]   = {1.0, 1.0, 1.0};

BasicGLViewer::BasicGLViewer()
{
}

BasicGLViewer::~BasicGLViewer()
{
	GLViewerManager::deleteViewer(getViewerID());
}

void BasicGLViewer::init(const string & title, int width, int height,
                         int initPositionX, int initPositionY)
{

	// window creation
	viewerID = GLViewerManager::createViewer(*this, 
			title, width, height, initPositionX, initPositionY);
	initDepth();
	initRendering();
	initLighting();
	initTexture();
	initCamera();
	reshapeFunc(width, height);
}

void BasicGLViewer::initGlut(int width, int height,
                             int initPositionX, int initPositionY)
{
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(initPositionX, initPositionY);	
}

void BasicGLViewer::initCamera()
{
	cameraAngle = M_PI*0.5f;
	distToOrigin = 20.0f;
	cameraHeight = 0.0f;
}

void BasicGLViewer::initDepth()
{
	glClearDepth(1.0);        // clear the depth buffer
	glDepthFunc(GL_LESS);     // the type of Depth test to do
	glEnable(GL_DEPTH_TEST);  // enable Depth testing
}

void BasicGLViewer::initRendering()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// backgroud color == black
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLineWidth(1.0f);
	glPointSize(2.0f);
}

void BasicGLViewer::initLighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	GLfloat lightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat lightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat lightPosition[]= { 4.0f*cosf(M_PI/3), 3.0f, 4.0f*sinf(M_PI/3), 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION,lightPosition);
	glEnable(GL_LIGHT0);
}

void BasicGLViewer::initTexture()
{	
}

void BasicGLViewer::setCamera()
{
	float x = distToOrigin*cos(cameraAngle);
	float y = cameraHeight;
	float z = distToOrigin*sin(cameraAngle);
	
	glLoadIdentity();
	gluLookAt(x, y, z, -x, y, -z, 0.0f, 1.0f, 0.0f);
	
	glutPostRedisplay();
}

int BasicGLViewer::getViewerID() const
{
	return viewerID;
}	

void BasicGLViewer::setViewerID(int id) 
{
	viewerID = id;
}

bool BasicGLViewer::isCurrentViewer() const
{
	return getViewerID() == GLViewerManager::getCurrentViewerID();
}

void BasicGLViewer::setAsCurrentViewer()
{
	GLViewerManager::setCurrentViewer(getViewerID());
}

void BasicGLViewer::startMainLoop()
{
	setAsCurrentViewer();
	GLViewerManager::startMainLoop();
}



///////////////////////////////////////////////////////////////////////////////
// DEFAULT CALLBACKS IMPLEMENTATION
void BasicGLViewer::displayFunc()
{
}

void BasicGLViewer::keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
		case 'i':
			GLViewerManager::toogleIdleFunc();
			break;
		case 'q':
			exit(1);
			break;
	}
}

void BasicGLViewer::motionFunc(int x, int y)
{
}

void BasicGLViewer::mouseFunc(int button, int state, int x, int y)
{
}

void BasicGLViewer::passiveMotionFunc(int x, int y)
{
}

void BasicGLViewer::reshapeFunc(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) 
		h = 1;
	
	float ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume (perspective camera)
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	setCamera();
}

void BasicGLViewer::specialFunc(int key, int x, int y)
{
// cerr << key << endl;
	
	// move the camera arond the scene
	switch (key) {		
		case GLUT_KEY_LEFT :
			cameraAngle += 0.1f;
			setCamera();  
			break;
		case GLUT_KEY_RIGHT :
			cameraAngle -=0.1f;
			setCamera();
			break;
		case GLUT_KEY_UP :
			distToOrigin -= 0.2f;
			if (distToOrigin<1.5f)
				distToOrigin = 1.5f;
			setCamera();
			break;
		case GLUT_KEY_DOWN :
			distToOrigin += 0.2f;
			setCamera();
			break;
		case GLUT_KEY_PAGE_UP :
			cameraHeight += 0.1f;
			setCamera();
			break;
		case GLUT_KEY_PAGE_DOWN :
			cameraHeight -= 0.1f;
			setCamera();
			break;
	}
}

void BasicGLViewer::visibilityFunc(int visible)
{
}



