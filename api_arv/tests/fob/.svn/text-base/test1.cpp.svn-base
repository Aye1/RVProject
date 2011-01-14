/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Fob.h>
#include <iostream>

using namespace arv;
using namespace std;
using namespace qglviewer;

int main(int /*argc*/, char* /*argv*/[]) {
    cout << "Test for the FOB inteface." << endl;
    Fob *fob = new Fob(Fob::HEMI_UPPER);
    cout << "> FOB allocated." << endl;
    fob->init();
    cout << "> FOB initialized." << endl;

    for (int i = 0;i < 100;i++) {
        cout << "**************************************************" << endl;
        fob->update();
        cout << "> pos = "          << fob->position()
             << "  orientation = "  << fob->orientation() << endl;
        }

    fob->close();
    cout << "> device closed." << endl;

    return 0;
    }
