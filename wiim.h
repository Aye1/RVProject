/*
 * WARNING : pour que le code compile, il est nécessaire de taper la commande
 * export QMAKEFEATURES=~/RVProject/api_arv/qmake
 * (éventuellement, l'ajouter dans le .bashrc ou équivalent)
 */

#ifndef WIIM_H
#define WIIM_H

#include <ApiArv/Wii.h>

using namespace arv;

class Wiim {

protected:

	Wii* wii;
	Wiimote wiimote1;
	Wiimote wiimote2;
	Vec pos1;
	Vec pos2;
	int zone1;
	int zone2;
	bool valid1;
	bool valid2;


public:

	bool stop(Wii *wii);
	
	Wii* init(bool& stop);

	void getPos(Wii *wii, float& pos1x, float& pos2x, float& pos1y, float& pos2y);

	void getVal(Wii *wii, int& wiiZone1, int& wiiZone2, bool& valid1, bool& valid2, float& acc1, float& acc2);

	void close(Wii *wii);

}

#endif
