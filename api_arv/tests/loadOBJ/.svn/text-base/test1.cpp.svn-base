/******************************************
 *  creation :
 *      2010 : Thibault Serot
 */

#include <ApiArv/OBJLoader.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


using namespace arv;
using namespace std;

double a_theta = 0.0;
double a_phi = 0.0;
double a_psi = -9360.0;
ObjLoader * objLoader = NULL;

void display(void) {
    if (objLoader == NULL || !objLoader->isInit()) return;
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat mat_diffuse_red[] = {1.0, 1.0, 1.0, 1.0};
    //GLfloat mat_diffuse_red2[] = {1.0, 0.0, 0.0, 1.0};
    //GLfloat mat_diffuse_green[] = {0.0, 1.0, 0.0, 0.6};
    GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_flash_shiny[] = {20.0};
    //GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
    GLfloat light_position[] = {-10.0, 200, 1000.0, 1.0};
    GLfloat ambi[] = {0.1, 0.1, 0.1, 1.0};
    GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( /* field of view in degree */   59.57,
                    /* aspect ratio */              1.0,
                    /* Z near */                    1.0,
                    /* Z far */                     10000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-100.0, 450.0, 1000.0,   /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,    /* center is at (0,0,0) */
              0.0, 1.0, 0.);    /* up is in positive Y direction */

    //glRotatef(eye[0],1.0,0.0,0.0);

    glEnable(GL_LIGHTING);
    //glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,.000003);
    glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.002);

    glShadeModel(GL_SMOOTH);
    //glEnable(GL_COLOR_MATERIAL);

    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    //glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    // the fob base :
    glColor4f(1.0,1.0,1.0,1.0f);


    double theta = a_theta * M_PI / 180;
    double phi = a_phi * M_PI / 180;
    double psi = a_psi * M_PI / 180;

    glRotatef(theta,0.0,1.0,0.0);
    glRotatef(phi,0.0,0.0,1.0);
    glRotatef(psi,1.0,0.0,0.0);
    //std::cout << "a_theta = " << a_theta << " , a_phi = " << a_phi << " , a_psi = " << a_psi << std::endl;

    objLoader->render();


    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_green);
    //glTranslated(pos[0]*100,pos[1]*100,(pos[2] - 0.125) * 100);

    //glutSolidCube(0.3);

    //glTranslated(-pos[0]*100,-pos[1]*100,-(pos[2] - 0.125) * 100);



    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    //glutSolidCube(6.0);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red2);
    //glutWireCube(6.0);

    glutSwapBuffers();



}


void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if (key == 27) {
        if (objLoader) delete objLoader;
        cout << "> Exiting" << endl;
        exit(0);
    }
#define INC 45.0
    if (key == 'z') {
        a_theta += INC;
    }
    if (key == 's') {
        a_theta -= INC;
    }
    if (key == 'q') {
        a_phi += INC;
    }
    if (key == 'd') {
        a_phi -= INC;
    }
    if (key == 'a') {
        a_psi += INC;
    }
    if (key == 'e') {
        a_psi -= INC;
    }


}


int main(int argc, char* argv[]) {
    char * filename = NULL;
    //char * texture = NULL;
    if (argc == 1){
        filename = (char*)"obj/wiimote.obj";
        //texture = (char*)"obj/wiimote.jpg";
    }else if (argc == 2){
        filename = argv[1];
        //texture = NULL;
    }else if (argc == 3) {
        filename = argv[1];
        //texture = argv[2];
    }else {
        std::cout << "Too many parameters, usage : " << argv[0] << " filename.obj" << std::endl;
        return 0;
    }

    cout << ">Starting openGL" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Falcon GL Test");
    //if (fullScreen)
    //    glutFullScreen();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    
    cout << ">Test for loading an obj." << endl;
    objLoader = new ObjLoader(filename);
    glutMainLoop();

    return 0;
}
