#include "Wii.h"
using namespace arv;


void arv::convertaxyz2arollpitch(double const & ax, double const & ay, double const & az, double & a, double & roll, double & pitch){
    a = sqrt(ax * ax + ay * ay + az * az);
    roll = atan(ax / az);
    if (az <= 0.0){
        roll += M_PI * ( (ax > 0.0)? 1: -1);
    }
    roll *= -1.0;
    pitch = atan(ay / az * cos(roll));
}

Wiimote::Wiimote(bool _isWiimote) {
    init = false;
    acc_init = false;
    acc_nunchuk_init = false;
    isWiimote = _isWiimote;
    //cwiid_btn = NULL;
    //cwiid_acc = NULL;
    //cwiid_ir = NULL;
    //cwiid_nunchuk = NULL;
    //cwiid_classic = NULL;
    //cwiid_motionplus = NULL;
    wiimote = NULL;
    //bdaddr = _bdaddr;
    memset(&bdaddr,0,sizeof(bdaddr_t));

    std::cout << "Put the " << (isWiimote?"wiimote":"wiibalance") <<" in discoverable mode now (press 1+2)..." << std::endl; 
    //wiimote = cwiid_open(&bdaddr, CWIID_FLAG_MESG_IFC);
    wiimote = cwiid_open(&bdaddr, CWIID_FLAG_CONTINUOUS|CWIID_FLAG_NONBLOCK);
    if (wiimote == NULL) {
        std::cout << "Didn't detect the " << (isWiimote?"wiimote":"wiibalance") << std::endl;
        return;
    }
    ba2str(&bdaddr,foundaddr);                             
	std::cout  << (isWiimote?"Wiimote":"Wiibalance") << " address : " << foundaddr << std::endl;



    if (isWiimote) {
        flag = CWIID_RPT_ACC|CWIID_RPT_BTN;
        int res = cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag);
        if (res != 0){
            std::cout << "Error while enabling accelerometer and button acquisition" << std::endl;
            close();
            return;
        }
    }else{
        flag = CWIID_RPT_BALANCE|CWIID_RPT_BTN;
        int res = cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag);
        if (res != 0){
            std::cout << "Error while enabling wii balance data acquisition" << std::endl;
            close();
            return;
        }
    }

    int res = cwiid_get_state(wiimote, &state);
    if (res != 0){
        std::cout << "Error while querying state" << std::endl;
        close();
        return;
    }

    printf("Battery state : %d%%\n" ,(int)(100.0 * state.battery / CWIID_BATTERY_MAX));

    if (isWiimote){
        // Calibration of the accelerometer
        if (cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &acc_calibration) == 0) {
            acc_init = true;
        }else {
            acc_init = false;
            std::cout << "Error while calibrating the accelerometer" << std::endl;
        }
    }else{
        // Calibration of the balance
        if (cwiid_get_balance_cal(wiimote, &balance_cal) == 0) {
            balance_cal_init = true;
        }else {
            balance_cal_init = false;
            std::cout << "Error while calibrating the balance" << std::endl;
        }
    }

    init = true;
}


