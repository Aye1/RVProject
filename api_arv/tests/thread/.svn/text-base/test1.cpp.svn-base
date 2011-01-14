/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2005 : Yohan Piccoli
 *      2008 : Lionel Baboud
 */
 
#include <ApiArv/Thread.h>
#include <iostream>

using namespace std;
using namespace arv;

class CountProcess : public Runnable  {
    public:
        CountProcess(int id, int time, int limit=-1) : id(id), time(time), limit(limit) {}
        
        virtual void run() {
            cout << "- process " << id << " started ..." << endl;
            for(int i=0; i<time; i++) {
                cout << "  process " << id << " : remaining " << (time-i) << endl;
                sleep(1);
                if (limit >= 0 && i >= limit) {
                    cout << "> process " << id << " : limit exceeded : force exit." << endl;
                    Thread::exit();
                    cout << "> error : these statements should not be reacheable." << endl;
                    }
                }
            cout << "> process " << id << " terminated by itself." << endl;
            }
        
    private:
        int id, time, limit;
    };

void* testRoutine(void *param) {
    int val = *((int *) param);
    cout << "- test routine " << val << "." << endl;
    sleep(1);
    return NULL;
    }

int main(int /*argc*/, char */*argv*/[]) {
    int val = 666;
    Thread *thread0 = Thread::create(testRoutine, (void*) (&val));
    cout << "> thread 0 created." << endl;
    if (thread0->join())
        cout << "> thread 0 finished properly." << endl;
    else
        cout << "> thread 0 did not finish properly." << endl;
    delete thread0;
    
    CountProcess proc1(1, 3);
    CountProcess proc2(2, 6);
    CountProcess proc3(3, 15);
    CountProcess proc4(4, 50, 4);
    
    Thread *thread1 = Thread::create(&proc1);
    Thread *thread2 = Thread::create(&proc2);
    Thread *thread3 = Thread::create(&proc3);
    Thread *thread4 = Thread::create(&proc4);

    // wait for the end of thread1 :
    if (thread1->join())
        cout << "> thread 1 finished properly." << endl;
    else
        cout << "> thread 1 did not finish properly." << endl;

    // wait for the end of thread2 :
    if (thread2->join())
        cout << "> thread 2 finished properly." << endl;
    else
        cout << "> thread 2 did not finish properly." << endl;
    
    // kill thread4 :
    if (thread4->kill())
        cout << "> thread 4 has been killed." << endl;
    else
        cout << "> thread 4 ended before being killed." << endl;

    // wait for the end of thread3 :
    if (thread3->join())
        cout << "> thread 3 finished properly." << endl;
    else
        cout << "> thread 3 did not finish properly." << endl;

    delete thread1;
    delete thread2;
    delete thread3;
    delete thread4;
    
    printf("> end of program.\n");
    }

