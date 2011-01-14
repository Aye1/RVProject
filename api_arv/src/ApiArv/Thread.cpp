/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include "Thread.h"
#include <signal.h>
#include <fstream>

namespace arv {

/*********************************************************************************************/

// execution de la méthode 'run' d'un objet <Runnable> :
void* start_runnable(void *runnable) {
    Runnable *object = static_cast<Runnable*>(runnable);
    object->run();
    return NULL;
    }

/*********************************************************************************************/

Thread* Thread::create(Runnable *runnable) {
    return create(start_runnable, (void*)runnable);
    }
        
Thread* Thread::create(void* (*run)(void*), void* arg) {
    pthread_t handle;
    if (pthread_create(&handle, NULL, run, arg) != 0) {
        fprintf(stderr, "ERROR->ARVTHREAD/CREATE : we can't create a new thread");
        return NULL;
        }
    return new Thread(handle);
    }

void Thread::exit() {
    pthread_exit(NULL);
    }

/*
void Thread::suspend() {
    pthread_kill(handle, SIGSTOP);      // SIGABRT, SIGKILL
    // doesn't seem to work..
    }

void Thread::resume() {
    pthread_kill(handle, SIGCONT);
    }
*/

bool Thread::join() const {
    return pthread_join(handle, NULL) == 0;
    }

bool Thread::kill() const {
    return pthread_cancel(handle) == 0;
    }

}