bool Wiimote::enableIR() {
    if (!init || !isWiimote) return false;
    flag |= CWIID_RPT_IR;
    bool res = (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
    if (!res) flag -= CWIID_RPT_IR;
    return res;
}

bool Wiimote::disableIR() {
    if (!init || !isWiimote) return false;
    if (flag & CWIID_RPT_IR) {
        flag -= CWIID_RPT_IR;
    }
    return (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
}


bool Wiimote::enableMotionPlus() {
    if (!init || !isWiimote) return false;
    flag |= CWIID_RPT_MOTIONPLUS;
    bool res = (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
    if (!res) flag -= CWIID_RPT_MOTIONPLUS;
    return res;
}

bool Wiimote::disableMotionPlus() {
    if (!init || !isWiimote) return false;
    if (flag & CWIID_RPT_MOTIONPLUS) {
        flag -= CWIID_RPT_MOTIONPLUS;
    }
    return (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
}

bool Wiimote::enableNunchuk() {
    if (!init || !isWiimote) return false;

    update();
    if (state.ext_type != CWIID_EXT_NUNCHUK) {
        std::cout << "No nunchuk connected, can't enabled it" << std::endl;
        return false;
    }

    flag |= CWIID_RPT_NUNCHUK;
    bool res = (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
    if (!res) flag -= CWIID_RPT_NUNCHUK;

    acc_nunchuk_init = false;
    if (res) {
        // Calibration of the accelerometer of the nunchuk
        if (cwiid_get_acc_cal(wiimote, CWIID_EXT_NUNCHUK, &acc_calibration_nunchuk) == 0) {
            acc_nunchuk_init = true;
        }else {
            std::cout << "Error while calibrating the accelerometer" << std::endl;
        }
    }

    return res;
}

bool Wiimote::disableNunchuk() {
    if (!init || !isWiimote) return false;
    if (flag & CWIID_RPT_NUNCHUK) {
        flag -= CWIID_RPT_NUNCHUK;
    }
    acc_nunchuk_init = false;
    return (cwiid_command(wiimote, CWIID_CMD_RPT_MODE, flag) == 0);
}



void Wiimote::close(){
    if (wiimote != NULL) {
        if (cwiid_disconnect(wiimote) != 0) {
            std::cout << "Failed to disconnect " << foundaddr << std::endl;
        }else {
            std::cout << foundaddr << " disconnected" << std::endl;
        }

    }

}

void Wiimote::startRumble() {
    if (!init || !isWiimote) return;
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 1);
}

void Wiimote::stopRumble() {
    if (!init || !isWiimote) return;
    cwiid_command(wiimote, CWIID_CMD_RUMBLE, 0);
}

void Wiimote::enableLeds(int led){
    if (!init || !isWiimote) return;
    cwiid_command(wiimote, CWIID_CMD_LED, led);

}

bool Wiimote::update(){
    if (!init) return false;
    int res = cwiid_get_state(wiimote,&state);
    //std::cout << "State : " << (int)state.ext_type << std::endl;
    return (res == 0);
}


int Wiimote::getButtonPressed(){
    if (!init || !isWiimote) return 0;
    return state.buttons;
}

int Wiimote::getButtonPressedNunchuk(){
    if (!init || !isWiimote) return 0;

    if (!(flag & CWIID_RPT_NUNCHUK)) {
        if (enableNunchuk()) update();
        else {
            std::cout << "Warning : Unable to initialize the nunchuk" << std::endl;
            return 0;
        }
    }
    if (state.ext_type == CWIID_EXT_NUNCHUK) {
        struct nunchuk_state nunchuk;
        nunchuk = state.ext.nunchuk;
        return nunchuk.buttons;
    }else{
        std::cout << "Warning : No nunchuk connected" << std::endl;
        return 0;
    }
    return 0;
}


void Wiimote::getJoystickNunchuk(double & x, double & y){
    x = 0;
    y = 0;
    if (!init || !isWiimote) return;

    if (!(flag & CWIID_RPT_NUNCHUK)) {
        if (enableNunchuk()) update();
        else {
            std::cout << "Warning : Unable to initialize the nunchuk" << std::endl;
            return;
        }
    }
    if (state.ext_type == CWIID_EXT_NUNCHUK) {
        struct nunchuk_state nunchuk;
        nunchuk = state.ext.nunchuk;
        x = nunchuk.stick[CWIID_X];
        y = nunchuk.stick[CWIID_Y];
    }else{
        std::cout << "Warning : No nunchuk connected" << std::endl;
        return;
    }
}

void Wiimote::getAcceleration(double & ax, double & ay, double & az){
    if (!init || !isWiimote){
        ax = 0;
        ay = 0;
        az = 0;
    }else{
        if (acc_init) {
            ax = ((double)state.acc[CWIID_X] - acc_calibration.zero[CWIID_X]) / (acc_calibration.one[CWIID_X] - acc_calibration.zero[CWIID_X]);
            ay = ((double)state.acc[CWIID_Y] - acc_calibration.zero[CWIID_Y]) / (acc_calibration.one[CWIID_Y] - acc_calibration.zero[CWIID_Y]);
            az = ((double)state.acc[CWIID_Z] - acc_calibration.zero[CWIID_Z]) / (acc_calibration.one[CWIID_Z] - acc_calibration.zero[CWIID_Z]);
        }else{
            ax = (double)state.acc[CWIID_X];
            ay = (double)state.acc[CWIID_Y];
            az = (double)state.acc[CWIID_Z];
        }

    }
}


void Wiimote::getAccelerationNunchuk(double & ax, double & ay, double & az){
    if (!init || !isWiimote){
        ax = 0;
        ay = 0;
        az = 0;
    }else{
        if (!(flag & CWIID_RPT_NUNCHUK)) {
            if (enableNunchuk()) update();
            else {
                std::cout << "Warning : Unable to initialize the nunchuk" << std::endl;
                ax = 0;
                ay = 0;
                az = 0;
                return;
            }
        }
        if (state.ext_type == CWIID_EXT_NUNCHUK) {
            struct nunchuk_state nunchuk;
            nunchuk = state.ext.nunchuk;
            if (acc_nunchuk_init) {
                ax = ((double)nunchuk.acc[CWIID_X] - acc_calibration_nunchuk.zero[CWIID_X]) / (acc_calibration_nunchuk.one[CWIID_X] - acc_calibration_nunchuk.zero[CWIID_X]);
                ay = ((double)nunchuk.acc[CWIID_Y] - acc_calibration_nunchuk.zero[CWIID_Y]) / (acc_calibration_nunchuk.one[CWIID_Y] - acc_calibration_nunchuk.zero[CWIID_Y]);
                az = ((double)nunchuk.acc[CWIID_Z] - acc_calibration_nunchuk.zero[CWIID_Z]) / (acc_calibration_nunchuk.one[CWIID_Z] - acc_calibration_nunchuk.zero[CWIID_Z]);
            }else{
                ax = (double)nunchuk.acc[CWIID_X];
                ay = (double)nunchuk.acc[CWIID_Y];
                az = (double)nunchuk.acc[CWIID_Z];
            }
        }else{
            std::cout << "Warning : No nunchuk connected" << std::endl;
        }

    }
}

cwiid_ir_src * Wiimote::getIRData(){
    if (!init || !isWiimote) return NULL;
    
    if (!(flag & CWIID_RPT_IR)) {
        if (enableIR()) update();
        else {
            std::cout << "Warning : Unable to initialize the IR" << std::endl;
            return NULL;
        }
    }
    return state.ir_src;
}


void Wiimote::getSensorBalance(double & right_top, double & right_bottom, double & left_top, double & left_bottom){
    right_top = 0.0;
    right_bottom = 0.0;
    left_top = 0.0;
    left_bottom = 0.0;

    if (!init || isWiimote) return;

    struct balance_state balance;
    balance = state.ext.balance;
    right_top = balance.right_top;
    right_bottom = balance.right_bottom;
    left_top = balance.left_top;
    left_bottom = balance.left_bottom;

    if (balance_cal_init) {
       // right_top = (right_top - balance_cal.right_top[0]) / (balance_cal.right_top[1] - balance_cal.right_top[0]);
       // right_bottom = (right_bottom - balance_cal.right_bottom[0]) / (balance_cal.right_bottom[1] - balance_cal.right_bottom[0]);
       // left_top = (left_top - balance_cal.left_top[0]) / (balance_cal.left_top[1] - balance_cal.left_top[0]);
       // left_bottom = (left_bottom - balance_cal.left_bottom[0]) / (balance_cal.left_bottom[1] - balance_cal.left_bottom[0]);

        double readings[4] = {right_top,right_bottom,left_top,left_bottom};
        double calibration0[4] = {balance_cal.right_top[0], balance_cal.right_bottom[0], balance_cal.left_top[0],balance_cal.left_bottom[0]};
        double calibration1[4] = {balance_cal.right_top[1], balance_cal.right_bottom[1], balance_cal.left_top[1],balance_cal.left_bottom[1]};
        double calibration2[4] = {balance_cal.right_top[2], balance_cal.right_bottom[2], balance_cal.left_top[2],balance_cal.left_bottom[2]};
        const char * sensor[4] = {"right_top","right_bottom","left_top","left_bottom"};

        for (int i = 0; i < 4; i++) {
            if (readings[i] > calibration2[i]){
                std::cout << "Warning " << sensor[i] << " above upper calibration value" << std::endl;
            }
            if (readings[i] < calibration1[i]) {
                readings[i] = WEIGHT_COEFF * ( readings[i] - calibration0[i] ) / ( calibration1[i] - calibration0[i] );
            }else {
                readings[i] = WEIGHT_COEFF * ( readings[i] - calibration1[i] ) / ( calibration2[i] - calibration1[i] ) + WEIGHT_COEFF;
            }
        }
        
        right_top = readings[0] / 100.0;
        right_bottom = readings[1] / 100.0;
        left_top = readings[2] / 100.0;
        left_bottom = readings[3] / 100.0;

    }

}

double Wiimote::computeWeight() {
    double right_top, right_bottom, left_top, left_bottom;
    if (!init || isWiimote) return 0.0;
    if (!balance_cal_init) {
        std::cout << "The balance need to be calibrated first" << std::endl;
        return 0.0;
    }


    getSensorBalance(right_top,right_bottom,left_top,left_bottom);
    return right_top + right_bottom + left_top + left_bottom;


    //struct balance_state balance;
    //balance = state.ext.balance;
    //right_top = balance.right_top;
    //right_bottom = balance.right_bottom;
    //left_top = balance.left_top;
    //left_bottom = balance.left_bottom;

    //double readings[4] = {right_top,right_bottom,left_top,left_bottom};
    //double calibration0[4] = {balance_cal.right_top[0], balance_cal.right_bottom[0], balance_cal.left_top[0],balance_cal.left_bottom[0]};
    //double calibration1[4] = {balance_cal.right_top[1], balance_cal.right_bottom[1], balance_cal.left_top[1],balance_cal.left_bottom[1]};
    //double calibration2[4] = {balance_cal.right_top[2], balance_cal.right_bottom[2], balance_cal.left_top[2],balance_cal.left_bottom[2]};
    //const char * sensor[4] = {"right_top","right_bottom","left_top","left_bottom"};
    //double weight = 0.0;

    //for (int i = 0; i < 4; i++) {
    //    if (readings[i] > calibration2[i]){
    //        std::cout << "Warning " << sensor[i] << " above upper calibration value" << std::endl;
    //    }
    //    if (readings[i] < calibration1[i]) {
    //        weight += WEIGHT_COEFF * ( readings[i] - calibration0[i] ) / ( calibration1[i] - calibration0[i] );
    //    }else {
    //        weight += WEIGHT_COEFF * ( readings[i] - calibration1[i] ) / ( calibration2[i] - calibration1[i] ) + WEIGHT_COEFF;
    //    }

    //}


    //if (right_top <  balance_cal.right_top[1]){
    //    weight += WEIGHT_COEFF * (right_top - balance_cal.right_top[0]) / (balance_cal.right_top[1] - balance_cal.right_top[0]);
    //}else{
    //    weight += WEIGHT_COEFF * (right_top - balance_cal.right_top[1]) / (balance_cal.right_top[2] - balance_cal.right_top[1]) + WEIGHT_COEFF;
    //}

    //if (right_bottom <  balance_cal.right_bottom[1]){
    //    weight += WEIGHT_COEFF * (right_bottom - balance_cal.right_bottom[0]) / (balance_cal.right_bottom[1] - balance_cal.right_bottom[0]);
    //}else{
    //    weight += WEIGHT_COEFF * (right_bottom - balance_cal.right_bottom[1]) / (balance_cal.right_bottom[2] - balance_cal.right_bottom[1]) + WEIGHT_COEFF;
    //}

    //if (left_bottom <  balance_cal.left_bottom[1]){
    //    weight += WEIGHT_COEFF * (left_bottom - balance_cal.left_bottom[0]) / (balance_cal.left_bottom[1] - balance_cal.left_bottom[0]);
    //}else{
    //    weight += WEIGHT_COEFF * (left_bottom - balance_cal.left_bottom[1]) / (balance_cal.left_bottom[2] - balance_cal.left_bottom[1]) + WEIGHT_COEFF;
    //}

    //if (left_top <  balance_cal.left_top[1]){
    //    weight += WEIGHT_COEFF * (left_top - balance_cal.left_top[0]) / (balance_cal.left_top[1] - balance_cal.left_top[0]);
    //}else{
    //    weight += WEIGHT_COEFF * (left_top - balance_cal.left_top[1]) / (balance_cal.left_top[2] - balance_cal.left_top[1]) + WEIGHT_COEFF;
    //}

    //return weight / 100.0;

}

double Wiimote::getBatteryStatus() {
    if (!init) return 0.0;
    return state.battery / CWIID_BATTERY_MAX;
}


//
//void Wiimote::wiimote_callback(cwiid_wiimote_t * wiimote,   /* The Wiimote struct */
//                 int msg_count,               /* The number of messages received */
//                 union cwiid_mesg msg_array[],     /* The messages themselves */
//                 struct timespec * timestamp  /* The timestamp of the callback */){
//	for (int i = 0; i < msg_count; i++) {
//        switch (msg_array[i].type) {
//            case CWIID_MESG_STATUS:
//
//                break;
//                /*
//            case CWIID_EXT_NONE:
//
//                break;
//            case CWIID_EXT_NUNCHUK:
//
//                break;
//            case CWIID_EXT_CLASSIC:
//                //ext_str = "Classic controller";
//                break;
//            case CWIID_EXT_MOTIONPLUS:
//                //ext_str = "MotionPlus";
//                break;
//            case CWIID_EXT_UNKNOWN:
//                //ext_str = "Unknown extension";
//                break;
//            case CWIID_MESG_BTN:
//                if (cwiid_btn != NULL) cwiid_btn(&msg_array[i].btn_mesg);
//                break;
//            case CWIID_MESG_ACC:
//                if (cwiid_acc != NULL) cwiid_acc(&msg_array[i].acc_mesg);
//                break;
//            case CWIID_MESG_IR:
//                if (cwiid_ir != NULL) cwiid_ir(&msg_array[i].ir_mesg);
//                break;
//            case CWIID_MESG_NUNCHUK:
//                if (cwiid_nunchuk != NULL) cwiid_nunchuk(&msg_array[i].nunchuk_mesg);
//                break;
//            case CWIID_MESG_CLASSIC:
//                if (cwiid_classic != NULL) cwiid_classic(&msg_array[i].classic_mesg);
//                break;
//            case CWIID_MESG_MOTIONPLUS:
//                if (cwidd_motionplus != NULL) cwiid_motionplus(&msg_array[i].motionplus_mesg);
//                break;
//            case CWIID_MESG_ERROR:
//                std::cout << "Error...disconnecting" << std::endl;
//                cwiid_close(wiimote);
//                init = false;
//                break;
//                */
//            default:
//                break;
//        }
//
//    }
//
//}
//




Wii::Wii() {
    wiimotes.clear();
}

int Wii::addWiimote() {
    Wiimote * wii = new Wiimote(true);
    if (! wii->isInit()){
        std::cout << "Error while adding the wiimote" << std::endl;
        return -1;
    }
    wiimotes.push_back(wii);
    return wiimotes.size() - 1;
}

int Wii::addWiibalance() {
    Wiimote * wii = new Wiimote(false);
    if (! wii->isInit()){
        std::cout << "Error while adding the wii balance" << std::endl;
        return -1;
    }
    wiimotes.push_back(wii);
    return wiimotes.size() - 1;

}


bool Wii::update() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (!update(i)){
            std::cout << "Error while updating device " << i << "...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::update(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii update : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii update : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->update();

    //struct cwiid_state * state = &wii->state;
    //cwiid_wiimote_t * wiimote = wii->wiimote;
    //
    //int res = cwiid_get_state(wiimote,state);
    //if (res != 0) {
    //    std::cout << "Wii update : Can't get state of wiimote id=" << id << " return code : " << res << std::endl;
    //    return;
    //}
    ////std::cout << "buttons : " << state->buttons << std::endl;
    ////std::cout << "acc : " << (int)state->acc[0] << " , " << (int)state->acc[1] << " , " << (int)state->acc[2] << std::endl;
    //std::cout << "Report mode : " << (int)state->rpt_mode << std::endl;

}


void Wii::close(){
    for (unsigned int i = 0; i < wiimotes.size(); i++) {
        if (wiimotes[i]->isInit()) wiimotes[i]->close();
    }
}

bool Wii::enableIR() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i) && !enableIR(i)){
            std::cout << "Error while enabling IR for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::enableIR(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii enableIR : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii enableIR : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->enableIR();

}

bool Wii::disableIR() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i) && !disableIR(i)){
            std::cout << "Error while disabling IR for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::disableIR(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii disableIR : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii disableIR : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->disableIR();

}


