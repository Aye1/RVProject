/******************************************
 *  creation :
 *      2010 : Thibault Serot
 */

#include <ApiArv/Wii.h>
#include <iostream>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>


using namespace arv;
using namespace std;

bool stop;

void sigproc(int /*i*/){
    stop = true;
}

int main(int /*argc*/, char* /*argv*/[]) {
    signal(SIGINT, sigproc);
    signal(SIGQUIT, sigproc);

    stop = false;
    cout << "Test for the Wii inteface." << endl;
    Wii * wii = new Wii();
    cout << "> Wii allocated." << endl;

    cout << "> Adding a wiimote" << endl;
    wii->addWiimote();
    wii->enableNunchuk(0);
    wii->enableMotionPlus(0);
    cwiid_ir_src* coucou;

//    cout << "> Adding a second wiimote" << endl;
//    wii->addWiimote();
 
  //int i = 0;
    bool rumble = false;
    //int led[4] = {CWIID_LED1_ON,CWIID_LED2_ON,CWIID_LED3_ON,CWIID_LED4_ON};
    while(!stop) {
        if (wii->update()){
         //   double ax,ay,az;
//            double nax,nay,naz;
       //     wii->getAcceleration(ax,ay,az,0);
//            wii->getAccelerationNunchuk(nax,nay,naz,0);
     //       std::cout << "Acc : " << ax << " , " << ay << " , " << az << std::endl;
	// " , Nunchuk : " << nax << " , " << nay << " , " << naz << std::endl;
            coucou = wii->getIRData();
            if (coucou->size == 0) {
                std::cout << 0 << " " << coucou->valid << std::endl;
            } else if (coucou->size == 1) {
                std::cout << 1 << " " << coucou->valid << std::endl;
            } else if (coucou->size == 2) {
                std::cout << 2 << " " << coucou->valid << std::endl;
            } else if (coucou->size == 3) {
                std::cout << 3 << " " << coucou->valid << std::endl;
            } else if (coucou->size == 4) {
                std::cout << 4 << " " << coucou->valid << std::endl;
            } else {
                std::cout << "autre" << std::endl;
            }
//            std::cout << "0  pos0 " << coucou->pos[0] << " pos1 " << coucou->pos[1] << " valid " << coucou->valid << " size " << coucou->size << s/std::endl;
//            coucou = wii->getIRData(1);
 //           std::cout << "1  pos0 " << coucou->pos[0] << " pos1 " << coucou->pos[1] << " valid " << coucou->valid << " size " << coucou->size << std::endl;
            int buttons = wii->getButtonPressed(0);
            if (buttons & CWIID_BTN_A) {
                if (!rumble) wii->startRumble(0);
                //else wii->stopRumble(0);
                rumble = !rumble;
            }else if (buttons & CWIID_BTN_B){
                if (rumble) wii->stopRumble(0);
                rumble = !rumble;
            }
            //int buttonsN = wii->getButtonPressedNunchuk(0);
            ////std::cout << "Nunchuk button : " << buttonsN << std::endl;
            //if (buttonsN & CWIID_NUNCHUK_BTN_Z) {
            //    if (!rumble) wii->startRumble(0);
            //    //else wii->stopRumble(0);
            //    rumble = !rumble;
            //}else if (buttonsN & CWIID_NUNCHUK_BTN_C) {
            //    if (rumble) wii->stopRumble(0);
            //    rumble = !rumble;
            //}
            //double x,y;

            //wii->getJoystickNunchuk(x,y,0);
            //std::cout << "x = " << x << " y = " << y << std::endl;

            //std::cout << "Button : " << wii->getButtonPressed(0) << std::endl;
        }
        //sleep(1);
        //if (i == 4) i = 0;
        //wii->enableLeds(led[i],0);
        //i++;
    }


    wii->close();
    cout << "> device closed." << endl;

    return 0;
}
