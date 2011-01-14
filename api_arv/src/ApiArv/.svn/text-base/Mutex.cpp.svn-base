/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Mutex.h"
#include <iostream>
using namespace std;

namespace arv {

Mutex* Mutex::create() {
    pthread_mutex_t handle;
    if (pthread_mutex_init(&handle, NULL) == 0)
        return new Mutex(handle);
    cerr << "- error in Mutex::create() : pthread_mutex_init returned an error." << endl;
    return NULL;
    }

Mutex::~Mutex() {
    pthread_mutex_destroy(&handle);     // returns 0 iff successful
    }

bool Mutex::lock() {
    return pthread_mutex_lock(&handle) == 0;
    }

bool Mutex::unlock() {
    return pthread_mutex_unlock(&handle) == 0;
    }

bool Mutex::trylock() {
    return pthread_mutex_trylock(&handle) == 0;
    }

}
