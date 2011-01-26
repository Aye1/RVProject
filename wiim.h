//ligne à rajouter dans le .bashrc (ou dans le terminal si ça vous amuse de le faire à chaque fois) pour que ça compile bien les trucs de l'api_arv : export QMAKEFEATURES=~/RVProject/api_arv/qmake

#include <ApiArv/Wii.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>


using namespace arv;
using namespace std;


bool stop = false;

void sigproc(int /*i*/) {
    stop = true;
}

//int main(int /*argc*/, char* /*argv*/[]) {
    
Wii* init() {
	signal(SIGINT, sigproc);
	signal(SIGQUIT, sigproc);

	stop = false;
	cout << "Connexion des wiimotes" << endl;
	Wii* wii = new Wii();
	cout << "> Wii allocated." << endl;

	cout << "> Adding the first wiimote" << endl;
	wii->addWiimote();
	//cwiid_ir_src* wii1;
	//cwiid_ir_src* wii2;

	cout << "> Adding the second wiimote" << endl;
	wii->addWiimote();
	return wii;
}

void getPos(Wii *wii, float& pos1x, float& pos2x, float& pos1y, float& pos2y) {
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
 
void getVal(Wii *wii, int& wiiZone1, int& wiiZone2, bool& valid1, bool& valid2, float& acc1, float& acc2) {
 
	// juste pour vérifier que ça compile
	/*int taillecaisse4pieds = 150;	
	int taillecaisse3pieds = 150;
	int taillecaisse2pieds = 150;
	int taillecaisse1pieds = 150;
	int taillepieds4 = 50;
	int taillepieds3 = 50;
	int taillepieds2 = 50;
	int taillepieds1 = 50;*/
	cwiid_ir_src* wii1;
	cwiid_ir_src* wii2;
//	wiiZone1 = 0;
//	wiiZone2 = 0;
	double a1x, a1y, a1z, a2x, a2y, a2z;
	bool rumble = false;
	valid1 = false;
	valid2 = false;
//	while(!stop) {
	if (wii->update()) {
		wii1 = wii->getIRData(0);
		wii2 = wii->getIRData(1);
		wii->getAcceleration(a1x, a1y, a1z, 0); 
		wii->getAcceleration(a2x, a2y, a2z, 1);
		if (a1y > 1.5 || a2y > 1.5 ) {
			//blabla validate touche
			if (a1y > 1.5 && acc1 < 1.5) {
		//		cout << "wii1 " << wiiZone1 << endl;
				valid1 = true;
			}
			if (a2y > 1.5 && acc2 < 1.5) {
		//		cout << "wii2 " << wiiZone2 << endl;
				valid2 = true;
			}
	    	} else {
	    		if (wii1->size >= 3) {
                		if (wii1->pos[0] < 200) {
               				// dans la zone x de la caisse 4
                			// if (wii1->pos[1] > (800 - taillecaisse4pieds) && wii1->pos[1] < (800 - taillepieds4)) {
                        		// dans la zone y de la caisse 4
		                        wiiZone1 = 4;
                			//}              
	        	        } else if (wii1->pos[0] > 275 && wii1->pos[0] < 425) {
        		            // dans la zone x de la caisse 3
        		            //if (wii1->pos[1] > (800 - taillecaisse3pieds) && wii1->pos[1] < (800 - taillepieds3)) {
        		                // dans la zone y de la caisse 4
        		                wiiZone1 = 3;
        		            //}
        		        } else if (wii1->pos[0] > 575 && wii1->pos[0] < 725) {
        		            // dans la zone x de la caisse 2
        		           // if (wii1->pos[1] > (800 - taillecaisse2pieds) && wii1->pos[1] < (800 - taillepieds2)) {
        		                // dans la zone y de la caisse 4
        		                wiiZone1 = 2;
        		            //}
        		        } else if (wii1->pos[0] > 800 && wii1->pos[0] < 1000) {
        		            // dans la zone x de la caisse 1
        		           // if (wii1->pos[1] > (800 - taillecaisse1pieds) && wii1->pos[1] < (800 - taillepieds1)) {
        		                // dans la zone y de la caisse 4
        		                wiiZone1 = 1;
                    		//}
                		}	
            		}
            		if (wii2->size >= 3) {
                		if (wii2->pos[0] < 250) {
                    			// dans la zone x de la caisse 4
                    			//if (wii2->pos[1] > (800 - taillecaisse4pieds) && wii2->pos[1] < (800 - taillepieds4)) {
                        		// dans la zone y de la caisse 4
                        		wiiZone2 = 4;
                    			//}
                		} else if (wii2->pos[0] < 500) {
                		    // dans la zone x de la caisse 3
                		    //if (wii2->pos[1] > (800 - taillecaisse3pieds) && wii2->pos[1] < (800 - taillepieds3)) {
                       			 // dans la zone y de la caisse 4
                       			 wiiZone2 = 3;
                    			//}
                		} else if (wii2->pos[0] < 750) {
                    			// dans la zone x de la caisse 2
                   			 //if (wii2->pos[1] > (800 - taillecaisse2pieds) && wii2->pos[1] < (800 - taillepieds2)) {
                  		      // dans la zone y de la caisse 4
                  		      wiiZone2 = 2;
                    		//}
              			} else if (wii2->pos[0] < 1000) {
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

void close(Wii *wii) {
    wii->close();
    cout << "> device closed." << endl;
}
