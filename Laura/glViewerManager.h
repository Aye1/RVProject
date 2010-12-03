#ifndef _GLVIEWERMANAGER_H_
#define _GLVIEWERMANAGER_H_

#include <map>
#include <string>
using namespace std;


class BasicGLViewer;

class GLViewerManager
{
public:
	static void startMainLoop();	
	static int createViewer(BasicGLViewer & viewer,	const string & title,
	                        int width = 800, int height = 800,
	                        int initPositionX = 0, int initPositionY = 0);
	static void deleteViewer(int id);

	static int getCurrentViewerID();
	static void setCurrentViewer(int id);

	static void toogleIdleFunc();
	
protected:
	// idle function must be UNIQUE for all viewers
	static void idleFunc();

private:
	// all viewers
	static map<int, BasicGLViewer *> viewers;

	static bool idleFuncEnabled;
	
	// static callbacks, linked to glut callback functions
	static void callbackDisplayFunc();
	static void callbackIdleFunc();
	static void callbackKeyboardFunc(unsigned char key, int x, int y);
	static void callbackMotionFunc(int x, int y);
	static void callbackMouseFunc(int button, int state, int x, int y);
	static void callbackPassiveMotionFunc(int x, int y);
	static void callbackReshapeFunc(int w, int h);
	static void callbackSpecialFunc(int key, int x, int y);
	static void callbackVisibilityFunc(int visible);
}; 

#endif // _GLVIEWERMANAGER_H_
