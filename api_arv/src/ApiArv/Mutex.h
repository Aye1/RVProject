/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_MUTEX__
#define __ARV_MUTEX__

#include <pthread.h>

namespace arv {

    /** This class offers a mutual exclusion (mutex) synchronisation mechanism.
     * To create an instance of \c Mutex, the static method \c create() must be called. This method returns \c NULL if an error occured during the creation of the object.
     * Here is an example of how to use it:
     * @code
     *  Mutex* mutex = Mutex::create();
     *  if (mutex == NULL) error();
     *  ...
     *  void someComputationsOnSharedRessources() {
     *      mutex->lock();
     *      ...
     *      // here the mutex is locked by the current thread
     *      ...
     *      mutex->unlock();
     *      }
     *  ...
     *  delete mutex;
     * @endcode
     *
     */
    class Mutex {
        
        public:
            static Mutex* create();
                /**<
                 * Creates a new mutex.
                 @return the newly created instance or \c NULL if an error occured.
                 */
                 
            ~Mutex();
                /**< Frees associated ressources. */
            
            bool lock();
                /**<
                 * Locks the mutex.
                 @return \c false if an error occured.
                 */
            
            bool unlock();
                /**<
                 * Unlocks the mutex.
                 @return \c false if an error occured.
                 */
            
            bool trylock();
                /**<
                 * Tries to lock the mutex. 
                 * As opposed to \c lock(), this method doesn't wait for the mutex to unlock.
                 @return \c true iff the mutex managed to lock itself.
                 */
            
        private:
            Mutex(pthread_mutex_t handle) : handle(handle) {}
            pthread_mutex_t handle;
        };
    
    }

#endif

