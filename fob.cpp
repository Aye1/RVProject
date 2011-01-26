#include <ApiArv/Fob.h>
#include <iostream>
#include "fob.h"

using namespace arv;
using namespace std;
using namespace qglviewer;

int init(Fob* fob) {
	cout << "Connecting FOB." << endl;
	fob = new Fob(Fob::HEMI_UPPER);
   	cout << "> FOB allocated." << endl;
   	fob->init();
   	cout << "> FOB initialized." << endl;
	return 0;
}

int getPosAndOri(Fob* fob, Vec& pos, Quaternion& ori) {
	
	fob->update();
	pos = fob->position();
	ori = fob->orientation();
	return 0;
}

int close(Fob* fob) {
	fob->close();
	cout << "> Device connection closed." << endl;
	return 0;
}
