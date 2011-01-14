/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Camera.h>
#include <ApiArv/Tracker.h>
#include <iostream>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int argc, char* argv[]) {
    fprintf(stderr, "\nOptions: \n");
    fprintf(stderr, " -c 0 : to use Sony Motorized Camera (default parameter).\n");
    fprintf(stderr, " -c 1 : to use Philips TouCam Camera.\n\n");

    Camera::Type camType = Camera::SONY;
    for (int i = 1; i < argc; i++) {
        if (strcmp("-c", argv[i]) == 0) {
            i++;
            camType = static_cast<Camera::Type>(atoi(argv[i]));
            }
        else
            fprintf(stderr, "%s is not a supported option of %s.\n", argv[i], argv[0]);
        }

    cerr << "test artoolkit inteface.." << endl;
    Camera *camera = Camera::create(camType);
    if (camera == NULL) ERROR
    cout << "> camera initialized." << endl;

    int w = camera->width();
    int h = camera->height();
    cout << "> camera size : " << w << "x" << h << "." << endl;

    //by default 1 marker patt.hiro, default video size, default file calib
    Tracker *tracker = Tracker::create();
    if (tracker == NULL) ERROR
    cout << "> tracker initialized." << endl;
    
    tracker->setThreshold(100);
    cerr << "set threshold..OK" << endl;

    camera->start();

    for (int i = 0;i < 100;i++) {
        camera->update();
        Image image = camera->getImage();
        tracker->update(image);
        cerr << "update artoolkit..OK" << endl;

        if (tracker->isMarkerVisible(0)) {
            cerr << "marker visible.." << endl;
            double patt_trans[3][4];
            tracker->getPos(0, patt_trans);
            for (int i = 0;i < 3;i++) {
                fprintf(stderr, "%f %f %f %f\n", patt_trans[i][0], patt_trans[i][1], patt_trans[i][2], patt_trans[i][3]);
                }
            }
        }

    cerr << "end of aquisition.." << endl;
    camera->stop();
    
    delete camera;
    delete tracker;

    return 0;
    }
