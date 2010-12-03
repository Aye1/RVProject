#ifndef _BASICGLVIEWER_H_
#define _BASICGLVIEWER_H_

#include <string>
using namespace std;

#ifdef USE_GLEW
	#include <GL/glew.h>	// OpenGL extensions
#endif
#include <GL/glut.h>
#include <GL/glu.h>

#include <cmath>
#ifndef M_PI
	#define M_PI 3.14159265358979323846f
#endif

class GLViewerManager;

/**
 * Basis of a GL windows.
 * All the callback functions can be overridden is sub-classes; they
 * will be automatically linked to gl callbacks.
 * Although it is possible, the use of multiple windows in a single 
 * application is not optimized. A better OOP viewer should use
 * a MVC pattern! (and not glut, which is definitely *NOT* object-oriented!!)
 * 
 * Some references about this class & the manager class 
 * (and generally speaking about oop & glut):
 *  http://paulsolt.com/2009/07/openglglut-classes-oop-and-problems/
 *  http://www.stetten.com/george/glutmaster/glutmaster.html
 *  http://www.itu.dk/courses/IG/F2005/Lectures/index.html
 *  http://www.songho.ca/opengl/gl_mvc.html
 */
class BasicGLViewer
{
	// My manager is a friend... Non-sense?
	friend class GLViewerManager;

public:
	BasicGLViewer();
	virtual ~BasicGLViewer();

	// I would have liked to directly init in the constructor, but it is 
	// not possible in C++: late binding of virtual init subfunctions
	// (initCamera(), ...) won't work if called during the construction 
	// of base class.
	// see: Never Call Virtual Functions during Construction or Destruction
	//      http://www.artima.com/cppsource/nevercall.html
	//      (Effective C++, Third Edition by Scott Meyers)
	// Hence the object must FIRST be created, then INIT is to be called... yuk!
	virtual void init(const string & title, int width, int height,
	          int initPositionX, int initPositionY);
	
	bool isCurrentViewer() const;
	void setAsCurrentViewer();
	
	void startMainLoop();

	// usual colors
	static const float black[];
	static const float red[];
	static const float green[];
	static const float yellow[];
	static const float blue[];
	static const float magenta[];
	static const float cyan[];
	static const float white[];

protected:
	
	// camera
	float cameraAngle;
	float distToOrigin;
	float cameraHeight;
	void setCamera();
	
	// init methods (to override in children, if required)
	virtual void initGlut(int width, int height, int initPositionX, int initPositionY);
	virtual void initCamera();
	virtual void initDepth();
	virtual void initRendering();
	virtual void initLighting();
	virtual void initTexture();
	
	// callbacks (to override in children, if required)
	virtual void displayFunc();
	virtual void keyboardFunc(unsigned char key, int x, int y);
	virtual void motionFunc(int x, int y);
	virtual void mouseFunc(int button, int state, int x, int y);
	virtual void passiveMotionFunc(int x, int y);
	virtual void reshapeFunc(int w, int h);   
	virtual void specialFunc(int key, int x, int y);   
	virtual void visibilityFunc(int visible);
	
private:
	void setViewerID(int id);
	int getViewerID() const;
	int viewerID;		
};

#endif	// _BASICGLVIEWER_H_
