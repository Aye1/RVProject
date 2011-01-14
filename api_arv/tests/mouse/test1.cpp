/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Mouse.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int /*argc*/, char* /*argv*/[]) {
    const float t  = 1.0f;     // duration of the test in seconds
    const float dt = 0.02f;    // intervals between updates in seconds

    Mouse *mouse = new Mouse(); cout << "> mouse allocated." << endl;
    if (!mouse->init()) ERROR
    cout << "> mouse initialized." << endl;

    const int N = (int)ceilf(t / dt);
    const long secs  = (int) floorf(dt);
    const long nanos = (long) (1000000000L * fmodf(dt, 1.0f));
    
    mouse->grab();
    cout << "> mouse grabbed." << endl;
    for (int i=0; i<N; i++) {
        mouse->update();

        cout << "pos = (" << mouse->xPos() << "," << mouse->yPos() << ") "
             << "buttons = " << mouse->isPressed(Mouse::LEFT) << "," << mouse->isPressed(Mouse::MIDDLE) << "," << mouse->isPressed(Mouse::RIGHT)
             << endl;

        struct timespec t = { secs, nanos };
        nanosleep(&t, NULL);
        }
    mouse->ungrab();
    cout << "> mouse ungrabbed." << endl;
    mouse->close();
    cout << "> mouse acquisition closed." << endl;
    
    return 0;
    }

