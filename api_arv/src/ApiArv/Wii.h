#ifndef __WII_H__
#define __WII_H__

#include <bluetooth/bluetooth.h>
#include <cwiid.h>
#include <vector>
#include <iostream>
#include <math.h>

#define WEIGHT_COEFF 1700

namespace arv {
  
    void convertaxyz2arollpitch(double const & ax, double const & ay, double const & az, double & a, double & roll, double & pitch);

    class Wiimote {
        public:
            Wiimote(bool wiimote = true);
            bool isInit() { return init;};
            bool isAWiimote() { return isWiimote;};
            
            void close();

            bool update();

            bool enableIR();
            bool disableIR();

            bool enableMotionPlus();
            bool disableMotionPlus();

            bool enableNunchuk();
            bool disableNunchuk();

            void startRumble();
            void stopRumble();

            void enableLeds(int led);

            int getButtonPressed();
            int getButtonPressedNunchuk();

            void getJoystickNunchuk(double & x, double & y);

            void getAcceleration(double & ax, double & ay, double & az);
            void getAccelerationNunchuk(double & ax, double & ay, double & az);
            cwiid_ir_src * getIRData();

            void getSensorBalance(double & right_top, double & right_bottom, double & left_top, double & left_bottom);
            double computeWeight();

            double getBatteryStatus();

        private:
            //void wiimote_callback(cwiid_wiimote_t * wiimote, int msg_count, union cwiid_mesg msgs[], struct timespec * timestamp);

            cwiid_wiimote_t * wiimote;
            struct cwiid_state state;
            bdaddr_t bdaddr;
            bool init;
            int flag;
            char foundaddr[20];
            bool isWiimote;
            
            bool acc_init;
            bool acc_nunchuk_init;
            bool balance_cal_init;
            struct acc_cal acc_calibration;
            struct acc_cal acc_calibration_nunchuk;
            struct balance_cal balance_cal;

            //void (*cwiid_btn)(struct cwiid_btn_mesg*);
            //void (*cwiid_acc)(struct cwiid_acc_mesg*);
            //void (*cwiid_ir)(struct cwiid_ir_mesg *);
            //void (*cwiid_nunchuk)(struct cwiid_nunchuk_mesg *);
            //void (*cwiid_classic)(struct cwiid_classic_mesg *);
            //void (*cwiid_motionplus)(struct cwiid_motionplus_mesg*);

    };



    class Wii {
        public:
        
            Wii();
            int addWiimote(/*bdaddr_t bdaddr = *BDADDR_ANY*/);
            int addWiibalance();

            bool update();
            bool update(unsigned int id);

            bool enableIR();
            bool enableIR(unsigned int id);

            bool disableIR();
            bool disableIR(unsigned int id);

            bool enableMotionPlus();
            bool enableMotionPlus(unsigned int id);

            bool disableMotionPlus();
            bool disableMotionPlus(unsigned int id);

            bool enableNunchuk();
            bool enableNunchuk(unsigned int id);

            bool disableNunchuk();
            bool disableNunchuk(unsigned int id);

            void startRumble();
            void startRumble(unsigned int id);

            void stopRumble();
            void stopRumble(unsigned int id);

            void enableLeds(int led);
            void enableLeds(int led,unsigned int id);

            int getButtonPressed(unsigned int id = 0);
            int getButtonPressedNunchuk(unsigned int id = 0);

            void getJoystickNunchuk(double & x, double & y, unsigned int id = 0);

            void getAcceleration(double & ax, double & ay, double & az, unsigned int id = 0);

            void getAccelerationNunchuk(double & ax, double & ay, double & az, unsigned int id = 0);

            cwiid_ir_src * getIRData(unsigned int id = 0);

            void getSensorBalance(double & right_top, double & right_bottom, double & left_top, double & left_bottom, unsigned int id = 0);
            double computeWeight(unsigned int id = 0);

            void close();
        
            bool isAWiimote(unsigned int id);

            double getBatteryStatus(unsigned int id);

        private:
            std::vector<Wiimote *> wiimotes;


    };



}

#endif
