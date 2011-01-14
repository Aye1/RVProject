/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Glove.h>
#include <QString>
#include <iostream>
#include <math.h>

using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }
#define PADDED_INT(value) QString("%1").arg(value, 4, 10, QLatin1Char(' ')).toLatin1().constData()

int main(int argc, char* argv[]) {
    cout << "Test for glove inteface." << endl;
    Glove *glove = Glove::create();
    if (glove == NULL) ERROR
    cout << "> glove device initialized successfully." << endl;

    Glove::State statePrev;
    Glove::State state;

    glove->update(statePrev);
    
    for (int i = 0;i < 2000;i++) {
        glove->update(state);

        cout << "active joints : ";
        for (int j=0; j<18; j++) {
            const float ratio = float(statePrev(j) / float(state(j)));
            if (fabsf(1.0f - ratio) > 0.05f)
                cout << PADDED_INT(j);
            else
                cout << "    ";
            }
        cout << endl;

        statePrev.set(state);
        
        cout << "joints values : ";
        for (int j=0; j<18; j++) cout << PADDED_INT(state(j));
        cout << endl;
        }

    cout << "- stopping device ..." << endl;
    delete glove;
    cout << "> glove device closed." << endl;
    
    return 0;
    }
