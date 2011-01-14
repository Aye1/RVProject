/**********************************************************************
 * \file test3.cpp
 *
 * test de deux fob en mode master slave
 *
 * \author Konubinix  (konubinix@gmail.com)
 * \date jeu 11:56:00 28/01/2010
 ***********************************************************************/
#include <ApiArv/Fob.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>

using namespace arv;
using namespace qglviewer;

Fob *fob;

void display(void) {
	GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
	GLfloat mat_flash_shiny[] = {50.0};
	GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
	GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
	GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 0.1};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	/* Setup the view of the cube. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( /* field of view in degree */   40.0,
			/* aspect ratio */              1.0,
			/* Z near */                    1.0,
			/* Z far */                     200.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 90.0,   /* eye is at (0,0,5) */
		  0.0, 0.0, 0.0,    /* center is at (0,0,0) */
		  0.0, 1.0, 0.);    /* up is in positive Y direction */


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	// the fob base :
	glutSolidCube(8.0);

	fob->update();

	Vec p;
	Quaternion q;	    

	glPushMatrix();
	p = fob->position(1);
	q = fob->orientation(1);
	glTranslatef(p.x, p.y, p.z);
	glMultMatrixd(q.matrix());
	glScalef (2.0f, 1.0f, 4.0f);
	glutSolidCube(2.0);
	glPopMatrix();

	glPushMatrix();
	p = fob->position(2);
	q = fob->orientation(2);
	glTranslatef(p.x, p.y, p.z);
	glMultMatrixd(q.matrix());
	glScalef (2.0f, 1.0f, 4.0f);
	glutSolidCube(2.0);
	glPopMatrix();
    
	// or :
	// glMultMatrixd(fob->transform().matrix());

	// the bird :
	glutSwapBuffers();
}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
	if ((key == 27) || (key == 'q') || (key == 'Q')) {
		fprintf(stderr, "BYE\n");
		exit(0);
        }
}

int main(int argc, char* argv[]) {
	std::cout << "Test de 2 fobs en mode master/slaves" << std::endl;
	std::cout << "Le master doit Ãªtre branchÃ© sur /dev/ttyS1, le slave doit Ãªtre branchÃ© sur le master en FBB (RJ45?)" << std::endl;
	std::cout << "Le master doit Ãªtre d'adresse 1 (dipswitch Ã  011000010 (=UP DOWN DOWN UP UP UP UP DOWN UP))" << std::endl;
	std::cout << "Le slave doit Ãªtre d'adresse 2 (dipswitch Ã  011000100 (=UP DOWN DOWN UP UP UP DOWN UP UP))" << std::endl;
	fprintf(stderr, "Options: \n");
	fprintf(stderr, " -f : for fullscreen.\n");
	fprintf(stderr, "Press Esc or q to quit.\n");

	bool fullScreen = false;
	for (int i = 1; i < argc; i++) {
		if (strcmp("-f", argv[i]) == 0)
			fullScreen = true;
		else
			fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("ARV Fob GL Test");
	if (fullScreen)
		glutFullScreen();
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutKeyboardFunc(keyboard);

	fob = new Fob(Fob::HEMI_UPPER);
	fob->init(2);

	glutMainLoop();
	return 0;
}