bool Wii::enableNunchuk() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i) && !enableNunchuk(i)){
            std::cout << "Error while enabling Nunchuk for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::enableNunchuk(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii enableNunchuk : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii enableNunchuk : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->enableNunchuk();

}


bool Wii::disableNunchuk() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i) && !disableNunchuk(i)){
            std::cout << "Error while disabling Nunchuk for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::disableNunchuk(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii disableNunchuk : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii disableNunchuk : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->disableNunchuk();

}


void Wii::startRumble() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i)) startRumble(i);
    }
}

void Wii::startRumble(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii startRumble : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii startRumble : Wiimote not initialized" << std::endl;
        return;
    }
    return wii->startRumble();

}


void Wii::stopRumble() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i)) stopRumble(i);
    }
}

void Wii::stopRumble(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii stopRumble : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii stopRumble : Wiimote not initialized" << std::endl;
        return;
    }
    return wii->stopRumble();

}

void Wii::enableLeds(int led) {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if (isAWiimote(i)) enableLeds(led,i);
    }
}

void Wii::enableLeds(int led,unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii enableLeds : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii enableLeds : Wiimote not initialized" << std::endl;
        return;
    }
    return wii->enableLeds(led);

}

int Wii::getButtonPressed(unsigned int id) {
    if (id >= wiimotes.size()) {
        std::cout << "Wii getButtonPressed : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return 0;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getButtonPressed : Wiimote not initialized" << std::endl;
        return 0;
    }
    return wii->getButtonPressed();
}

