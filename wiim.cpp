#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include "wiim.h"

using namespace arv;
using namespace std;


bool _stop;

void sigproc(int /*i*/) {
    _stop = true;
}

Wiim::Wiim() {
	wii = new Wii();
}

bool Wiim::stop(Wii *wii) {
	return _stop;
}

void Wiim::init() {
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);
	
	_stop = false;
	cout << "Connexion des wiimotes" << endl;
	cout << "> Wii allocated." << endl;
	
	cout << "> Adding the first wiimote" << endl;
	if (wii->addWiimote() == -1) {
		stop = true;
	}
		
	cout << "> Adding the second wiimote" << endl;
	if (wii->addWiimote() == -1) {
		stop = true;
	}
	return wii;
}

void Wiim::getPos(float& pos1x, float& pos2x, float& pos1y, float& pos2y) {
	cwiid_ir_src* wii1;
	cwiid_ir_src* wii2;
	double a1x, a1y, a1z, a2x, a2y, a2z;
	wii1 = wii->getIRData(0);
	wii2 = wii->getIRData(1);
	pos1x = wii1->pos[0];
	pos2x = wii2->pos[0];
	pos1y = wii1->pos[1];
	pos2y = wii2->pos[1];
}	

void Wiim::getVal(int& wiiZone1, int& wiiZone2, bool& valid1, bool& valid2, float& acc1, float& acc2) {
	
	cwiid_ir_src* wii1;
	cwiid_ir_src* wii2;
	
	double a1x, a1y, a1z, a2x, a2y, a2z;
	bool rumble = false;
	bool val1, val2;
	valid1 = false;
	valid2 = false;
	
	float seuil = 1.5;
	if (wii->update()) {
		wii1 = wii->getIRData(0);
		wii2 = wii->getIRData(1);
		wii->getAcceleration(a1x, a1y, a1z, 0); 
		wii->getAcceleration(a2x, a2y, a2z, 1);
		//val1 = (acc1 > seuil && a1y < acc1 && a1y > 0);
		//val2 = (acc2 > seuil && a2y < acc2 && a2y > 0);
		val1 = (a1y > seuil && acc1 < seuil && acc1 > 0);
		val2 = (a2y > seuil && acc2 < seuil && acc2 > 0);

		if (val1 || val2) {
			//blabla validate touche
			if (val1) {
				//		cout << "wii1 " << wiiZone1 << endl;
				valid1 = true;
			}
			if (val2) {
				//		cout << "wii2 " << wiiZone2 << endl;
				valid2 = true;
			}
		} else {
			if (wii1->size >= 3) {
				if (wii1->pos[0] < 225) {
					// dans la zone x de la caisse 4
					// if (wii1->pos[1] > (800 - taillecaisse4pieds) && wii1->pos[1] < (800 - taillepieds4)) {
					// dans la zone y de la caisse 4
					wiiZone1 = 4;
					//}              
				} else if (wii1->pos[0] > 250 && wii1->pos[0] < 450) {
					// dans la zone x de la caisse 3
					//if (wii1->pos[1] > (800 - taillecaisse3pieds) && wii1->pos[1] < (800 - taillepieds3)) {
					// dans la zone y de la caisse 4
					wiiZone1 = 3;
					//}
				} else if (wii1->pos[0] > 550 && wii1->pos[0] < 750) {
					// dans la zone x de la caisse 2
					// if (wii1->pos[1] > (800 - taillecaisse2pieds) && wii1->pos[1] < (800 - taillepieds2)) {
					// dans la zone y de la caisse 4
					wiiZone1 = 2;
					//}
				} else if (wii1->pos[0] > 775 && wii1->pos[0] < 1000) {
					// dans la zone x de la caisse 1
					// if (wii1->pos[1] > (800 - taillecaisse1pieds) && wii1->pos[1] < (800 - taillepieds1)) {
					// dans la zone y de la caisse 4
					wiiZone1 = 1;
					//}
				}	
			}
			if (wii2->size >= 3) {
				if (wii2->pos[0] < 225) {
					// dans la zone x de la caisse 4
					//if (wii2->pos[1] > (800 - taillecaisse4pieds) && wii2->pos[1] < (800 - taillepieds4)) {
					// dans la zone y de la caisse 4
					wiiZone2 = 4;
					//}
				} else if (wii2->pos[0] > 250 && wii2->pos[0] < 450) {
					// dans la zone x de la caisse 3
					//if (wii2->pos[1] > (800 - taillecaisse3pieds) && wii2->pos[1] < (800 - taillepieds3)) {
					// dans la zone y de la caisse 4
					wiiZone2 = 3;
					//}
				} else if (wii2->pos[0] > 550 && wii2->pos[0] < 750) {
					// dans la zone x de la caisse 2
					//if (wii2->pos[1] > (800 - taillecaisse2pieds) && wii2->pos[1] < (800 - taillepieds2)) {
					// dans la zone y de la caisse 4
					wiiZone2 = 2;
					//}
				} else if (wii2->pos[0] > 775 && wii2->pos[0] < 1000) {
					// dans la zone x de la caisse 1
					//if (wii2->pos[1] > (800 - taillecaisse1pieds) && wii2->pos[1] < (800 - taillepieds1)) {
					// dans la zone y de la caisse 4
					wiiZone2 = 1;
					//}
				}
			}
		}
		acc1 = a1y;
		acc2 = a2y;
		
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
    //}
	
}

void Wiim::close(Wii *wii) {
    wii->close();
    cout << "> device closed." << endl;
}
