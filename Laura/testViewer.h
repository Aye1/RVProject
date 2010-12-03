#ifndef _TEST_VIEWER_H_
#define _TEST_VIEWER_H_

#include "basicGLViewer.h"

/**
 * Example class of an openGL scene.
 * The display function is overiden to draw a referential and
 * a wireframe spere. A new keyboard event is handled to display
 * (or not) the referential.
 *
 * Only OpenGL 1.1 commands are used!
 */
class TestViewer : public BasicGLViewer
{
public:
	TestViewer();

protected:
	// init override
	virtual void initCamera();

	// callback overides
	virtual void displayFunc(void);   
	virtual void keyboardFunc(unsigned char key, int x, int y);

	bool displayReferential;
	void toogleDisplayReferential();
	void drawReferential();
};

#endif	// _TEST_VIEWER_H_
