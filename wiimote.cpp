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
    cout << "Connexion des wiimotes" << endl;
    Wii * wii = new Wii();
    cout << "> Wii allocated." << endl;

    cout << "> Adding the first wiimote" << endl;
    wii->addWiimote();
    wii->enableNunchuk(0);
    wii->enableMotionPlus(0);
    cwiid_ir_src* wii1;
    cwiid_ir_src* wii2;

    cout << "> Adding the second wiimote" << endl;
    wii->addWiimote();
 
    // juste pour vérifier que ça compile
    int taillecaisse4pieds = 150;
    int taillecaisse3pieds = 150;
    int taillecaisse2pieds = 150;
    int taillecaisse1pieds = 150;
    int taillepieds4 = 50;
    int taillepieds3 = 50;
    int taillepieds2 = 50;
    int taillepieds1 = 50;
    int lavariablequiditquelemonsieurilatape = 0;
    bool rumble = false;
    while(!stop) {
        if (wii->update()){
            wii1 = wii->getIRData();
            wii2 = wii->getIRData(1);
            if (wii1->valid) {
                if (wii1->pos[0] < 250) {
                    // dans la zone x de la caisse 4
                    if (wii1->pos[1] > (800 - taillecaisse4pieds) && wii1->pos[1] < (800 - taillepieds4)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 4;
                    }              
                } else if (wii1->pos[0] < 500) {
                    // dans la zone x de la caisse 3
                    if (wii1->pos[1] > (800 - taillecaisse3pieds) && wii1->pos[1] < (800 - taillepieds3)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 3;
                    }
                } else if (wii1->pos[0] < 750) {
                    // dans la zone x de la caisse 2
                    if (wii1->pos[1] > (800 - taillecaisse2pieds) && wii1->pos[1] < (800 - taillepieds2)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 2;
                    }
                } else if (wii1->pos[0] < 1000) {
                    // dans la zone x de la caisse 1
                    if (wii1->pos[1] > (800 - taillecaisse1pieds) && wii1->pos[1] < (800 - taillepieds1)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 1;
                    }
                }
            }
            if (wii2->valid) {
                if (wii2->pos[0] < 250) {
                    // dans la zone x de la caisse 4
                    if (wii2->pos[1] > (800 - taillecaisse4pieds) && wii2->pos[1] < (800 - taillepieds4)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 4;
                    }
                } else if (wii2->pos[0] < 500) {
                    // dans la zone x de la caisse 3
                    if (wii2->pos[1] > (800 - taillecaisse3pieds) && wii2->pos[1] < (800 - taillepieds3)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 3;
                    }
                } else if (wii2->pos[0] < 750) {
                    // dans la zone x de la caisse 2
                    if (wii2->pos[1] > (800 - taillecaisse2pieds) && wii2->pos[1] < (800 - taillepieds2)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 2;
                    }
                } else if (wii2->pos[0] < 1000) {
                    // dans la zone x de la caisse 1
                    if (wii2->pos[1] > (800 - taillecaisse1pieds) && wii2->pos[1] < (800 - taillepieds1)) {
                        // dans la zone y de la caisse 4
                        lavariablequiditquelemonsieurilatape = 1;
                    }
                }
            }
            cout << lavariablequiditquelemonsieurilatape << endl;
            int buttons = wii->getButtonPressed(0);
            if (buttons & CWIID_BTN_A) {
                if (!rumble) wii->startRumble(0);
                //else wii->stopRumble(0);
                rumble = !rumble;
            }else if (buttons & CWIID_BTN_B){
                if (rumble) wii->stopRumble(0);
                rumble = !rumble;
            }
        }
    }


    wii->close();
    cout << "> device closed." << endl;

    return 0;
}
