/******************************************
 *  creation :
 *      2004 : Raphael Grasset
 *  modification :
 *      2008 : Lionel Baboud
 */

#include <ApiArv/Microphone.h>
#include <iostream>

using namespace std;
using namespace arv;

#define ERROR { cerr << "- an error happened, ending test." << endl; return 1; }

int main(int /*argc[]*/, char* /*argv[]*/) {
	Microphone *micro = new Microphone();
	cout << "> microphone device allocated."<< endl;

	if (!micro->init()) ERROR
	cout << "> microphone device initialized."<< endl;

	micro->alloc(5.0f);
	cout << "> 5 seconds allocated."<< endl;

	micro->start();
	cout << "> recording started ..."<< endl;

	sleep(5);
    
	micro->stop();
	cout << "> recording finished."<< endl;

	if (!micro->save("record.wav")) ERROR
	cout << "> record saved." << endl;
    
	micro->close();
    delete micro;
	cout << "> recording device closed." << endl;

	return 0;
    }

