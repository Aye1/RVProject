#ifndef __FALCON_H__
#define __FALCON_H__

//#include <falcon/core/FalconLogger.h>
#include <falcon/core/FalconDevice.h>
#include <falcon/firmware/FalconFirmwareNovintSDK.h>
#include <falcon/util/FalconFirmwareBinaryNvent.h>
#include "falcon/grip/FalconGripFourButton.h"
#include "falcon/kinematic/FalconKinematicStamper.h"

#include <vector>

#define MAX_RETRY 10

using namespace libnifalcon;
namespace arv {


//	enum{
//			GREEN_LED=0x2,		/**< Flag to control Green LED */
//			BLUE_LED=0x4,		/**< Flag to control Blue LED */
//			RED_LED=0x8			/**< Flag to control Red LED  */
//	} FalconLEDValues;



    class Falcon {
        public:
            Falcon();

            bool init();
            bool update();
            bool update(unsigned int id);
            void close();
            boost::array<double, 3> getPosition(unsigned int id) const;
            void setForce(boost::array<double, 3> force,unsigned int id = 0);
            bool isButton1Pressed(unsigned int id = 0);
            bool isButton2Pressed(unsigned int id = 0);
            bool isButton3Pressed(unsigned int id = 0);
            bool isButton4Pressed(unsigned int id = 0);
            unsigned int size() const;


        private:

            bool calibrateDevice(unsigned int id);

            //boost::shared_ptr<FalconFirmware> f;
            std::vector<FalconDevice> dev;
            std::vector<unsigned int> buttons;
            bool displayCalibMsg;

            unsigned int num_falcons;
            bool isInit;
            std::vector<boost::array<double, 3> > pos;
            //std::vector<uint8_t> LEDGreen;
            //std::vector<uint8_t> LEDBlue;
            //std::vector<uint8_t> LEDRed;










    };

}




#endif