int Wii::getButtonPressedNunchuk(unsigned int id) {
    if (id >= wiimotes.size()) {
        std::cout << "Wii getButtonPressedNunchuk : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return 0;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getButtonPressedNunchuk : Wiimote not initialized" << std::endl;
        return 0;
    }
    return wii->getButtonPressedNunchuk();
}

void Wii::getJoystickNunchuk(double & x, double & y, unsigned int id){
    x = 0;
    y = 0;
    if (id >= wiimotes.size()) {
        std::cout << "Wii getJoystickNunchuk : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getJoystickNunchuk : Wiimote not initialized" << std::endl;
        return;
    }
    
    wii->getJoystickNunchuk(x,y);
}



void Wii::getAcceleration(double & ax, double & ay, double & az, unsigned int id){
    ax = 0;
    ay = 0;
    az = 0;
    if (id >= wiimotes.size()) {
        std::cout << "Wii getAcceleration : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getAcceleration : Wiimote not initialized" << std::endl;
        return;
    }
    
    wii->getAcceleration(ax,ay,az);


}


void Wii::getAccelerationNunchuk(double & ax, double & ay, double & az, unsigned int id){
    ax = 0;
    ay = 0;
    az = 0;
    if (id >= wiimotes.size()) {
        std::cout << "Wii getAccelerationNunchuk : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getAccelerationNunchuk : Wiimote not initialized" << std::endl;
        return;
    }
    
    wii->getAccelerationNunchuk(ax,ay,az);


}




