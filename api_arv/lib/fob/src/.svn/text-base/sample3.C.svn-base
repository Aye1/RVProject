
// test the open flock of bird, run driver, and get one value.
// test in alone mode with one fob.


#include <math.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream.h>
#include <Performer/pf.h>
#include "FobAlonePerf.h"

int
main (int argc, char *argv[],char *envp[])
{
   
    pfVec3 pos;
    pfQuat quat;
    int oneFob=0;

    FobAlonePerf *fob=new FobAlonePerf(1);//1 fob in the flock
    
    //configure hardware
    fob->Set(FobAlonePerf::SERIAL_SOFT,3,0);//serial wire=soft, serial port=3 fob 1
    fob->Init();  //init hardware the flock  
    //configure software
    fob->SetPen(0);
    fob->Start();//run the process
    
    sleep(1);// just wait the begin of the stream output for the bird

    fob->GetPosQuat(pos,quat,oneFob);
    cerr<<"Get one value.."<<endl;
    cerr<<"pos Flock"<<oneFob<<" ("<<pos[0]<<","<<pos[1]<<","<<pos[2]<<")"<<endl;
    cerr<<"quat Flock"<<oneFob<<" ("<<quat[0]<<","<<quat[1]<<","<<quat[2]<<","<<quat[3]<<")"<<endl; 
    
    fob->Stop(); //stop the process
    fob->Close();
    return 0;
}
