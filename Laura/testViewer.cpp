#include "testViewer.h"

TestViewer::TestViewer()
	: BasicGLViewer()
{
	displayReferential = true;
}

void TestViewer::initCamera()
{
	cameraAngle = M_PI*0.5f;
	distToOrigin = 8.0f;
	cameraHeight = 0.0f;
}

void TestViewer::displayFunc()
{
	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        	
	// set the scene:
	if(displayReferential)
		drawReferential();

	glPushMatrix();
	glTranslatef(2.0f, 0.0f, 0.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);
	glColor3fv(red);
	
	// Draw a square with two triangles (no strip)
	GLfloat vertices[] = {0.0f, 0.0f,
	                      1.0f, 0.0f,
	                      1.0f, 1.0f,
	                      0.0f, 1.0f };
	GLfloat normals[] = {-1.0f, -1.0f, 0.0f,
	                      1.0f, -1.0f, 0.0f,
	                      1.0f,  1.0f, 0.0f,
	                     -1.0f,  1.0f, 0.0f };
	GLfloat colors[] = {1.0f, 0.0f, 0.0f,
	                    1.0f, 1.0f, 0.0f,
	                    0.0f, 1.0f, 0.0f,
	                    1.0f, 1.0f, 0.0f };
	GLubyte indices [] = { 0,1,3, 3,1,2 };	
	
	// enable arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);
	glColorPointer(3, GL_FLOAT, 0, colors);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glPopMatrix();

	// render
	glutSwapBuffers();
}


void TestViewer::keyboardFunc(unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
			toogleDisplayReferential();
			glutPostRedisplay();
			break;
		default:
			// evant not handled here, let it to parent class
			BasicGLViewer::keyboardFunc(key, x, y);
	}
}

void TestViewer::toogleDisplayReferential() 
{
	displayReferential = !displayReferential;
}

void TestViewer::drawReferential() 
{	
	glBegin(GL_LINES);
		glColor3fv(red);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);

		glColor3fv(green);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);

		glColor3fv(blue);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();
}

// Test program
int main(int argc, char **argv)
{
	TestViewer window;
	window.init(string("Openg GL - TestViewer"),
	                 800, 800,    // height, width
	                 200, 100);   // initPosition (x,y)

	window.startMainLoop();	
	return(0);
}