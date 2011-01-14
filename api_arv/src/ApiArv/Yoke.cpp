/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Yoke.h"
#include <js.h>
#include <iostream>

using namespace std;

namespace arv {
    
#define ERROR_RETURN(msg, ret) { std::cerr << "- error in arv::Yoke : " << msg << "." << std::endl; return ret; }

Yoke* Yoke::create(int port) {
    jsInit();
    jsJoystick* handle = new jsJoystick(port);
    if (handle->notWorking()) ERROR_RETURN("problem at initialization", NULL);
    return new Yoke(handle);
    }

Yoke::Yoke(jsJoystick* handle) : handle(handle) {
    n = handle->getNumAxes();
    axes = new float[n];
    }
            
Yoke::~Yoke() {
    delete handle;
    delete[] axes;
    }

void Yoke::update() {
    handle->rawRead(&buttons, axes);    // 'rawRead' skips joystick calibration, as opposed to 'read'
    }

bool Yoke::buttonPressed(int i) const {
    return (buttons & (1 << i)) != 0;
    }

}

