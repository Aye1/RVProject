/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Sound.h>
#include <iostream>
#include <math.h>

using namespace std;
using namespace arv;

#define ERROR { cout << "- error during test : stop." << endl; return 1; }

int main(int /*argc*/, char* /*argv*/[]) {
    Sound *sound = new Sound();
    cout << "allocate sound..OK" << endl;

    if (!sound->init()) ERROR
    cout << "init sound..OK" << endl;

    Sound::Sample *sample1 = sound->add("./samples/jungle.wav");
    if (sample1 == NULL) ERROR
    cout << "> sample 1 loaded : " << sample1->duration() << " s." << endl;
    
    cout << "- play sample ..." << endl;
    sample1->play();
    sleep((int)ceilf(sample1->duration()));
    cout << "> done." << endl;
    
    Sound::Sample *sample2 = sound->add("./samples/drumloop.wav");
    sample2->setLoopMode(true);
    cout << "> sample 2 loaded : " << sample2->duration() << " s." << endl;

    sample1->setLoopMode(true);

    cout << "- preparing sounds ..." << endl;
    sample1->prepare();
    sample2->prepare();
    sleep(1);
    cout << "> sounds ready, go..." << endl;
    sound->resume();
        
    sleep(4);

    cout << "- pause ..." << endl;
    sound->pause();
    sleep(2);
    sound->resume();
    cout << "> resumed." << endl;
    sleep(4);
    
    sample1->stop();
    cout << "> sample 1 stopped." << endl;

    sleep(2);

    sound->stop();
    cout << "> sound stopped." << endl;

    sound->close();
    cout << "close sound..OK" << endl;

    return 0;
    }
