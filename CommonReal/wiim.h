/*
 * WARNING : pour que le code compile, il est nécessaire de taper la commande
 * export QMAKEFEATURES=~/RVProject/api_arv/qmake
 * (éventuellement, l'ajouter dans le .bashrc ou équivalent)
 */

#ifndef WIIM_H_
#define WIIM_H_

#include <ApiArv/Wii.h>

using namespace arv;
using namespace std;

class Wiim { 

protected:

	Wii* _wii;
	bool _stop;
	cwiid_ir_src* _wiimote1;
	cwiid_ir_src* _wiimote2;
	double acc1;
	double acc2;
	int wiiZone1;
	int wiiZone2;
	bool valid1;
	bool valid2;


public:

	void sigproc(int* i);

	Wiim();
	
	arv::Wii* getWii();

	bool getStop();
	
	int getWiiZone1();
	int getWiiZone2();

	bool getValid1();
	bool getValid2();

	float getAcc1();
	float getAcc2();
	
	void init();

	void getPos(float& pos1x, float& pos2x, float& pos1y, float& pos2y, double& acc1x, double& acc2x, double& acc1y, double& acc2y, double& acc1z, double& acc2z);

	void update();

	void getVal();

	void close();

};

#endif

