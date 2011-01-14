/******************************************
 *  creation :
 *      2010 : Thibault Serot
 */

#include <ApiArv/Wii.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>


using namespace arv;
using namespace std;

bool stop;

void sigproc(int /*i*/){
    stop = true;
}

int main(int /*argc*/, char* /*argv*/[]) {
    signal(SIGINT, sigproc);
    signal(SIGQUIT, sigproc);

    stop = false;
    cout << "Test for the Wii inteface." << endl;
    Wii * wii = new Wii();
    cout << "> Wii allocated." << endl;

    cout << "> Adding a wiimote" << endl;
    wii->addWiibalance();
    while(!stop) {
        if (wii->update()){
            //double right_top, right_bottom, left_top, left_bottom;
            //wii->getSensorBalance(right_top, right_bottom, left_top, left_bottom,0);
            //std::cout << right_top << " " << right_bottom << " " << left_top << " " << left_bottom << std::endl;
            std::cout << "Weight : " << wii->computeWeight() << std::endl;
        }

    }
    
    wii->close();
    cout << "> device closed." << endl;

    return 0;
}
