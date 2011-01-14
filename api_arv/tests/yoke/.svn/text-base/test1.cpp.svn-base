/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Yoke.h>
#include <iostream>

using namespace arv;
using namespace std;

int main(int /*argc*/, char* /*argv*/[]) {
    cerr << "Test of the yoke inteface." << endl;

    Yoke *yoke = Yoke::create(0);
    if (yoke == NULL) return 1;
    cout << "> yoke initialized." << endl;
    
    const int na = yoke->numAxes();
    const int nb = 7;   // the number of buttons
    
    //for (int i=0; i<5000; i++) {
    while (true) {
        yoke->update();
        cout << "axes :";   for (int k=0; k<na; k++) cout << " " <<  yoke->axisValue(k);
        cout << "button :"; for (int k=0; k<nb; k++) cout << " " << (yoke->buttonPressed(k) ? 1 : 0);
        cout << endl;
        }
    cout << "> yoke input terminated." << endl;

    delete yoke;
    cout << "> yoke deleted." << endl;

    return 0;
    }
