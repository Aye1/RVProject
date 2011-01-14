/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Camera.h>
#include <iostream>
using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int argc, char *argv[]) {
    cout << "Options:" << endl;
    cout << " -c 0 : to use Sony Motorized Camera (default parameter)." << endl;
    cout << " -c 1 : to use Philips ToUcam Camera.:" << endl;

    Camera::Type camType = Camera::SONY;

    for (int i=1; i<argc; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            i++;
            camType = static_cast<Camera::Type>(atoi(argv[i]));
            }
        else
            cout << "- warnin : " << argv[i] << "is not a supported option of " << argv[0] << "." << endl;
        }

    cout << "Test for camera inteface." << endl;

    Camera* camera = Camera::create(camType);
    if (camera == NULL) ERROR
    cout << "> camera allocated and initialized." << endl;
    
    cout << "size = (" << camera->width() << ", " << camera->height() << ")" << endl;

    camera->start();
    cout << "> camera started." << endl;

    camera->update();
    cout << "> camera updated." << endl;

    Image image = camera->getImage();
    cout << "> image obtained." << endl;
    
    camera->stop();
    cout << "> camera stopped." << endl;
    
    delete camera;
    cout << "> camera closed." << endl;
    
    return 0;
    }
