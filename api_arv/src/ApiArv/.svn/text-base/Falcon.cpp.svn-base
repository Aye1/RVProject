#include "Falcon.h"

namespace arv {

    Falcon::Falcon() {
        isInit = false;
    }

    bool Falcon::init(){
        if (isInit) {
            std::cout << "Falcons already initialized" << std::endl;
            return true;
        }
        dev.push_back(FalconDevice());
        dev[0].setFalconFirmware<FalconFirmwareNovintSDK>();
        if (! dev[0].getDeviceCount(num_falcons)) {
            std::cout << "Cannot get device count" << std::endl;
            return false;
        }

        //int count = 0;
        
        if (num_falcons == 0) {
            std::cout << "No falcon found" << std::endl;
            return false;
        }
        
        std::cout << num_falcons << " Falcons found" << std::endl;

        std::cout << "Opening falcons" << std::endl;
        dev.clear();
        for (unsigned int i = 0; i < num_falcons; i++) {
            dev.push_back(FalconDevice());
            dev[i].setFalconFirmware<FalconFirmwareNovintSDK>();
            std::cout << "Opening falcon " << i << std::endl;
            if(!dev[i].open(i)){
                std::cout << "Cannot open falcon - Error: " << dev[i].getErrorCode() << " Device Error Code: " << dev[i].getFalconComm()->getDeviceErrorCode() << std::endl;
                return false;
            }
            std::cout << "...Falcon opened" << std::endl;
        }

        for (unsigned int i = 0; i < num_falcons; ++i) {
            if(!dev[i].isFirmwareLoaded()){
			    std::cout << "Loading firmware" << std::endl;
			    for(int z = 0; z < MAX_RETRY; ++z){
				    if(!dev[i].getFalconFirmware()->loadFirmware(true, NOVINT_FALCON_NVENT_FIRMWARE_SIZE, const_cast<uint8_t*>(NOVINT_FALCON_NVENT_FIRMWARE))){
                        std::cout << "Firmware loading failed...";
				    }else{
					    std::cout <<"Firmware loaded" << std::endl;
					    break;
				    }
			    }
			    if(!dev[i].isFirmwareLoaded()){
				    std::cout << "Firmware didn't load correctly. Try running the program again" << std::endl;
				    return false;
			    }
            }
        }

        // Enable Kinematic et Grip mode
        for (unsigned int i = 0; i < num_falcons; ++i) {
        	dev[i].setFalconKinematic<FalconKinematicStamper>();
	        dev[i].setFalconGrip<FalconGripFourButton>();
        }

        // Seems to be important to run io loop once to be sure of sensible values next time
        for (unsigned int i = 0; i < num_falcons; ++i) {
            //dev[i].runIOLoop();
            //dev[i].getFalconFirmware()->setHomingMode(true);
            displayCalibMsg = true;
            while (!calibrateDevice(i)){ }
        }

        pos.clear();
        pos.resize(num_falcons);
        buttons.clear();
        buttons.resize(num_falcons);

        isInit = true;
        return true;

    }
    
    bool Falcon::update() {
        for (unsigned int i = 0; i < num_falcons; i++){
            if (!update(i)) return false;
        }
        return true;
    }

    bool Falcon::update(unsigned int id) {
        if (!isInit || id >= num_falcons) return false;

        if (!dev[id].runIOLoop()) {
            //std::cout << "Falcon::update : runIOLoop failed " << dev[id].getErrorCode() << std::endl;
            return false;
        }

        pos[id] = dev[id].getPosition();
        //std::cout << "update position : " << pos[id][0] << " , " << pos[id][1] << " , " << pos[id][2] << std::endl;
        buttons[id] = dev[id].getFalconGrip()->getDigitalInputs();

        return true;

    }

    boost::array<double, 3> Falcon::getPosition(unsigned int id) const {
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::getPosition : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return boost::array<double, 3>();
        }
        return pos[id];
    }

    void Falcon::setForce(boost::array<double, 3> force,unsigned int id){
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::setForce : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return;
        }
        dev[id].setForce(force);
        
    }

    bool Falcon::isButton1Pressed(unsigned int id) {
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::isButton1Pressed : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return false;
        }
        return (buttons[id] & FalconGripFourButton::BUTTON_1);
    }

    bool Falcon::isButton2Pressed(unsigned int id) {
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::isButton2Pressed : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return false;
        }
        return (buttons[id] & FalconGripFourButton::BUTTON_2);
    }

    bool Falcon::isButton3Pressed(unsigned int id) {
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::isButton3Pressed : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return false;
        }
        return (buttons[id] & FalconGripFourButton::BUTTON_3);
    }

    bool Falcon::isButton4Pressed(unsigned int id) {
        if (!isInit || id >= num_falcons) {
            std::cout << "Falcon::isButton4Pressed : Invalid index (" << id << " >= " << num_falcons << std::endl;
            return false;
        }
        return (buttons[id] & FalconGripFourButton::BUTTON_4);
    }


    

    void Falcon::close() {
        if (!isInit) return;
        //TODO : Why close() block the program
        //for (unsigned int i = 0; i < num_falcons; ++i){
        //    dev[i].close();
        //}
        isInit = false;
    }

    unsigned int Falcon::size() const {
        return num_falcons;
    }


    bool Falcon::calibrateDevice(unsigned int id) {
        if (id >= num_falcons) return false;
        bool homing = false;
        dev[id].getFalconFirmware()->setHomingMode(true);
        dev[id].runIOLoop();
        if (!dev[id].getFalconFirmware()->isHomed()) {
            if (!homing){
                dev[id].getFalconFirmware()->setLEDStatus(FalconFirmware::RED_LED);
                if (displayCalibMsg){
                    std::cout << "Falcon not currently calibrated. Move control all the way out then push straight all the way in." << std::endl;
					displayCalibMsg = false;
                }
            }
            homing = true;
            return false;
        }
        std::cout << "Falcon calibrated successfully" << std::endl;
        dev[id].getFalconFirmware()->setLEDStatus(FalconFirmware::GREEN_LED);
        homing = false;
        return true;
    }

}

