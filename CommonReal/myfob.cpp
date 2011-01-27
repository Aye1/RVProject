#include <ApiArv/Fob.h>
#include  <QGLViewer/qglviewer.h>
#include <iostream>
#include "myfob.h"

using namespace arv;
using namespace std;
using namespace qglviewer;


int myFob::init() {
	cout << "Connecting FOB." << endl;
	fob = new Fob(Fob::HEMI_UPPER);
   	cout << "> FOB allocated." << endl;
   	fob->init();
   	cout << "> FOB initialized." << endl;
	return 0;
}

int myFob::getPosAndOri(Vec& pos, Quaternion& ori) {
	
	fob->update();
	pos = fob->position();
	ori = fob->orientation();
	return 0;
}

int myFob::close() {
	fob->close();
	cout << "> Device connection closed." << endl;
	return 0;
}
