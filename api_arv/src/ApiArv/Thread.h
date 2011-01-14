/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#ifndef __ARV_THREAD__
#define __ARV_THREAD__

#include <pthread.h>

namespace arv {

    /** This interface represents a runnable object.
     * The only requirement for a \c Runnable object is to have a \c run() method.
     * This is useful to define a process that needs to be run in a \c Thread.
     */
    class Runnable {
        public:
            virtual ~Runnable() {}
            
            virtual void run() = 0;
                /**< This method is called when this object needs to be run. */
        };
    
    /*
    /// This class represents a generic function as a runnable object.
    class RunnableFunction : public Runnable {
        public:
            RunnableFunction(void *(*function)(void *), void* arg = NULL) : function(function), arg(arg) {}
            virtual void run() { (*function)(arg); }
    
        private:
            void *(*function)(void *);
            void *arg;
        };
    */
    
    /** This class allows to execute multiple threads.
     * A thread is created and executed when one of the \c create() static methods is called.
     * A simple way to run multiple processes into different threads is to extend the class \c Runnable:
     * \code
     *  class MyProcess1 : public Runnable {
     *      public:
     *          void run() {
     *              ... // process 1
     *              }
     *      };
     *
     *  class MyProcess2 : public Runnable {
     *      public:
     *          void run() {
     *              ... // process 2
     *              }
     *      };
     *
     *  Runnable *proc1 = new MyProcess1();
     *  Runnable *proc2 = new MyProcess2();
     *  Thread *thread1 = new Thread(proc1);
     *  Thread *thread2 = new Thread(proc2);
     *  if (thread1 == NULL || thread2 == NULL) error();
     *  // the two processes are run
     *  thread1.join();
     *  thread2.join();
     *  // at this point the two processes are terminated.
     * \endcode
     */
    class Thread {
        public:
            static Thread* create(Runnable *runnable);
                /**< Creates and starts a new thread from a \c Runnable object.
                 * Once the thread is created, the \c run() method of \p runnable is called.
                 @param runnable the object to run on this thread.
                 @return The created thread or NULL if an error occured.
                 */
                
            static Thread* create(void* (*run)(void*), void* arg = NULL);
                /**< Creates and starts a new thread.
                 @param run the function to run.
                 @param arg the arguments for the function \p run.
                 @return The created thread or NULL if an error occured.
                 */
            
            bool join() const;
                /**< Waits for this thread to end.
                 * The calling thread will be locked at that point until this thread ends.
                 @return True iff the thread joined correctly.
                 */

            bool kill() const;
                /**< Kills this thread.
                 @return True iff the thread was killed correctly.
                 */
            
            static void exit();
                /**< Exits the thread from which this function was called. */

            //void suspend();           /**< Suspend execution of the thread */
            //void resume();            /**< Resume the execution after a suspend() call*/

        private:
            Thread(pthread_t handle) : handle(handle) {}
            
        private:
            pthread_t handle;
        };
    
    }

#endif

