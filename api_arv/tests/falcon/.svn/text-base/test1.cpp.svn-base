/******************************************
 *  creation :
 *      2010 : Thibault Serot
 */

#include <ApiArv/Falcon.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>


using namespace arv;
using namespace std;

Falcon * falcon = NULL;

void sigproc(int i){
	if (falcon != NULL) {
        falcon->close();
        cout << "> device closed." << endl;
    }
    exit(0);
}

int main(int argc, char** argv) {
    signal(SIGINT, sigproc);
    signal(SIGQUIT, sigproc);

    cout << "Test for the Falcon inteface." << endl;
    falcon = new Falcon();
    cout << "> Falcon allocated." << endl;
    falcon->init();
    cout << "> Falcon initialized." << endl;
    boost::array<double, 3> pos;
    while (1){
        //if (!falcon->update()) break;
        if (falcon->update()){
            for (unsigned j = 0; j < falcon->size(); j++){
                pos = falcon->getPosition(j);
                std::cout << "Falcon " << j;
                std::cout << " Buttons : " << falcon->isButton1Pressed(j) << " " << falcon->isButton2Pressed(j) << " ";
                std::cout << falcon->isButton3Pressed(j) << " " << falcon->isButton4Pressed(j);
                std::cout << " position : " << pos[0] << " , " << pos[1] << " , " << pos[2] << std::endl;
            }
        }

    }



    return 0;
}
