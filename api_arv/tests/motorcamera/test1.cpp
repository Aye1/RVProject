/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/MotorizedCamera.h>
#include <iostream>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int /*argc*/, char* /*argv[]*/) {
    cout << "Test for motorized camera inteface." << endl;

    MotorizedCamera *camera = MotorizedCamera::create();
    if (camera == NULL) ERROR
    cout << "> camera initialized." << endl;

    int w = camera->width();
    int h = camera->height();
    cout << "> camera size : (" << w << "," << h << ")." << endl;
    
    camera->start();
    cout << "start camera..OK" << endl;
    
    camera->update();
    cout << "update camera..OK" << endl;
    
    Image image = camera->getImage();
    cout << "get image camera..OK" << endl;

    camera->panAndTilt(20, 20);
    cout << "pan tilt camera..OK" << endl;
    
    camera->pan(-20);
    cout << "pan camera..OK" << endl;
    
    camera->pan(40, true);
    cout << "pan relative camera..OK" << endl;
    
    camera->tilt(-20);
    cout << "tilt camera..OK" << endl;
    
    camera->tilt(40, true);
    cout << "tilt relative camera..OK" << endl;
    
    camera->zoom(200);
    cout << "zoom camera..OK" << endl;
    
    camera->zoom(-100, true);
    cout << "zoom relative camera..OK" << endl;

    camera->stop();
    cout << "stop camera..OK" << endl;
    
    delete camera;
    cout << "close camera..OK" << endl;
    
    return 0;
    }
