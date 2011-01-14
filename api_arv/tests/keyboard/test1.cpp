/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Keyboard.h>
#include <iostream>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int /*argc*/, char* /*argv*/[]) {
    cout << "Test for keyboard inteface." << endl;

    Keyboard *keyboard = new Keyboard();
    if (!keyboard->init()) ERROR
    cout << "> keyboard device initialized." << endl;
    
    Keyboard::Code key = keyboard->keyCode("space");
    
    keyboard->grab();
    cout << "> keyboard grabbed." << endl;
    
    for (int i=0; i<100000; i++) {
        keyboard->update();
        
        const char* keyName = keyboard->keyString(key);
        
        if (keyboard->stateChanged(key))
            cout << "***********************************" << endl << keyName << " key state changed" << endl;
        
        if (keyboard->justPressed(key))
            cout << keyName << " key just pressed" << endl;
        if (keyboard->justReleased(key))
            cout << keyName << " key just released" << endl;
        }
    keyboard->ungrab();
    cout << "> keyboard ungrabbed." << endl;

    keyboard->close();
    cout << "> keyboard device closed." << endl;

    return 0;
    }
