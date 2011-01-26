
#include <ApiArv/Fob.h>
#include <iostream>

using namespace arv;
using namespace std;
using namespace qglviewer;

int init(Fob* fob) {
	cout << "Connecting FOB." << endl;
	fob = new Fob(Fob::HEMI_UPPER);
   	cout << "> FOB allocated." << endl;
   	fob->init();
   	cout << "> FOB initialized." << endl;
	return 1;
}

int getPosOri(Fob* fob, Vec& pos, Quaternion& ori) {

	fob->update();
	pos = fob->position();
	ori = fob->orientation();
	return 1;
}

int close(Fob* fob) {
	fob->close();
	cout << "> device closed." << endl;
	return 1;
}

