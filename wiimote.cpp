//ligne à rajouter dans le .bashrc (ou dans le terminal si ça vous amuse de le faire à chaque fois) pour que ça compile bien les trucs de l'api_arv : export QMAKEFEATURES=~/RVProject/api_arv/qmake

#include <ApiArv/Wii.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include "wiim.h"

using namespace arv;
using namespace std;

int main(int /*argc*/, char* /*argv*/[]) {

	Wii *wii = NULL;
	wii = init();
	
	int wiiZone1 = 0;
	int wiiZone2 = 0;
	bool valid1 = false;
	bool valid2 = false;
	float acc1 = 0;
	float acc2 = 0;
	float pos1x, pos2x, pos1y, pos2y;
//	int i = 0;
	while (!stop(wii)) {
		getVal(wii, wiiZone1, wiiZone2, valid1, valid2, acc1, acc2);
		if (valid1) {
			cout << "wii1 validee dans la zone " << wiiZone1 << endl;
		}
		if (valid2) {
			cout << "wii2 validee dans la zone " << wiiZone2 << endl;
		}
		getPos(wii, pos1x, pos2x, pos1y, pos2y);
/*		if (i < 100000) {
			i++;
		} else {
			cout << "position 1 " << pos1x << " " << pos1y << " position 2 " << pos2x << " " << pos2y << endl;
			i = 0;
		}
*/	}
	close(wii);
	return 0;
}
