#include "wiim.h"
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>

using namespace arv;
using namespace std;


void Wiim::sigproc(int* i) {
	_stop = true;
}

Wiim::Wiim() {
	_wii = new Wii();
	_stop = false;
	_seuilPos = 1.0;
	_seuilVal = 1.5;
	acc1 = 0;
	acc2 = 0;
	wiiZone1 = 0;
	wiiZone2 = 0;
	valid1 = false;
	valid2 = false;
}

// accesseurs
Wii* Wiim::getWii() {
	return _wii;
}

bool Wiim::getStop() {
	return _stop;
}

float Wiim::getSeuilPos() {
	return _seuilPos;
}

float Wiim::getSeuilVal() {
	return _seuilVal;
}

int Wiim::getWiiZone1() {
	return wiiZone1;
}

int Wiim::getWiiZone2() {
	return wiiZone2;
}

bool Wiim::getValid1() {
	return valid1;
}

bool Wiim::getValid2() {
	return valid2;
}

float Wiim::getAcc1() {
	return acc1;
}

float Wiim::getAcc2() {
	return acc2;
}


void Wiim::init() {
	
	_stop = false;
	cout << "Connexion des wiimotes" << endl;
	cout << "> Wii allocated." << endl;
	
	cout << "> Adding the first wiimote" << endl;
	if (_wii->addWiimote() == -1) {
		_stop = true;
	}
		
	cout << "> Adding the second wiimote" << endl;
	if (_wii->addWiimote() == -1) {
		_stop = true;
	}
	_wii->update();
	_wiimote1 = _wii->getIRData(0);
	_wiimote2 = _wii->getIRData(1);
	_pos1x = _wiimote1->pos[0];
	_pos2x = _wiimote2->pos[0];
}

void Wiim::getPos1(float& pos1x, float& pos1y, double& acc1x, double& acc1y, double& acc1z) {
	_wiimote1 = _wii->getIRData(0);
	_wii->getAcceleration(acc1x, acc1y, acc1z, 0);
	if (acc1y > _seuilPos) {
		pos1x = _pos1x;
	} else {
		pos1x = _wiimote1->pos[0];
	}
	pos1y = _wiimote1->pos[1];
	if (_wiimote1->size >= 3) {
		if (_wiimote1->pos[0] < 225) {
			wiiZone1 = 4;
		} else if (_wiimote1->pos[0] > 250 && _wiimote1->pos[0] < 450) {
			wiiZone1 = 3;
		} else if (_wiimote1->pos[0] > 550 && _wiimote1->pos[0] < 750) {
			wiiZone1 = 2;
		} else if (_wiimote1->pos[0] > 775 && _wiimote1->pos[0] < 1000) {
			wiiZone1 = 1;
		}
	}
	_pos1x = pos1x;
}	

void Wiim::getPos2(float& pos2x, float& pos2y, double& acc2x, double& acc2y, double& acc2z) {
	_wiimote2 = _wii->getIRData(1);
	_wii->getAcceleration(acc2x, acc2y, acc2z, 1);
	if (acc2y > _seuilPos) {
		pos2x = _pos2x;
	} else {
		pos2x = _wiimote2->pos[0];
	}
	pos2y = _wiimote2->pos[1];
	if (_wiimote2->size >= 3) {
		if (_wiimote2->pos[1] < 225) {
			wiiZone2 = 4;
		} else if (_wiimote2->pos[1] > 250 && _wiimote2->pos[1] < 450) {
			wiiZone2 = 3;
		} else if (_wiimote2->pos[1] > 550 && _wiimote2->pos[1] < 750) {
			wiiZone2 = 2;
		} else if (_wiimote2->pos[1] > 775) {
			wiiZone2 = 1;
		}
	}
	_pos2x = pos2x; 
}	


void Wiim::update() {
	_wii->update();
}
	
void Wiim::getVal() {
	
	double a1x, a1y, a1z, a2x, a2y, a2z;
	bool val1, val2;
	valid1 = false;
	valid2 = false;
	
	_wiimote1 = _wii->getIRData(0);
	_wiimote2 = _wii->getIRData(1);
	_wii->getAcceleration(a1x, a1y, a1z, 0); 
	_wii->getAcceleration(a2x, a2y, a2z, 1);
	val1 = (a1y > _seuilVal && acc1 < _seuilVal && acc1 > 0);
	val2 = (a2y > _seuilVal && acc2 < _seuilVal && acc2 > 0);
	if (val1 || val2) {
		if (val1) {
			valid1 = true;
		}
		if (val2) {
			valid2 = true;
		}
	} else {
		if (_wiimote1->size >= 3 && a1y < _seuilPos) {
			if (_wiimote1->pos[0] < 225) {
				wiiZone1 = 4;
			} else if (_wiimote1->pos[0] > 250 && _wiimote1->pos[0] < 450) {
				wiiZone1 = 3;
			} else if (_wiimote1->pos[0] > 550 && _wiimote1->pos[0] < 750) {
				wiiZone1 = 2;
			} else if (_wiimote1->pos[0] > 775 && _wiimote1->pos[0] < 1000) {
				wiiZone1 = 1;
			}	
		}
		if (_wiimote2->size >= 3 && a2y < _seuilPos) {
			if (_wiimote2->pos[0] < 225) {
				wiiZone2 = 4;
				
			} else if (_wiimote2->pos[0] > 250 && _wiimote2->pos[0] < 450) {
				wiiZone2 = 3;
			} else if (_wiimote2->pos[0] > 550 && _wiimote2->pos[0] < 750) {
				wiiZone2 = 2;
			} else if (_wiimote2->pos[0] > 775 && _wiimote2->pos[0] < 1000) {
				wiiZone2 = 1;
			}
		}
		acc1 = a1y;
		acc2 = a2y;
		
	}	
}

void Wiim::close() {
    _wii->close();
    cout << "> device closed." << endl;
}
