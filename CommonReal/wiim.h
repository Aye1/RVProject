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
	/* booleen permettant de savoir si les deux wiimotes sont bien détectées */
	bool _stop;
	cwiid_ir_src* _wiimote1;
	cwiid_ir_src* _wiimote2;
	double acc1;
	double acc2;
	float _pos1x, _pos2x;
	/* zone de validation des wiimotes */
	int wiiZone1;
	int wiiZone2;
	/* booléen indiquant si une caisse est frappée */
	bool valid1;
	bool valid2;
	/* Seuil bloquant la zone */
	float _seuilPos;
	/* Seuil validant la frappe d'une caisse */
	float _seuilVal;

public:

	void sigproc(int* i);

	Wiim();
	
	arv::Wii* getWii();

	bool getStop();

	float getSeuilPos();
	float getSeuilVal();
	
	int getWiiZone1();
	int getWiiZone2();

	bool getValid1();
	bool getValid2();

	float getAcc1();
	float getAcc2();
	
	/* initialisation des deux wiimotes */
	void init();

	/* récupère position et accélérations de la wiimote1 */
	void getPos1(float& pos1x, float& pos1y, double& acc1x, double& acc1y, double& acc1z);

	/* récupère position et accélérations de la wiimote2 */
	void getPos2(float& pos2x, float& pos2y, double& acc2x, double& acc2y, double& acc2z);
	
	void update();
	
	/* met à jour les booléens valid1 et valid2 */
	void getVal();

	void close();

};

#endif

