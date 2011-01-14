/******************************************
 *  creation :
 *      2010 : Thibault Serot
 */

#include <ApiArv/Falcon.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cstring>
#include <math.h>

using namespace arv;
using namespace std;

Falcon * falcon = NULL;
boost::array<double, 3> pos;
boost::array<double, 3> eye;
boost::array<double, 3> cornerA;
boost::array<double, 3> cornerB;
double stiffness = 1000.0;
bool isInitializing = true;
bool msgPrinted = false;

double a_theta = 0.0;
double a_phi = 0.0;
double radius = 10;


void display(void) {
    GLfloat mat_ambient[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat mat_diffuse_red[] = {1.0, 0.0, 0.0, 0.2};
    GLfloat mat_diffuse_red2[] = {1.0, 0.0, 0.0, 1.0};
    GLfloat mat_diffuse_green[] = {0.0, 1.0, 0.0, 0.6};
    GLfloat mat_flash[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_flash_shiny[] = {20.0};
    //GLfloat light_position[] = {100.0, -200.0, 200.0, 0.0};
    GLfloat light_position[] = {-10.0, 7.0 + 3, 10.0, 0.0};
    GLfloat ambi[] = {0.1, 0.1, 0.1, 0.1};
    GLfloat lightZeroColor[] = {0.9, 0.9, 0.9, 1.0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    /* Setup the view of the cube. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective( /* field of view in degree */   59.57,
                    /* aspect ratio */              1.0,
                    /* Z near */                    1.0,
                    /* Z far */                     1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-10.0, 7.0, 10.0,   /* eye is at (0,0,5) */
              0.0, 0.0, 0.0,    /* center is at (0,0,0) */
              0.0, 1.0, 0.);    /* up is in positive Y direction */

    //glRotatef(eye[0],1.0,0.0,0.0);


    glEnable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

    // the fob base :
    //glColor4f(1.0,0.0,0.0,0.2f);

    

    double theta = a_theta * M_PI / 180;
    double phi = a_phi * M_PI / 180;

    glRotatef(theta,0.0,1.0,0.0);
    glRotatef(phi,0.0,0.0,1.0);
   


    if (falcon->update(0)){
        pos = falcon->getPosition(0);
        if (isInitializing) {
            if (!msgPrinted){
                cout << "Move the end effector all the way out" << endl;
                msgPrinted = true;
            }
            if (pos[2] > 0.170) {
                cout << "Starting cube simulation..." << endl;
                isInitializing = false;
            }
        }else{
            boost::array<double, 3> force;
            double dist = 10000;
            int closest = -1;
            int outside = 3;
            for (int axis = 0; axis < 3; axis++){
                force[axis] = 0;
                if (pos[axis] > cornerA[axis] && pos[axis] < cornerB[axis]){
                    double dA = pos[axis] - cornerA[axis];
                    double dB = pos[axis] - cornerB[axis];
                    if (fabs(dA) < fabs(dist)) {
                        dist = dA;
                        closest = axis;
                    }
                    if (fabs(dB) < fabs(dist)){
                        dist = dB;
                        closest = axis;
                    }
                    outside--;
                }
            }
            if (closest > -1 && !outside) force[closest] = - stiffness * dist;
            falcon->setForce(force,0);

            if (falcon->isButton2Pressed()) {
                a_theta += 10.0;
            }
            
            if (falcon->isButton3Pressed()) {
                a_theta -= 10.0;
            }

            if (falcon->isButton4Pressed()) {
                a_phi += 10.0;
            }

            if (falcon->isButton1Pressed()) {
                a_phi -= 10.0;
            }

            //double theta = a_theta * M_PI / 180;
            //double phi = a_phi * M_PI / 180;

            //eye[0] = sin(theta) * cos(phi);
            //eye[1] = sin(theta) * sin(phi);
            //eye[2] = cos(theta);



        }


    }

    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_green);
    glTranslated(pos[0]*100,pos[1]*100,(pos[2] - 0.125) * 100);

    glutSolidCube(0.3);

    glTranslated(-pos[0]*100,-pos[1]*100,-(pos[2] - 0.125) * 100);



    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red);
    glutSolidCube(6.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_red2);
    glutWireCube(6.0);



    //fob->update();

    //Vec p = fob->position();
    //Quaternion q = fob->orientation();
    //glTranslatef(p.x, p.y, p.z);
    //glMultMatrixd(q.matrix());
    //
    //// or :
    //// glMultMatrixd(fob->transform().matrix());

    //// the bird :
    //glutSolidCube(2.0);

    glutSwapBuffers();



}

void keyboard(unsigned char key, int /*x*/, int /*y*/) {
    if ((key == 27) || (key == 'q') || (key == 'Q')) {
        //fprintf(stderr, "BYE\n");
        falcon->close();
        cout << "> device closed." << endl;
        exit(0);
    }
}
void mouse(int state, int button, int /*x*/, int /*y*/) {

}
//void sigproc(int /*i*/){
//	if (falcon != NULL) {
//        falcon->close();
//        cout << "> device closed." << endl;
//    }
//    exit(0);
//}



int main(int argc, char** argv) {
    //signal(SIGINT, sigproc);
    //signal(SIGQUIT, sigproc);

	
    cout << "Options:" << endl;
    cout << " -f : for fullscreen." << endl;
    cout << "Press Esc or q to quit." << endl;

    bool fullScreen = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp("-f", argv[i]) == 0){
            fullScreen = true;
        }else{
            cout << argv[i] << " is not a supported option of " << argv[0] << endl;
        }
    }

    cout << ">Starting openGL" << endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("ARV Falcon GL Test");
    if (fullScreen)
        glutFullScreen();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    falcon = new Falcon();
    cout << "> Falcon allocated." << endl;
    falcon->init();
    cout << "> Falcon initialized." << endl;
    //eye[0] = -10.0;
    //eye[1] = 7.0;
    //eye[2] = 10.0;

    eye[0] = 0.0;
    eye[1] = 0.0;
    eye[2] = 10.0;

    cout << "> Initialisation of the cube" << endl;
    cornerA[0] = -0.030;
    cornerA[1] = -0.030;
    cornerA[2] =  0.095;
    
    cornerB[0] = 0.030;
    cornerB[1] = 0.030;
    cornerB[2] = 0.155;

    pos[0] = 0.0;
    pos[1] = 0.0;
    pos[2] = 0.0;

    glutMainLoop();
    return 0;


    while(1){

        if (falcon->update(0)){
            pos = falcon->getPosition(0);
            if (isInitializing) {
                if (!msgPrinted){
                    cout << "Move the end effector all the way out" << endl;
                    msgPrinted = true;
                }
                if (pos[2] > 0.170) {
                    cout << "Starting cube simulation..." << endl;
                    isInitializing = false;
                }
            }else{
                boost::array<double, 3> force;
                double dist = 10000;
                int closest = -1;
                int outside = 3;
                for (int axis = 0; axis < 3; axis++){
                    force[axis] = 0;
                    if (pos[axis] > cornerA[axis] && pos[axis] < cornerB[axis]){
                        double dA = pos[axis] - cornerA[axis];
                        double dB = pos[axis] - cornerB[axis];
                        if (fabs(dA) < fabs(dist)) {
                            dist = dA;
                            closest = axis;
                        }
                        if (fabs(dB) < fabs(dist)){
                            dist = dB;
                            closest = axis;
                        }
                        outside--;
                    }
                }
                if (closest > -1 && !outside) force[closest] = - stiffness * dist;
                falcon->setForce(force,0);
            }


        }

    }
}
