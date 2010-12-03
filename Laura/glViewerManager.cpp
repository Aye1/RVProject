#include "glViewerManager.h"
#include "basicGLViewer.h"

#include <iostream>
using namespace std;

// init of static members
map<int, BasicGLViewer *> GLViewerManager::viewers;
bool GLViewerManager::idleFuncEnabled = false;

int GLViewerManager::createViewer(BasicGLViewer & viewer, const string & title,
	                        int width, int height,
	                        int initPositionX, int initPositionY)
{
	if (viewers.size() == 0) {
		// first viewer, create glut context
		const char * dummy_argv[] = { "run" } ;
		int dummy_argc = 1;

		// Initialize GLUT
		glutInit(&dummy_argc, (char **)dummy_argv);
	}
	
	// gl stuff
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(initPositionX, initPositionY);	

	int windowID = glutCreateWindow(title.c_str());
	viewers[windowID] = &viewer;
	viewer.setViewerID(windowID);
	
	// Set the glut callbacks (must be done for each viewer,
	// even if the -static- callbacks are the same)
	glutDisplayFunc(callbackDisplayFunc);
	glutKeyboardFunc(callbackKeyboardFunc);
	glutSpecialFunc(callbackSpecialFunc);
	glutMouseFunc(callbackMouseFunc);
	glutMotionFunc(callbackMotionFunc);
	glutPassiveMotionFunc(callbackPassiveMotionFunc);
	glutReshapeFunc(callbackReshapeFunc); 
	glutVisibilityFunc(callbackVisibilityFunc);
	toogleIdleFunc();
	
	cout << "GL Version: " << glGetString(GL_VERSION) << endl;
	//	cout << glGetString(GL_EXTENSIONS) << endl;

#ifdef USE_GLEW
	// glew init, must come AFTER the OpenGL context creation (glutInit...)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cerr << "Error glew init: " << glewGetErrorString(err) << endl;
	}
#endif
	
	return windowID;
}
	
void GLViewerManager::deleteViewer(int id)
{
	if (viewers.erase(id) > 0) {
		glutDestroyWindow(id);
	}
}

int GLViewerManager::getCurrentViewerID()
{
	return glutGetWindow();
}

void GLViewerManager::setCurrentViewer(int id) 
{
	glutSetWindow(id);
}

void GLViewerManager::toogleIdleFunc()
{
	if (idleFuncEnabled == true) {
		// switch it off
		idleFuncEnabled = false;
		glutIdleFunc(callbackDisplayFunc); 
	}
	else {
		// switch it on
		idleFuncEnabled = true;
		glutIdleFunc(callbackIdleFunc); 
	}
}

	
void GLViewerManager::startMainLoop() 
{
	glutMainLoop();
}


///////////////////////////////////////////////////////////////////////////////
void GLViewerManager::idleFunc()
{
//	// something
//	... 
//	// call for redisplay
//	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////////

void GLViewerManager::callbackDisplayFunc() {
	viewers[glutGetWindow()]->displayFunc();
}

void GLViewerManager::callbackIdleFunc()
{
	idleFunc();
}

void GLViewerManager::callbackKeyboardFunc(unsigned char key, int x, int y) {
	viewers[glutGetWindow()]->keyboardFunc(key, x, y);
}

void GLViewerManager::callbackMotionFunc(int x, int y) {
	viewers[glutGetWindow()]->motionFunc(x, y);
}

void GLViewerManager::callbackMouseFunc(int button, int state, int x, int y) {
	viewers[glutGetWindow()]->mouseFunc(button, state, x, y);
}

void GLViewerManager::callbackPassiveMotionFunc(int x, int y) {
	viewers[glutGetWindow()]->passiveMotionFunc(x, y);
}

void GLViewerManager::callbackReshapeFunc(int w, int h) {
	viewers[glutGetWindow()]->reshapeFunc(w, h);
}

void GLViewerManager::callbackSpecialFunc(int key, int x, int y) {
	viewers[glutGetWindow()]->specialFunc(key, x, y);
}

void GLViewerManager::callbackVisibilityFunc(int visible) {
	viewers[glutGetWindow()]->visibilityFunc(visible);
}

