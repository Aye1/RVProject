/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Keyboard.h>
#include <iostream>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int /*argc[]*/, char* /*argv[]*/) {
    cout << "Test keyboard interface." << endl;

    Keyboard *keyboard = new Keyboard();
    if (!keyboard->init()) ERROR
    cout << "> keyboard device initialized." << endl;
    
    keyboard->grab();
    for (int i=0; i<100000; i++) {
        keyboard->update();
        vector<Keyboard::Code> keys = keyboard->activeKeys();
        for (unsigned int i=0; i<keys.size(); i++)
            cout << keyboard->keyString(keys[i]) << " ";
        if (!keys.empty()) cout << endl;
        }
    keyboard->ungrab();
    keyboard->close();
    cout << "> keyboard device closed." << endl;

    return 0;
    }
