/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Glove.h"
#include <cyberglove.h>
#include <iostream>

#define ERROR_RETURN(msg, ret) { std::cerr << "- error in arv::Glove : " << msg << "." << std::endl; return ret; }

namespace arv {

/******************************************************************************************/
// Glove::State implementation :

Glove::State::State() {
    for (int i=0; i<GLOVE_DOF; i++) val[i] = 0;
    }

void Glove::State::substract(const State &refState) {
    for (int i=0; i<GLOVE_DOF; i++)
        val[i] -= refState.val[i];
    }

void Glove::State::set(const State &state) {
    for (int i=0; i<GLOVE_DOF; i++)
        val[i] = state.val[i];
    }

/******************************************************************************************/
// Glove implementation :

Glove* Glove::create(SerialPort port) {
    CCyberGlove *glove = new CCyberGlove(getSystemName(port), B115200);
    if (glove == NULL) ERROR_RETURN("problem at initialization", NULL)
    return new Glove(glove);
    }

Glove::~Glove() {
    delete glove;
    }

void Glove::update(State &state) {
    glove->UpdateData();
    for (int i = 0; i < 18; i++)
        state(i) = glove->GetData(i);
    }

}

