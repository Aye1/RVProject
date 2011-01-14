/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Thread.h>
#include <ApiArv/Mutex.h>
#include <iostream>

using namespace std;
using namespace arv;

class UnsafeCounter : public Runnable {
    public:
        UnsafeCounter(long inc) : inc(inc), val(0L) {}
        
        virtual void run() {
            for (long i=0; i<inc; i++)  val ++;
            }
        
    public:
        const long inc;
        long val;
    };

class SafeCounter : public Runnable {
    public:
        SafeCounter(long inc) : inc(inc), val(0L), mutex(Mutex::create()) {}
        ~SafeCounter() { delete mutex; }
        
        virtual void run() {
            mutex->lock();
            for (long i=0; i<inc; i++) val ++;
            mutex->unlock();
            }
        
    public:
        const long inc;
        long val;
        Mutex *mutex;
    };

int main(int /*argc*/, char */*argv*/[]) {

    const long N = 100L;        // number of threads per counting process
    const long K = 10000000L;   // increment par counting process

    cout << "- theoric result : " << (long) (N * K) << endl;
    cout << "- start counting threads ..." << endl;
    
    UnsafeCounter unsafeProc(K);
    SafeCounter   safeProc(K);

    Thread *tabThreadSafe[N];
    Thread *tabThreadNotSafe[N];

    for (int i=0; i<N; i++) {
        tabThreadNotSafe[i] = Thread::create(&unsafeProc);
        tabThreadSafe[i]    = Thread::create(&safeProc);
        }

    for (int i=0; i<N; i++) {
        tabThreadNotSafe[i]->join();
        tabThreadSafe[i]->join();
        delete tabThreadNotSafe[i];
        delete tabThreadSafe[i];
        }

    cout << "- unsafe result : " << unsafeProc.val << endl;
    cout << "- safe result : "   << safeProc.val   << endl;
    }

