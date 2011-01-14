/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Sound3D.h>
#include <iostream>
#include <QGLViewer/vec.h>
#include <time.h>
#include <math.h>

using namespace arv;
using namespace std;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

using qglviewer::Vec;

int main(int /*argc*/, char* /*argv*/[]) {
    Sound3D *sound = Sound3D::create();
    if (sound == NULL) ERROR
    cout << "> device initialized successfully." << endl;
    
    sound->setListener(Vec(0,0,0), Vec(0,0,0), Vec(0,0,0-1), Vec(0,1,0));
    
    Vec pos(0,0,0);
    Sound3D::Source* src = sound->addSource("samples/bass.wav", pos);
    if (src == NULL) ERROR
    cout << "> sound source added." << endl;
    
    src->setLoopMode(true);
    src->play();
    
    const int nTurns = 3;           // number of turns around listener
    const float turnTime = 2.0f;    // time for one turn in seconds
    const int N = 200;              // sampling of the trajectory for one turn
    const float R = 2.0f;           // radius of the turns
    
    const struct timespec dt = {
        (time_t)floorf(turnTime / N),
        (long)(1000000000 * fmodf(turnTime / N, 1.0f))
        };
    
    for (int i=0; i<nTurns; i++) {
        for (int k=0; k<N; k++) {
            const float a = 2 * M_PI * float(k) / float(N);
            pos.x = R * cosf(a);
            pos.z = R * sinf(a);
            Vec v = 0.01 * Vec(-sinf(a), cosf(a), 0);
            src->setState(pos, v);
            nanosleep(&dt, NULL);
            //sleep(1);
            }
        }

    src->stop();

    delete sound;
    cout << "> device destroyed." << endl;

    return 0;
    }
