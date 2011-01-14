
// test the open flock of bird, run driver, and get one value.
// test in flock mode with one fob.


#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <stdio.h>

#include "FobAlone.h"

int main (int argc, char *argv[],char *envp[])
{
    float pos[3];
    float quat[4];
    float euler[3];

    // FobAlone *fob=new FobAlone(POSITION_ANGLES,FORWARD,"/dev/ttyS1",CONTROL_SOFT,B9600);
    // FobAlone *fob=new FobAlone(POSITION_QUATERNION,FORWARD,"/dev/ttyS1",CONTROL_SOFT,B9600);
    FobAlone *fob=new FobAlone(POSITION_QUATERNION,FORWARD,"/dev/ttyS1",CONTROL_HARD,B38400);

    fob->InitAlone();
    std::cerr<<"init..OK" << std::endl;

    // fob->SetFilter(FILTER_ON,FILTER_OFF,FILTER_ON);
    std::cerr << "filter..OK" << std::endl;
    //fob->RunDriver();

    // sleep(1);// just wait the begin of the stream output for the bird

    while (1)
    {

        std::cerr << "Get one value.." << std::endl;

        //  fob->GetSPosQuaternion(pos,quat);
        //	 cerr<<"pos Flock ("<<pos[0]<<","<<pos[1]<<","<<pos[2]<<")"<<endl;

        //  cerr<<"quat Flock"<<quat[0]<<","<<quat[1]<<","<<quat[2]<<","<<quat[3]<<endl;

        fob->GetPosQuaternion(pos,quat);
        std::cerr << "pos Flock (" << pos[0] << "," << pos[1] << "," << pos[2] << ")" << std::endl;

        std::cerr << "quat Flock" << quat[0] << "," << quat[1] << "," << quat[2] << "," << quat[3] << std::endl;
        // fob->GetPosAngle(pos,euler);

        //   cerr<<"pos Flock ("<<pos[0]<<","<<pos[1]<<","<<pos[2]<<")"<<endl;
        //	cerr<<"euler Flock ("<<euler[0]<<","<<euler[1]<<","<<euler[2]<<")"<<endl;
    }

    // fob->StopDriver(); 

    return 0;

}
