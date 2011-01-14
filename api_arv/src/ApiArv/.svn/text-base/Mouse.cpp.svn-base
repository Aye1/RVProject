/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#include "Mouse.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fstream>

namespace arv {

/*********************************************************************************************/

// this class allows to hide system dependent includes (Xlib) from the header :
class Mouse::X {
    public:
        X() {
            display = XOpenDisplay(NULL);
            if (display == NULL) fprintf(stderr, "ERROR->MOUSE/INIT: can't open a X connection !!\n");
            }
        
        ~X() {
            if (display != NULL) XCloseDisplay(display);
            }

        bool ready() const { return display != NULL; }
        
    public:
        Display *display;
    };

/*********************************************************************************************/

bool Mouse::init() {
    data = new X();
    if (!data->ready()) return false;
    update();
    for (int i=0; i<3; i++) statePrev[i] = state[i];
    return true;
    }

void Mouse::close() {
    if (data != NULL) {
        delete data;
        data = NULL;
        }
    }

void Mouse::update() {
    for (int i=0; i<3; i++) statePrev[i] = state[i];
    
    int win_x, win_y;
    Window root, child;
    unsigned int buttons;

    XQueryPointer(data->display, DefaultRootWindow(data->display),
                   &root, &child, &xCursor, &yCursor,
                   &win_x, &win_y, &buttons);

    state[0] = (buttons & Button1Mask) != 0;
    state[1] = (buttons & Button2Mask) != 0;
    state[2] = (buttons & Button3Mask) != 0;
    }

/*********************************************************************************************/

void Mouse::getPos(int *pos) const {
    pos[0] = xCursor;
    pos[1] = yCursor;
    }

int Mouse::xPos() const { return xCursor; }
int Mouse::yPos() const { return yCursor; }

bool Mouse::isPressed(Button b)     const { return state[b]; }
bool Mouse::stateChanged(Button b)  const { return state[b] != statePrev[b]; }

/*********************************************************************************************/

void Mouse::grab() const {
    Window w = DefaultRootWindow(data->display);
    Cursor c = None;
    Window confine_to = None;
    unsigned int event_mask = ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask;
    XGrabPointer(data->display, w, False, event_mask, GrabModeAsync, GrabModeAsync, confine_to, c, CurrentTime);
    }

void Mouse::ungrab() const {
    XUngrabPointer(data->display, CurrentTime);
    }

}