bool Wii::enableMotionPlus() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if ((isAWiimote(i)) && !enableMotionPlus(i)){
            std::cout << "Error while enabling MotionPlus for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::enableMotionPlus(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii enableMotionPlus : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii enableMotionPlus : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->enableMotionPlus();

}

bool Wii::disableMotionPlus() {
    for (unsigned int i = 0; i < wiimotes.size(); i++){
        if ((isAWiimote(i)) && !disableMotionPlus(i)){
            std::cout << "Error while disabling MotionPlus for " << i << " ...aborting" << std::endl;
            return false;
        }
    }
    return true;
}

bool Wii::disableMotionPlus(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii disableMotionPlus : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return false;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii disableMotionPlus : Wiimote not initialized" << std::endl;
        return false;
    }
    return wii->disableMotionPlus();

}


cwiid_ir_src * Wii::getIRData(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii getIRData : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return NULL;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getIRData : Wiimote not initialized" << std::endl;
        return NULL;
    }
    return wii->getIRData();

}

bool Wii::isAWiimote(unsigned int id){
    if (id >= wiimotes.size()) return false;
    return wiimotes[id]->isAWiimote();

}

void Wii::getSensorBalance(double & right_top, double & right_bottom, double & left_top, double & left_bottom, unsigned int id){
    right_top = 0;
    right_bottom = 0;
    left_top = 0;
    left_bottom = 0;
    if (id >= wiimotes.size()) {
        std::cout << "Wii getSensorBalance : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getSensorData : Wiibalance not initialized" << std::endl;
        return;
    }

    wii->getSensorBalance(right_top,right_bottom,left_top,left_bottom);

}

double Wii::computeWeight(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii computeWeight : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return 0.0;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii computeWeight : Wiibalance not initialized" << std::endl;
        return 0.0;
    }

    return wii->computeWeight();

}

double Wii::getBatteryStatus(unsigned int id){
    if (id >= wiimotes.size()) {
        std::cout << "Wii getBatteryStatus : Index out of bound (" << id << " >= " << wiimotes.size() << ")" << std::endl;
        return 0.0;
    }
    Wiimote * wii = wiimotes[id];
    if (!wii->isInit()){
        std::cout << "Wii getBatteryStatus : Device not initialized" << std::endl;
        return 0.0;
    }

    return wii->getBatteryStatus();

}
