//ligne à rajouter dans le .bashrc (ou dans le terminal si ça vous amuse de le faire à chaque fois) pour que ça compile bien les trucs de l'api_arv : export QMAKEFEATURES=~/RVProject/api_arv/qmake

#include <ApiArv/Wii.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include "wiim.h"

//using namespace arv;
//using namespace std;

int main(int argc, char* argv[]) {


	Wiim wiim = Wiim();
	wiim.init();
	if (wiim.getStop()) {
		cout << "ERREUR : lors de (of the rings) l'ajout des wiimotes" << endl;
		exit(0);
	}
	float pos1x, pos2x, pos1y, pos2y;

	while (!wiim.getStop()) {
		wiim.getVal();
		if (wiim.getValid1()) {
			cout << "wii1 validee dans la zone " << wiim.getWiiZone1() << endl;
		}
		if (wiim.getValid2()) {
			cout << "wii2 validee dans la zone " << wiim.getWiiZone2() << endl;
		}
		wiim.getPos(pos1x, pos2x, pos1y, pos2y);
	}
	wiim.close();
	return 0;
}

