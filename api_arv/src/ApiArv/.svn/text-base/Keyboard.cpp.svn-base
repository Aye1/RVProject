/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */

#include "Keyboard.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <iostream>


namespace arv {

#define PRESSED(c, x)   ( c[x/8] & (1 << (x%8)) )

/*********************************************************************************************/

// this class allows to hide system dependent includes (Xlib) from the header :
class Keyboard::X {
    public:
        X() {
            display = XOpenDisplay(NULL);
            if (display == NULL) fprintf(stderr, "ERROR->KEYBOARD/INIT: can't open a X connection !!\n");
            }
        
        ~X() {
            if (display != NULL) XCloseDisplay(display);
            }

        bool ready() const { return display != NULL; }
        
    public:
        Display *display;
    };

/*********************************************************************************************/

Keyboard::Keyboard() : data(NULL) { }

bool Keyboard::init() {
    data = new X();
    if (!data->ready()) return false;
    update();
    for (int i=0; i<32; i++) lastState[i] = state[i];
    return true;
    }

void Keyboard::close() {
    if (data != NULL) {
        delete data;
        data = NULL;
        }
    }

void Keyboard::update() {
    for (int i=0; i<32; i++) lastState[i] = state[i];
    XQueryKeymap(data->display, state);
    }

/*********************************************************************************************/

bool            Keyboard::exists(char *s)     const { return XStringToKeysym(s) != NoSymbol; }
Keyboard::Code  Keyboard::keyCode(char *s)    const { return XKeysymToKeycode(data->display, XStringToKeysym(s)); }
char*           Keyboard::keyString(Code key) const { return XKeysymToString(XKeycodeToKeysym(data->display, key, 0)); }

std::vector<Keyboard::Code> Keyboard::activeKeys() const {
    std::vector<Code> res;
    for (unsigned int key=0; key<256; key++)
        if (PRESSED(state, key)) res.push_back(key);
    return res;
    }

bool Keyboard::isPressed(Code key)    const { return PRESSED(state, key); }
bool Keyboard::stateChanged(Code key) const { return (PRESSED(state, key) != PRESSED(lastState, key)); }
bool Keyboard::justPressed(Code key)  const { return stateChanged(key) &&  isPressed(key); }
bool Keyboard::justReleased(Code key) const { return stateChanged(key) && !isPressed(key); }

/*********************************************************************************************/

void Keyboard::grab() const {
    Window w =  DefaultRootWindow(data->display);
    XGrabKeyboard(data->display, w, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    }

void Keyboard::ungrab() const {
    XUngrabKeyboard(data->display, CurrentTime);
    }

}

