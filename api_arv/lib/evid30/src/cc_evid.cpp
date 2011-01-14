// $Id: cc_evid.cpp,v 1.4 2000/03/07 14:11:14 schwerdt Exp $
// $Log: cc_evid.cpp,v $
//




#include <cc_evid.hpp>





////  Speed Limits  ////////////////////////////////////////
static const long MAX_PAN_SPEED = 0x18;
static const long MIN_PAN_SPEED = 0x01;
static const long MAX_TILT_SPEED = 0x14;
static const long MIN_TILT_SPEED = 0x01;
static const long MAX_ZOOM_SPEED = 0x07;
static const long MIN_ZOOM_SPEED = 0x01;//0x02;
// -- Position Limits ----------------------------------
static const long MAX_ABS_PAN = 0x0370;	// approx;
static const long MIN_ABS_PAN = -0x0370;	// approx;
static const long MAX_ABS_TILT = 0x012c;	// approx;
static const long MIN_ABS_TILT = -0x012c;	// approx;
static const long MAX_ZOOM = 0x03FF;	// tele
static const long MIN_ZOOM = 0x0000;	// wide
static const long MAX_FOCUS = 0x9FFF;	// close
static const long MIN_FOCUS = 0x1000;	// infinity







///////////////////////////////////////////////////////////////// Variables
// -- Commands -----------------------------------------
static char ADDRESS_SET[] = { 4, 0x88, 0x30, 0x01, 0xFF };
static char IF_CLEAR[] = { 5, 0x88, 0x01, 0x00, 0x01, 0xFF };
static char CANCEL[] = { 3, 0x81, 0x21, 0xFF };

static char ON[] = { 6, 0x81, 0x01, 0x04, 0x00, 0x02, 0xFF };
static char OFF[] = { 6, 0x81, 0x01, 0x04, 0x00, 0x03, 0xFF };

static char STOP_ZOOM[] = { 6, 0x81, 0x01, 0x04, 0x07, 0x00, 0xFF };
static char TELE_STD[] = { 6, 0x81, 0x01, 0x04, 0x07, 0x02, 0xFF };
static char WIDE_STD[] = { 6, 0x81, 0x01, 0x04, 0x07, 0x03, 0xFF };
static char TELE_VAR[] = { 6, 0x81, 0x01, 0x04, 0x07, 0x27, 0xFF };
static char WIDE_VAR[] = { 6, 0x81, 0x01, 0x04, 0x07, 0x37, 0xFF };
static char ZOOM_DIRECT[] =
    { 9, 0x81, 0x01, 0x04, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF };

static char STOP_FOCUS[] = { 6, 0x81, 0x01, 0x04, 0x08, 0x00, 0xFF };
static char FAR[] = { 6, 0x81, 0x01, 0x04, 0x08, 0x02, 0xFF };
static char NEAR[] = { 6, 0x81, 0x01, 0x04, 0x08, 0x03, 0xFF };
static char AUTO_FOCUS_ON[] = { 6, 0x81, 0x01, 0x04, 0x38, 0x02, 0xFF };
static char MAN_FOCUS_ON[] = { 6, 0x81, 0x01, 0x04, 0x38, 0x03, 0xFF };
static char AUTO_MAN_FOCUS[] = { 6, 0x81, 0x01, 0x04, 0x38, 0x10, 0xFF };
static char FOCUS_DIRECT[] =
    { 9, 0x81, 0x01, 0x04, 0x48, 0x00, 0x00, 0x00, 0x00, 0xFF };

static char CAM_AE_FULL_AUTO[] = { 6, 0x81, 0x01, 0x04, 0x39, 0x00, 0xFF };
static char CAM_AE_MANUAL[] = { 6, 0x81, 0x01, 0x04, 0x39, 0x03, 0xFF };
static char CAM_AE_SHUT_PRIO[] = { 6, 0x81, 0x01, 0x04, 0x39, 0x0A, 0xFF };
static char CAM_AE_IRIS_PRIO[] = { 6, 0x81, 0x01, 0x04, 0x39, 0x0B, 0xFF };
static char CAM_AE_BRIGHT_MODE[] =
    { 6, 0x81, 0x01, 0x04, 0x39, 0x0D, 0xFF };

static char CAM_BRIGHT_RESET[] = { 6, 0x81, 0x01, 0x04, 0x0D, 0x00, 0xFF };
static char CAM_BRIGHT_UP[] = { 6, 0x81, 0x01, 0x04, 0x0D, 0x02, 0xFF };
static char CAM_BRIGHT_DOWN[] = { 6, 0x81, 0x01, 0x04, 0x0D, 0x03, 0xFF };

static char CAM_SHUT_RESET[] = { 6, 0x81, 0x01, 0x04, 0x0A, 0x00, 0xFF };
static char CAM_SHUT_UP[] = { 6, 0x81, 0x01, 0x04, 0x0A, 0x02, 0xFF };
static char CAM_SHUT_DOWN[] = { 6, 0x81, 0x01, 0x04, 0x0A, 0x03, 0xFF };
static char CAM_SHUT_DIRECT[] =
    { 9, 0x81, 0x01, 0x04, 0x4A, 0x00, 0x00, 0x00, 0x00, 0xFF };

static char CAM_IRIS_RESET[] = { 6, 0x81, 0x01, 0x04, 0x0B, 0x00, 0xFF };
static char CAM_IRIS_UP[] = { 6, 0x81, 0x01, 0x04, 0x0B, 0x02, 0xFF };
static char CAM_IRIS_DOWN[] = { 6, 0x81, 0x01, 0x04, 0x0B, 0x03, 0xFF };
static char CAM_IRIS_DIRECT[] =
    { 9, 0x81, 0x01, 0x04, 0x4B, 0x00, 0x00, 0x00, 0x00, 0xFF };

static char CAM_GAIN_RESET[] = { 6, 0x81, 0x01, 0x04, 0x0C, 0x00, 0xFF };
static char CAM_GAIN_UP[] = { 6, 0x81, 0x01, 0x04, 0x0C, 0x02, 0xFF };
static char CAM_GAIN_DOWN[] = { 6, 0x81, 0x01, 0x04, 0x0C, 0x03, 0xFF };
static char CAM_GAIN_DIRECT[] =
    { 9, 0x81, 0x01, 0x04, 0x4C, 0x00, 0x00, 0x00, 0x00, 0xFF };

static char BACKLIGHT_ON[] = { 6, 0x81, 0x01, 0x04, 0x33, 0x02, 0xFF };
static char BACKLIGHT_OFF[] = { 6, 0x81, 0x01, 0x04, 0x33, 0x03, 0xFF };

static char UP[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x03, 0x01, 0xFF };
static char DOWN[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x03, 0x02, 0xFF };
static char LEFT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x01, 0x03, 0xFF };
static char RIGHT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x02, 0x03, 0xFF };
static char UPLEFT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x01, 0x01, 0xFF };
static char UPRIGHT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x02, 0x01, 0xFF };
static char DOWNLEFT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x01, 0x02, 0xFF };
static char DOWNRIGHT[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x18, 0x14, 0x02, 0x02, 0xFF };
static char STOP[] =
    { 9, 0x81, 0x01, 0x06, 0x01, 0x01, 0x01, 0x03, 0x03, 0xFF };
static char ABS_POSITION[] =
    { 15, 0x81, 0x01, 0x06, 0x02, 0x18, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF };
static char HOME[] = { 5, 0x81, 0x01, 0x06, 0x04, 0xFF };
static char RESET[] = { 5, 0x81, 0x01, 0x06, 0x05, 0xFF };

static char DATASCREEN_ON[] = { 6, 0x081, 0x01, 0x06, 0x06, 0x02, 0xFF };
static char DATASCREEN_OFF[] = { 6, 0x081, 0x01, 0x06, 0x06, 0x03, 0xFF };
static char DATASCREEN_ONOFF[] =
    { 6, 0x081, 0x01, 0x06, 0x06, 0x10, 0xFF };

static char AT_MODE_ON[] = { 6, 0x081, 0x01, 0x07, 0x01, 0x02, 0xFF };
static char AT_MODE_OFF[] = { 6, 0x081, 0x01, 0x07, 0x01, 0x03, 0xFF };
static char AT_MODE_ONOFF[] = { 6, 0x081, 0x01, 0x07, 0x01, 0x10, 0xFF };

static char AT_AE_ON[] = { 6, 0x081, 0x01, 0x07, 0x02, 0x02, 0xFF };
static char AT_AE_OFF[] = { 6, 0x081, 0x01, 0x07, 0x02, 0x03, 0xFF };
static char AT_AE_ONOFF[] = { 6, 0x081, 0x01, 0x07, 0x02, 0x10, 0xFF };

static char AT_AUTO_ZOOM_ON[] = { 6, 0x081, 0x01, 0x07, 0x03, 0x02, 0xFF };
static char AT_AUTO_ZOOM_OFF[] =
    { 6, 0x081, 0x01, 0x07, 0x03, 0x03, 0xFF };
static char AT_AUTOZOOM_ONOFF[] =
    { 6, 0x081, 0x01, 0x07, 0x03, 0x10, 0xFF };

static char MD_START_STOP[] = { 6, 0x081, 0x01, 0x07, 0x06, 0x10, 0xFF };
static char MD_MODE_ON[] = { 6, 0x081, 0x01, 0x07, 0x08, 0x02, 0xFF };
static char MD_MODE_OFF[] = { 6, 0x081, 0x01, 0x07, 0x08, 0x03, 0xFF };
static char MD_MODE_ONOFF[] = { 6, 0x081, 0x01, 0x07, 0x08, 0x10, 0xFF };

static char MD_ADJUST_Y[] =
    { 7, 0x081, 0x01, 0x07, 0x0B, 0x00, 0x00, 0xFF };
static char MD_ADJUST_HUE[] =
    { 7, 0x081, 0x01, 0x07, 0x0C, 0x00, 0x00, 0xFF };
static char MD_ADJUST_SIZE[] =
    { 7, 0x081, 0x01, 0x07, 0x0D, 0x00, 0x00, 0xFF };
static char MD_ADJUST_D_TIME[] =
    { 7, 0x081, 0x01, 0x07, 0x0F, 0x00, 0x00, 0xFF };
static char MD_REFR_MODE_1[] = { 6, 0x081, 0x01, 0x07, 0x10, 0x00, 0xFF };
static char MD_REFR_MODE_2[] = { 6, 0x081, 0x01, 0x07, 0x10, 0x01, 0xFF };
static char MD_REFR_MODE_3[] = { 6, 0x081, 0x01, 0x07, 0x10, 0x02, 0xFF };
static char MD_ADJUST_R_TIME[] =
    { 7, 0x081, 0x01, 0x07, 0x0B, 0x00, 0x00, 0xFF };


// -- Inquiry Commands ---------------------------------------------------------
static char ZOOM_POS[] = { 5, 0x081, 0x09, 0x04, 0x47, 0xFF };
static char FOCUS_POS[] = { 5, 0x081, 0x09, 0x04, 0x48, 0xFF };
static char AUTO_EXPOSURE_MODE[] = { 5, 0x081, 0x09, 0x04, 0x39, 0xFF };
static char SHUTTER_POS[] = { 5, 0x081, 0x09, 0x04, 0x4A, 0xFF };
static char IRIS_POS[] = { 5, 0x081, 0x09, 0x04, 0x4B, 0xFF };
static char GAIN_POS[] = { 5, 0x081, 0x09, 0x04, 0x4C, 0xFF };
static char PAN_TILT_POS[] = { 5, 0x081, 0x09, 0x06, 0x12, 0xFF };








////////////////////////////////////////////////////////////////////////////////
template < class T > inline void FourBytes2Long(T * bytes, long &oLong)
{
    oLong = 0;
    oLong = bytes[0];
    oLong <<= 4;
    oLong |= bytes[1];
    oLong <<= 4;
    oLong |= bytes[2];
    oLong <<= 4;
    oLong |= bytes[3];
    oLong = (oLong > 0x1000) ? (oLong - 0xFFFF) : oLong;
}







////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
inline void Long2FourBytes(const long &oLong, char *bytes)
{
    bytes[0] = static_cast < char >((oLong >> 12) & 0x0F);
    bytes[1] = static_cast < char >((oLong >> 8) & 0x0F);
    bytes[2] = static_cast < char >((oLong >> 4) & 0x0F);
    bytes[3] = static_cast < char >(oLong & 0x0F);
}






CEVID30 & CEVID30::operator=(const CEVID30 & i_evid)
{
    if (this != &i_evid) {
	m_Answer = i_evid.m_Answer;
	m_PanSpeed = i_evid.m_PanSpeed;
	m_TiltSpeed = i_evid.m_TiltSpeed;
	m_ZoomSpeed = i_evid.m_ZoomSpeed;
	m_PanPosition = i_evid.m_PanPosition;
	m_TiltPosition = i_evid.m_TiltPosition;
	m_ZoomPosition = i_evid.m_ZoomPosition;
    }
    return *this;
}







int CEVID30::Init(const long &iLineNum)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::Init" << std::endl;
    struct termios tdata;	// "old" termios structure: set _OLD_TERMIOS
    char *line = (iLineNum == 1) ? (char *) SERIAL1 : (iLineNum == 2) ?
	(char *) SERIAL2 : (char *) "\0";
    m_Handle = -1;

    ////  open serial line for communication  ////////////////
    if ((m_Handle = open(line, O_RDWR | O_NOCTTY)) < 0) {
	perror("CEVID30::Init ");
	return -1;
    }

    ////  get communication parameters  //////////////////////
    if (ioctl(m_Handle, TCGETS, &tdata) < 0) {
	perror("CEVID30::Init ");
	return -1;
    }
    // at this point, make sure that the 'right' <termios.h> version
    // is used. add #define _OLD_TERMIOS
    tdata.c_iflag = 0;
    tdata.c_oflag = 0;
    tdata.c_cflag = B9600 | CS8 | CSTOPB | CREAD | CLOCAL;
    tdata.c_lflag = 0;
    tdata.c_cc[VMIN] = 0;
    tdata.c_cc[VTIME] = 3;

    ////  set communication parameters  //////////////////////
    if (ioctl(m_Handle, TCSETS, &tdata) < 0) {
	perror("CEVID30::Init ");
	return -1;
    }

    ////  now send intialization commands to camera  /////////
    int err = 0;
    if ((err = this->addressSet()))
	return err;

    ////  if addressSet() could not assign an address > 0 (usually a 1, if
    //    only one camera is connected, then we don't have access to a 
    //    camera at serial line iLineNum
    //
    if (!m_Address)
	return -1;
    else {
	m_RecvAddress = 0x80 + (m_Address << 4);
	m_Address = 0x80 + m_Address;
//    std::cout << "m_Address = " << hex << (int)m_Address << dec << std::endl;
//    std::cout << "m_RecvAddress = " << hex << (int)m_RecvAddress << dec << std::endl;
    }

    if ((err = this->ifClear())) {
	return err;
    }

    ////  the reset is ABSOLUTELY necessary  /////////////////
    //    to make the camera addressable
    if ((err = this->cameraReset())) {
	return err;
    }

    if ((err = this->idle())) {
	return err;
    }
    // set default values ...
    this->setPanSpeed((MAX_PAN_SPEED + MIN_PAN_SPEED) >> 1);
    this->setTiltSpeed((MAX_TILT_SPEED + MIN_TILT_SPEED) >> 1);
    this->setZoomSpeed((MAX_ZOOM_SPEED + MIN_ZOOM_SPEED) >> 1);

    if ((err = this->backLightOff())) {
	return err;
    }

    if ((err = this->setAutoFocus())) {
	return err;
    }
//  std::cout << "[cc_evid.cpp] leaving CEVID30::Init\n" << std::endl;
    return err;
}


////////////////////
int CEVID30::addressSet(void)
{
    return this->Execute(ADDRESS_SET[0], &ADDRESS_SET[1]);
}

int CEVID30::ifClear(void)
{
    return this->Execute(IF_CLEAR[0], &IF_CLEAR[1]);
}

int CEVID30::commandCancel(const int &i_socket)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::commandCancel" << std::endl;
    CANCEL[2] = 0x20 + i_socket;
//  std::cout << "[cc_evid.cpp] CANCEL[2] = " << hex << (int)CANCEL[2] << dec << std::endl;
//  std::cout << "[cc_evid.cpp] leaving CEVID30::commandCancel\n" << std::endl;
    return this->Execute(CANCEL[0], &CANCEL[1]);
}























// @CLASS
//    CEVID30
// @NAME
//    turnPowerOn
//    turnPowerOff
//    cameraReset
//    ready
//    idle
//    reqNameOfModel
//    reqOpStatus
//    backLightOn
//    backLightOff
// @SYNOPSIS
//    int CEVID30::turnPowerOn      ( void )
//    int CEVID30::turnPowerOff     ( void )
//    int CEVID30::cameraReset      ( void )
//    int CEVID30::ready            ( void )
//    int CEVID30::idle             ( void )
//    int CEVID30::reqNameOfModel   ( char*& oName )  
//    int CEVID30::reqOpStatus      ( char*& )
//    int CEVID30::backLightOn      ( void )
//    int CEVID30::backLightOff     ( void )
// @DESCRIPTION
//    These are some basic functions to control the camera
//
//    turnPowerOn switches the camera on (from stand-by mode) 
//    turnPowerOff turns the power off (puts camera into stand-by mode)
//
//    cameraReset performs a reset of all camera functions. It is strongly
//                recommended to run cameraReset at least once at the beginning
//                of a program. cameraReset is called in the Init method,
//                that is, if the camera is initialized by Init (RECOMMENDED
//                since it is MUCH easier), cameraReset
//                does not have to be called seperately.
//
//    ready is equivalent to cameraReset
//    idle is waiting for the camera to finish its previous command
//    reqNameOfModel returns a character string with the name of the camera
//                    model
//    reqOpStatus is equivalent to cameraReset
//
//    backLightOn turns on backlight compensation
//    backLightOff turns off backlight compensation
// @SEEALSO
//    Init
int CEVID30::turnPowerOn(void)
{
    return Execute(ON[0], &ON[1]);
}

int CEVID30::turnPowerOff(void)
{
    return Execute(OFF[0], &OFF[1]);
}

int CEVID30::cameraReset(void)
{
    return Execute(RESET[0], &RESET[1]);
}

int CEVID30::ready(void)
{
    return Execute(RESET[0], &RESET[1]);
}

int CEVID30::idle(void)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::idle" << std::endl;
    int err = 0;
    do {
	err = this->Receive();
//    std::cout << m_Answer;
    }
    while ((m_Answer.message[0] & 0x40) != 0x40);
//  std::cout << "[cc_evid.cpp] leaving CEVID30::idle\n" << std::endl;
    return err;
}

int CEVID30::reqOpStatus(char *&)
{				// VC-C1 has a 3 Byte code for its operational status
    return Execute(RESET[0], &RESET[1]);
}

int CEVID30::backLightOn(void)
{
    return Execute(BACKLIGHT_ON[0], &BACKLIGHT_ON[1]);
}

int CEVID30::backLightOff(void)
{
    return Execute(BACKLIGHT_OFF[0], &BACKLIGHT_OFF[1]);
}

















// @CLASS
//    CEVID30
// @NAME
//    setFullAutoMode
//    setManualMode
//    setShutterPriority
//    setIrisPriority
//    setBrightMode
//    brightnessReset
//    setBrightnessUp
//    setBrightnessDown
//    shutterReset
//    setShutterUp
//    setShutterDown
//    setShutterPosition
//    irisReset
//    setIrisUp
//    setIrisDown
//    setIrisPosition
//    gainReset
//    setGainUp
//    setGainDown
//    setGainPosition
// @SYNOPSIS
//    int CEVID30::setFullAutoMode     ( void );
//    int CEVID30::setManualMode       ( void );
//    int CEVID30::setShutterPriority  ( void );
//    int CEVID30::setIrisPriority     ( void );
//    int CEVID30::setBrightMode       ( void );
//    int CEVID30::brightnessReset     ( void );
//    int CEVID30::setBrightnessUp     ( void );
//    int CEVID30::setBrightnessDown   ( void );
//    int CEVID30::shutterReset        ( void );
//    int CEVID30::setShutterUp        ( void );
//    int CEVID30::setShutterDown      ( void );
//    int CEVID30::setShutterPosition  ( const long& );
//    int CEVID30::irisReset           ( void );
//    int CEVID30::setIrisUp           ( void );
//    int CEVID30::setIrisDown         ( void );
//    int CEVID30::setIrisPosition     ( const long& );
//    int CEVID30::gainReset           ( void );
//    int CEVID30::setGainUp           ( void );
//    int CEVID30::setGainDown         ( void );
//    int CEVID30::setGainPosition     ( const long& );
// @DESCRIPTION
//    Exposure Mode Functions
void CEVID30::reqExposureMode(long &exposureMode)
{
    Inquiry(AUTO_EXPOSURE_MODE[0], &AUTO_EXPOSURE_MODE[1]);
    exposureMode = m_Answer.message[1];
}

int CEVID30::setFullAutoMode(void)
{
    return Execute(CAM_AE_FULL_AUTO[0], &CAM_AE_FULL_AUTO[1]);
}

int CEVID30::setManualMode(void)
{
    return Execute(CAM_AE_MANUAL[0], &CAM_AE_MANUAL[1]);
}

int CEVID30::setShutterPriority(void)
{
    return Execute(CAM_AE_SHUT_PRIO[0], &CAM_AE_SHUT_PRIO[1]);
}

int CEVID30::setIrisPriority(void)
{
    return Execute(CAM_AE_IRIS_PRIO[0], &CAM_AE_IRIS_PRIO[1]);
}

int CEVID30::setBrightMode(void)
{
    return Execute(CAM_AE_BRIGHT_MODE[0], &CAM_AE_BRIGHT_MODE[1]);
}

int CEVID30::brightnessReset(void)
{
    return Execute(CAM_BRIGHT_RESET[0], &CAM_BRIGHT_RESET[1]);
}

int CEVID30::setBrightnessUp(void)
{
    return Execute(CAM_BRIGHT_UP[0], &CAM_BRIGHT_UP[1]);
}

int CEVID30::setBrightnessDown(void)
{
    return Execute(CAM_BRIGHT_DOWN[0], &CAM_BRIGHT_DOWN[1]);
}

void CEVID30::reqShutterPosition(long &oShutterPos)
{
    Inquiry(SHUTTER_POS[0], &SHUTTER_POS[1]);
    FourBytes2Long(&m_Answer.message[1], oShutterPos);
}

int CEVID30::shutterReset(void)
{
    return Execute(CAM_SHUT_RESET[0], &CAM_SHUT_RESET[1]);
}

int CEVID30::setShutterUp(void)
{
    return Execute(CAM_SHUT_UP[0], &CAM_SHUT_UP[1]);
}

int CEVID30::setShutterDown(void)
{
    return Execute(CAM_SHUT_DOWN[0], &CAM_SHUT_DOWN[1]);
}

int CEVID30::setShutterPosition(const long &shutterPos)
{
    Long2FourBytes(shutterPos, &CAM_SHUT_DIRECT[5]);
    return Execute(CAM_SHUT_DIRECT[0], &CAM_SHUT_DIRECT[1]);
}

void CEVID30::reqIrisPosition(long &oIrisPos)
{
    Inquiry(IRIS_POS[0], &IRIS_POS[1]);
    FourBytes2Long(&m_Answer.message[1], oIrisPos);
}

int CEVID30::irisReset(void)
{
    return Execute(CAM_IRIS_RESET[0], &CAM_IRIS_RESET[1]);
}

int CEVID30::setIrisUp(void)
{
    return Execute(CAM_IRIS_UP[0], &CAM_IRIS_UP[1]);
}

int CEVID30::setIrisDown(void)
{
    return Execute(CAM_IRIS_DOWN[0], &CAM_IRIS_DOWN[1]);
}

int CEVID30::setIrisPosition(const long &irisPos)
{
    Long2FourBytes(irisPos, &CAM_IRIS_DIRECT[5]);
    return Execute(CAM_IRIS_DIRECT[0], &CAM_IRIS_DIRECT[1]);
}

void CEVID30::reqGainPosition(long &oGainPos)
{
    Inquiry(GAIN_POS[0], &GAIN_POS[1]);
    FourBytes2Long(&m_Answer.message[1], oGainPos);
}

int CEVID30::gainReset(void)
{
    return Execute(CAM_GAIN_RESET[0], &CAM_GAIN_RESET[1]);
}

int CEVID30::setGainUp(void)
{
    return Execute(CAM_GAIN_UP[0], &CAM_GAIN_UP[1]);
}

int CEVID30::setGainDown(void)
{
    return Execute(CAM_GAIN_DOWN[0], &CAM_GAIN_DOWN[1]);
}

int CEVID30::setGainPosition(const long &gainPos)
{
    Long2FourBytes(gainPos, &CAM_GAIN_DIRECT[5]);
    return Execute(CAM_GAIN_DIRECT[0], &CAM_GAIN_DIRECT[1]);
}













// @CLASS
//    CEVID30
// @NAME
//    dataScreenOn
//    dataScreenOff
//    dataScreenOnOff
// @SYNOPSIS
//    int CEVID30::dataScreenOn    ( void )
//    int CEVID30::dataScreenOff   ( void )
//    int CEVID30::dataScreenOnOff ( void )
// @DESCRIPTION
//    Switch Datascreen on and off
int CEVID30::dataScreenOn(void)
{
    return Execute(DATASCREEN_ON[0], &DATASCREEN_ON[1]);
}

int CEVID30::dataScreenOff(void)
{
    return Execute(DATASCREEN_OFF[0], &DATASCREEN_OFF[1]);
}

int CEVID30::dataScreenOnOff(void)
{
    return Execute(DATASCREEN_ONOFF[0], &DATASCREEN_ONOFF[1]);
}






















// @CLASS
//    CEVID30
// @NAME
//    startPanRight
//    startPanLeft
//    reqPanSpeed
//    setPanSpeed
//    reqPanAbsPos
//    panAbsPosition
//    reqMinPanSpeed
//    reqMaxPanSpeed
//    reqMinAbsPan
//    reqMaxAbsPan
// @SYNOPSIS
//    int CEVID30::startPanRight    ( void ) 
//    int CEVID30::startPanLeft     ( void )
//    void  CEVID30::reqPanSpeed      (       long& oPanSpeed )
//    void  CEVID30::setPanSpeed      ( const long& iPanSpeed )
//    void  CEVID30::reqPanAbsPos     (       long& oPanPos ) 
//    int CEVID30::panAbsPosition   (       long  iPanPos ) 
//    void  CEVID30::reqMinPanSpeed   (       long& oPanSpeed ) 
//    void  CEVID30::reqMaxPanSpeed   (       long& oPanSpeed )  
//    void  CEVID30::reqMinAbsPan     (       long& oPanPos ) 
//    void  CEVID30::reqMaxAbsPan     (       long& oPanPos )
// @DESCRIPTION
//    Panning commands            
int CEVID30::startPanRight(void)
{
    return FastExecute(RIGHT[0], &RIGHT[1]);
}

int CEVID30::startPanLeft(void)
{
    return FastExecute(LEFT[0], &LEFT[1]);
}

void CEVID30::reqPanSpeed(long &oPanSpeed)
{
    oPanSpeed = m_PanSpeed;
}

void CEVID30::setPanSpeed(const long &iPanSpeed)
{
    m_PanSpeed = iPanSpeed;
    LEFT[5] = RIGHT[5] = UP[5] = DOWN[5] = UPLEFT[5] = UPRIGHT[5] =
	DOWNLEFT[5] = DOWNRIGHT[5] = static_cast < char >(m_PanSpeed);
    ABS_POSITION[5] = static_cast < char >(m_PanSpeed);
}

////////////////////////////////////////////////////////////
void CEVID30::reqPanAbsPos(long &oPanPos)
{
//  do 
//  { 
//  Inquiry(PAN_TILT_POS[0], &PAN_TILT_POS[1]);
//  } while (err);
    if (!(Inquiry(PAN_TILT_POS[0], &PAN_TILT_POS[1])))
	FourBytes2Long(&m_Answer.message[1], m_PanPosition);
    else
	std::cout << "[cc_evid.cpp] Error @ CEVID30::reqPanAbsPos" << std::
	    endl;
    oPanPos = m_PanPosition;
}

////////////////////////////////////////////////////////////
int CEVID30::panAbsPosition(const long &iPanPos)
{
    long tilt_pos = 0;
    reqTiltAbsPos(tilt_pos);
    Long2FourBytes(iPanPos, &ABS_POSITION[7]);
    Long2FourBytes(tilt_pos, &ABS_POSITION[11]);
//  comout(&ABS_POSITION[0], "ABS_POSITION");
    return Execute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::FastPanAbsPosition(const long &iPanPos)
{
    long tilt_pos = 0;
    reqTiltAbsPos(tilt_pos);
    Long2FourBytes(iPanPos, &ABS_POSITION[7]);
    Long2FourBytes(tilt_pos, &ABS_POSITION[11]);
//  comout(&ABS_POSITION[0], "ABS_POSITION");
    return FastExecute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
void CEVID30::reqMinPanSpeed(long &oSpeed)
{
    oSpeed = MIN_PAN_SPEED;
}

void CEVID30::reqMaxPanSpeed(long &oSpeed)
{
    oSpeed = MAX_PAN_SPEED;
}

void CEVID30::reqMinAbsPan(long &oPos)
{
    oPos = MIN_ABS_PAN;
}

void CEVID30::reqMaxAbsPan(long &oPos)
{
    oPos = MAX_ABS_PAN;
}




// @CLASS
//    CEVID30
// @NAME
//    startTiltDown
//    startTiltUp
//    setTiltSpeed
//    reqTiltSpeed
//    reqTiltAbsPos
//    tiltAbsPosition
//    reqMinTiltSpeed
//    reqMaxTiltSpeed
//    reqMinAbsTilt
//    reqMaxAbsTilt
// @SYNOPSIS
//    int  sCEVID30::startTiltDown       ( void );
//    int  CEVID30::startTiltUp         ( void );
//    void  CEVID30::setTiltSpeed        ( const long& iTiltspeed );
//    void  CEVID30::reqTiltSpeed        (       long& oTiltSpeed );
//    void  CEVID30::reqTiltAbsPos       (       long& oTiltPos );
//    int  CEVID30::tiltAbsPosition     ( const long& iTiltPos );
//    void  CEVID30::reqMinTiltSpeed     (       long& oTiltSpeed );
//    void  CEVID30::reqMaxTiltSpeed     (       long& oTiltSpeed );
//    void  CEVID30::reqMinAbsTilt       (       long& oTiltPos );
//    void  CEVID30::reqMaxAbsTilt       (       long& oTiltPos );
// @DESCRIPTION
//    Tilting commands            
int CEVID30::startTiltDown(void)
{
    return FastExecute(DOWN[0], &DOWN[1]);
}

int CEVID30::startTiltUp(void)
{
    return FastExecute(UP[0], &UP[1]);
}

void CEVID30::setTiltSpeed(const long &iTiltSpeed)
{
    m_TiltSpeed = iTiltSpeed;
    LEFT[6] = RIGHT[6] = UP[6] = DOWN[6] = UPLEFT[6] = UPRIGHT[6] =
	DOWNLEFT[6] = DOWNRIGHT[6] = static_cast < char >(m_TiltSpeed);
    ABS_POSITION[6] = static_cast < char >(m_TiltSpeed);
}

void CEVID30::reqTiltSpeed(long &oTiltSpeed)
{
    oTiltSpeed = m_TiltSpeed;
}

////////////////////////////////////////////////////////////
int CEVID30::tiltAbsPosition(const long &iTiltPos)
{
    long pan_pos = 0;
    reqPanAbsPos(pan_pos);
    Long2FourBytes(pan_pos, &ABS_POSITION[7]);
    Long2FourBytes(iTiltPos, &ABS_POSITION[11]);
    return Execute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::FastTiltAbsPosition(const long &iTiltPos)
{
    long pan_pos = 0;
    reqPanAbsPos(pan_pos);
    Long2FourBytes(pan_pos, &ABS_POSITION[7]);
    Long2FourBytes(iTiltPos, &ABS_POSITION[11]);
    return FastExecute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
void CEVID30::reqTiltAbsPos(long &oTiltPos)
{				//int err = 0;
//  do 
//  { err = Inquiry(PAN_TILT_POS[0], &PAN_TILT_POS[1]);
//    if ( !err )
//      FourBytes2Long ( &m_Answer.message[5], m_TiltPosition );
//  } while (err);

    if (!(Inquiry(PAN_TILT_POS[0], &PAN_TILT_POS[1])))
	FourBytes2Long(&m_Answer.message[5], m_TiltPosition);
    else
	std::
	    cout << "[cc_evid.cpp] Error @ CEVID30::reqTiltAbsPos" << std::
	    endl;
    oTiltPos = m_TiltPosition;
//  std::cout << "oTiltPos : " << oTiltPos << std::endl;
}

////////////////////////////////////////////////////////////
void CEVID30::reqMinTiltSpeed(long &oSpeed)
{
    oSpeed = MIN_TILT_SPEED;
}

void CEVID30::reqMaxTiltSpeed(long &oSpeed)
{
    oSpeed = MAX_TILT_SPEED;
}

void CEVID30::reqMinAbsTilt(long &oPos)
{
    oPos = MIN_ABS_TILT;
}


void CEVID30::reqMaxAbsTilt(long &oPos)
{
    oPos = MAX_ABS_TILT;
}

























// @CLASS
//   CEVID30
// @NAME
//    stopZoom
//    startSlowWideZoom
//    startSlowTeleZoom
//    startFastWideZoom
//    startFastTeleZoom
//    reqAbsZoomPos
//    setAbsZoomPos
//    reqMinZoom
//    reqMaxZoom
//    setZoomSpeed
//    reqZoomSpeed
//    reqMinZoomSpeed
//    reqMaxZoomSpeed
// @SYNOPSIS
//    int CEVID30::stopZoom           ( void );
//    int CEVID30::startSlowWideZoom  ( void );
//    int CEVID30::startSlowTeleZoom  ( void );
//    int CEVID30::startFastWideZoom  ( void );
//    int CEVID30::startFastTeleZoom  ( void );
//    void  CEVID30::reqAbsZoomPos      (       long& oZoomPos );       
//    int CEVID30::setAbsZoomPos      ( const long  iZoomPos );
//    void  CEVID30::reqMinZoom         (       long& oZoomPos );
//    void  CEVID30::reqMaxZoom         (       long& oZoomPos );
//    void  CEVID30::setZoomSpeed       ( const long& iPanSpeed );
//    void  CEVID30::reqZoomSpeed       (       long& oPanSpeed );
//    void  CEVID30::reqMinZoomSpeed    (       long& oSpeed );
//    void  CEVID30::reqMaxZoomSpeed    (       long& oSpeed );
// @DESCRIPTION
//    Zoom Commands
int CEVID30::stopZoom(void)
{
    return FastExecute(STOP_ZOOM[0], &STOP_ZOOM[1]);
}

int CEVID30::startSlowWideZoom(void)
{
    return Execute(WIDE_STD[0], &WIDE_STD[1]);
}

int CEVID30::startSlowTeleZoom(void)
{
    return Execute(TELE_STD[0], &TELE_STD[1]);
}

int CEVID30::startFastWideZoom(void)
{
    return FastExecute(WIDE_VAR[0], &WIDE_VAR[1]);
}

int CEVID30::startFastTeleZoom(void)
{
    return FastExecute(TELE_VAR[0], &TELE_VAR[1]);
}

void CEVID30::reqAbsZoomPos(long &oZoomPos)
{
    this->Inquiry(ZOOM_POS[0], &ZOOM_POS[1]);
    FourBytes2Long(&m_Answer.message[1], oZoomPos);
}

int CEVID30::setAbsZoomPos(const long &iZoomPos)
{
    Long2FourBytes(iZoomPos, &ZOOM_DIRECT[5]);
    int err = Execute(ZOOM_DIRECT[0], &ZOOM_DIRECT[1]);
    return err;
}

int CEVID30::FastSetAbsZoomPos(const long &iZoomPos)
{
    Long2FourBytes(iZoomPos, &ZOOM_DIRECT[5]);
    int err = FastExecute(ZOOM_DIRECT[0], &ZOOM_DIRECT[1]);
    return err;
}

void CEVID30::reqMinZoom(long &oPos)
{
    oPos = MIN_ZOOM;
}

void CEVID30::reqMaxZoom(long &oPos)
{
    oPos = MAX_ZOOM;
}

void CEVID30::setZoomSpeed(const long &iZoomSpeed)
{
    m_ZoomSpeed = iZoomSpeed;
    TELE_VAR[5] = 0x20 | static_cast < char >(m_ZoomSpeed);
    WIDE_VAR[5] = 0x30 | static_cast < char >(m_ZoomSpeed);
}

void CEVID30::reqZoomSpeed(long &oZoomSpeed)
{
    oZoomSpeed = m_ZoomSpeed;
}

void CEVID30::reqMinZoomSpeed(long &oSpeed)
{
    oSpeed = MIN_ZOOM_SPEED;
}

void CEVID30::reqMaxZoomSpeed(long &oSpeed)
{
    oSpeed = MAX_ZOOM_SPEED;
}






















// @CLASS
//   CEVID30
// @NAME
//   stopPanTilt
//   moveHome
//   startGoDownleft
//   startGoDownright
//   startGoUpleft
//   startGoUpright
// @SYNOPSIS
//    int CEVID30::stopPanTilt         ( void );
//    int CEVID30::moveHome            ( void );
//    int CEVID30::startGoDownleft     ( void );
//    int CEVID30::startGoDownright    ( void );
//    int CEVID30::startGoUpleft       ( void );
//    int CEVID30::startGoUpright      ( void );
// @DESCRIPTION
//    Commands performing Pan and Tilt at the same time
int CEVID30::stopPanTilt(void)
{
    return FastExecute(STOP[0], &STOP[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::panTiltAbsPosition(const long &iPanPos, const long &iTiltPos)
{
    Long2FourBytes(iPanPos, &ABS_POSITION[7]);
    Long2FourBytes(iTiltPos, &ABS_POSITION[11]);
//  std::cout << "iPanPos : " << iPanPos << std::endl;
//  std::cout << "iTiltPos : " << iTiltPos << std::endl;
//  for ( int i = 0; i < ABS_POSITION[0]; i++ )
//    std::cout << hex << (int)ABS_POSITION[i+1] << " ";
//  std::cout << dec << std::endl;
    return Execute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::FastPanTiltAbsPosition(const long &iPanPos,
				    const long &iTiltPos)
{
    Long2FourBytes(iPanPos, &ABS_POSITION[7]);
    Long2FourBytes(iTiltPos, &ABS_POSITION[11]);
//  std::cout << "iPanPos : " << iPanPos << std::endl;
//  std::cout << "iTiltPos : " << iTiltPos << std::endl;
//  for ( int i = 0; i < ABS_POSITION[0]; i++ )
//    std::cout << hex << (int)ABS_POSITION[i+1] << " ";
//  std::cout << dec << std::endl;
    return FastExecute(ABS_POSITION[0], &ABS_POSITION[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::moveHome(void)
{
    return Execute(HOME[0], &HOME[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::startGoUpleft(void)
{
    return FastExecute(UPLEFT[0], &UPLEFT[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::startGoUpright(void)
{
    return FastExecute(UPRIGHT[0], &UPRIGHT[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::startGoDownleft(void)
{
    return FastExecute(DOWNLEFT[0], &DOWNLEFT[1]);
}

////////////////////////////////////////////////////////////
int CEVID30::startGoDownright(void)
{
    return FastExecute(DOWNRIGHT[0], &DOWNRIGHT[1]);
}

////////////////////////////////////////////////////////////























// @CLASS
//    CEVID30
// @NAME
//    setAutoFocus
//    setManualFocus
//    moveFocusCloser
//    moveFocusAway
//    reqAbsFocusPos
//    setAbsFocusPos
//    reqMinFocus
//    reqMaxFocus
//    switchAutoManFocus
//    stopFocus
// @SYNOPSIS
//    int CEVID30::setAutoFocus        ( void );
//    int CEVID30::setManualFocus      ( void );
//    int CEVID30::moveFocusCloser     ( void );
//    int CEVID30::moveFocusAway       ( void );
//    void  CEVID30::reqAbsFocusPos      (       long& oFocusPos );
//    int CEVID30::setAbsFocusPos      ( const long& iFocusPos );
//    void  CEVID30::reqMinFocus         (       long& oFocusPos );
//    void  CEVID30::reqMaxFocus         (       long& oFocusPos );
//    void  CEVID30::switchAutoManFocus  ( void );
//    int CEVID30::stopFocus           ( void );
// @DESCRIPTION
//    Focus Commands
int CEVID30::setAutoFocus(void)
{
    return Execute(AUTO_FOCUS_ON[0], &AUTO_FOCUS_ON[1]);
}

int CEVID30::setManualFocus(void)
{
    return Execute(MAN_FOCUS_ON[0], &MAN_FOCUS_ON[1]);
}

int CEVID30::moveFocusCloser(void)
{
    return Execute(NEAR[0], &NEAR[1]);
}

int CEVID30::moveFocusAway(void)
{
    return Execute(FAR[0], &FAR[1]);
}

void CEVID30::reqAbsFocusPos(long &oFocusPos)
{
    Inquiry(FOCUS_POS[0], &FOCUS_POS[1]);
    FourBytes2Long(&m_Answer.message[1], oFocusPos);
}

int CEVID30::setAbsFocusPos(const long &iFocusPos)
{
    Long2FourBytes(iFocusPos, &FOCUS_DIRECT[5]);
    return Execute(FOCUS_DIRECT[0], &FOCUS_DIRECT[1]);
}

void CEVID30::reqMinFocus(long &oFocusPos)
{
    oFocusPos = MIN_FOCUS;
}

void CEVID30::reqMaxFocus(long &oFocusPos)
{
    oFocusPos = MAX_FOCUS;
}

int CEVID30::switchAutoManFocus(void)
{
    return Execute(AUTO_MAN_FOCUS[0], &AUTO_MAN_FOCUS[1]);
}

int CEVID30::stopFocus(void)
{
    return Execute(STOP_FOCUS[0], &STOP_FOCUS[1]);
}






















// @CLASS
//    CEVID30
// @NAME
//    atModeOn
//    atModeOff
//    atModeOnOff
//    atAutoExposureOn
//    atAutoExposureOff
//    atAutoExposureOnOff
//    atAutoZoomOn
//    atAutoZoomOff
//    atAutoZoomOnOff
// @SYNOPSIS
//    int CEVID30::atModeOn            ( void );    
//    int CEVID30::atModeOff           ( void );    
//    int CEVID30::atModeOnOff         ( void );    
//    int CEVID30::atAutoExposureOn    ( void );    
//    int CEVID30::atAutoExposureOff   ( void );    
//    int CEVID30::atAutoExposureOnOff ( void );    
//    int CEVID30::atAutoZoomOn        ( void );    
//    int CEVID30::atAutoZoomOff       ( void );    
//    int CEVID30::atAutoZoomOnOff     ( void );    
// @DESCRIPTION
//    Target Traking Functions
int CEVID30::atModeOn(void)
{
    return Execute(AT_MODE_ON[0], &AT_MODE_ON[1]);
}

int CEVID30::atModeOff(void)
{
    return Execute(AT_MODE_OFF[0], &AT_MODE_OFF[1]);
}

int CEVID30::atModeOnOff(void)
{
    return Execute(AT_MODE_ONOFF[0], &AT_MODE_ONOFF[1]);
}

int CEVID30::atAutoExposureOn(void)
{
    return Execute(AT_AE_ON[0], &AT_AE_ON[1]);
}

int CEVID30::atAutoExposureOff(void)
{
    return Execute(AT_AE_OFF[0], &AT_AE_OFF[1]);
}

int CEVID30::atAutoExposureOnOff(void)
{
    return Execute(AT_AE_ONOFF[0], &AT_AE_ONOFF[1]);
}

int CEVID30::atAutoZoomOn(void)
{
    return Execute(AT_AUTO_ZOOM_ON[0], &AT_AUTO_ZOOM_ON[1]);
}

int CEVID30::atAutoZoomOff(void)
{
    return Execute(AT_AUTO_ZOOM_OFF[0], &AT_AUTO_ZOOM_OFF[1]);
}

int CEVID30::atAutoZoomOnOff(void)
{
    return Execute(AT_AUTOZOOM_ONOFF[0], &AT_AUTOZOOM_ONOFF[1]);
}




























// @CLASS
//    CEVID30
// @NAME
//    mdStartStop
//    mdModeOn
//    mdModeOff
//    mdModeOnOff
//    mdAdjustYlevel
//    mdAdjustHuelevel
//    mdAdjustSize
//    mdDisplayTime
//    mdRefreshMode1
//    mdRefreshMode2
//    mdRefreshMode3
//    mdRefreshTime
// @SYNOPSIS
//    int CEVID30::mdStartStop         ( void );
//    int CEVID30::mdModeOn            ( void );
//    int CEVID30::mdModeOff           ( void );
//    int CEVID30::mdModeOnOff         ( void );
//    int CEVID30::mdAdjustYlevel      ( const long& ylevel );
//    int CEVID30::mdAdjustHuelevel    ( const long& hue_level );
//    int CEVID30::mdAdjustSize        ( const long& size );
//    int CEVID30::mdDisplayTime       ( const long& display_time );
//    int CEVID30::mdRefreshMode1      ( void );
//    int CEVID30::mdRefreshMode2      ( void );
//    int CEVID30::mdRefreshMode3      ( void );
//    int CEVID30::mdRefreshTime       ( const long& refresh_time );
// @DESCRIPTION
//    Motion detection commands, exploit built-in motion detection 
//    capabilities of EVI-D30/D-31
//
//    mdStartStop starts or stops motion detection or tracking
//    mdModeOn turns detector mode on 
//    mdModeOff turns detector mode off
//    mdModeOnOff switches detector mode or off
//    mdAdjustYlevel
//    mdAdjustHuelevel
//    mdAdjustSize
//    mdDisplayTime
//    mdRefreshMode1
//    mdRefreshMode2
//    mdRefreshMode3
//    mdRefreshTime
int CEVID30::mdStartStop(void)
{
    return Execute(MD_START_STOP[0], &MD_START_STOP[1]);
}

int CEVID30::mdModeOn(void)
{
    return Execute(MD_MODE_ON[0], &MD_MODE_ON[1]);
}

int CEVID30::mdModeOff(void)
{
    return Execute(MD_MODE_OFF[0], &MD_MODE_OFF[1]);
}

int CEVID30::mdModeOnOff(void)
{
    return Execute(MD_MODE_ONOFF[0], &MD_MODE_ONOFF[1]);
}

int CEVID30::mdAdjustYlevel(const long &ylevel)
{
    MD_ADJUST_Y[6] = static_cast < char >(ylevel);
    return Execute(MD_ADJUST_Y[0], &MD_ADJUST_Y[1]);
}

int CEVID30::mdAdjustHuelevel(const long &hue_level)
{
    MD_ADJUST_HUE[6] = static_cast < char >(hue_level);
    return Execute(MD_ADJUST_HUE[0], &MD_ADJUST_HUE[1]);
}

int CEVID30::mdAdjustSize(const long &size)
{
    MD_ADJUST_SIZE[6] = static_cast < char >(size);
    return Execute(MD_ADJUST_SIZE[0], &MD_ADJUST_SIZE[1]);
}

int CEVID30::mdDisplayTime(const long &display_time)
{
    MD_ADJUST_D_TIME[6] = static_cast < char >(display_time);
    return Execute(MD_ADJUST_D_TIME[0], &MD_ADJUST_D_TIME[1]);
}

int CEVID30::mdRefreshMode1(void)
{
    return Execute(MD_REFR_MODE_1[0], &MD_REFR_MODE_1[1]);
}

int CEVID30::mdRefreshMode2(void)
{
    return Execute(MD_REFR_MODE_2[0], &MD_REFR_MODE_2[1]);
}

int CEVID30::mdRefreshMode3(void)
{
    return Execute(MD_REFR_MODE_3[0], &MD_REFR_MODE_3[1]);
}

int CEVID30::mdRefreshTime(const long &refresh_time)
{
    MD_ADJUST_R_TIME[6] = static_cast < char >(refresh_time);
    return Execute(MD_ADJUST_R_TIME[0], &MD_ADJUST_R_TIME[1]);
}















////////////////////////////////////////////////////////////////////////////////
//
//        Some private help functions
//
////////////////////////////////////////////////////////////////////////////////
int CEVID30::Execute(char &length, char *comm)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::Execute" << std::endl;
//  int err = 0;
//  if ( (err = this->Receive()) )

    if (write(m_Handle, comm, length) < 0) {
	perror("CEVID30::Execute ");
	return -1;
    }

    ////  old version:
    return this->Receive();
}




int CEVID30::FastExecute(char &length, char *comm)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::FastExecute" << std::endl;
    if (write(m_Handle, comm, length) < 0) {
	perror("CEVID30::Execute ");
	return -1;
    }
//  std::cout << "[cc_evid.cpp] leaving CEVID30::FastExecute\n" << std::endl;
    return 0;
}














////////////////////////////////////////////////////////////
int CEVID30::Inquiry(char &length, char *comm)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::Inquiry" << std::endl;
    int err = 0;

    ////  if both sockets in the camera are used, wait until
    //    the previous commands are completed
/*
    std::cout << "m_CameraSockets[0] = " << m_CameraSockets[0] << std::
	endl;
    std::cout << "m_CameraSockets[1] = " << m_CameraSockets[1] << std::
	endl;

*/

    while (m_CameraSockets[0] && m_CameraSockets[1])
	err = this->Receive();
/*

    std::cout << "m_CameraSockets[0] = " << m_CameraSockets[0] << std::
	endl;
    std::cout << "m_CameraSockets[1] = " << m_CameraSockets[1] << std::
	endl;
*/
    //   alternatively, we could cancel a command (the last one?)



    if (write(m_Handle, comm, length) < 0) {
	perror("CEVID30::Inquiry ");
	return -1;
    }


    err = this->Receive();


    return err;
}












////  generic receive answer function  /////////////////////
int CEVID30::Receive(CEVID30ans *, const int &)
{
    return this->Receive();
}

int CEVID30::Receive(const int &)
{
    return this->Receive();
}

////////////////////////////////////////////////////////////

//  man koennte die erwartete antwortlaenge passieren

int CEVID30::Receive(void)
{				//std::cout << "\n[cc_evid.cpp] entering CEVID30::Receive" << std::endl;
    int err = 0;
    int idxm = 0, cnt = 1;
    unsigned char retc = 0;

    m_Answer.Reset();


    while (cnt) {		////  read from serial line  /////////


	if ((cnt = read(m_Handle, &retc, sizeof(unsigned char))) < 0) {
	    perror("CEVID30::Receive ");
	    return -1;
	}


	////  evaluate buffer content  ///////
	if ((retc == m_RecvAddress) || (retc == 0x88)) {
	    m_Answer.header = retc;
	} else if (retc != 0xFF) {
	    m_Answer.message[idxm++] = retc;
	}

	else if (retc == 0xFF) {
	    m_Answer.terminator = retc;
	    idxm = 0;
	    switch (m_Answer.message[0]) {
	    case 0x41:
		m_CameraSockets[0] = true;
		err = 0;
		break;
	    case 0x42:
		m_CameraSockets[1] = true;
		err = 0;
		break;
	    case 0x51:
		m_CameraSockets[0] = false;
		err = 0;
		break;
	    case 0x52:
		m_CameraSockets[1] = false;
		err = 0;
		break;

	    case 0x60:
		switch (m_Answer.message[1]) {
		case 0x02:
		    err = -1;
		    break;
		case 0x03:
#ifdef __DEBUG_LEVEL2__
		    if (!(m_CameraSockets[0]) || !(m_CameraSockets[1])) {
			std::
			    cout << "hier stimmt was nicht mit'm code" <<
			    std::endl;
//                        exit ( EXIT_FAILURE ); 
		    }
#endif
		    err = -1;
		    break;
		case 0x04:
		    err = -1;
		    break;
		case 0x05:
		    err = -1;
		    break;
		case 0x41:
		    err = -1;
		    break;
		default:
		    std::cerr << "caught unknown error!" << std::endl;
		    std::cerr << m_Answer;
		    exit(EXIT_FAILURE);
		}

	    case 0x50:
		err = 0;
		break;

	    case 0x30:		//std::cout << m_Answer;
		m_Address = m_Answer.message[1] - 1;
		err = 0;
		break;

	    case 0x38:
//                    std::cout << m_Address; 
		err = 0;
		break;
	    case 0x01:		//  let's hope that this is a ifClear command
		err = 0;
		break;

	    default:
#ifdef __DEBUG_LEVEL2__
		std::cerr << "Invalid message caught" << std::endl;
		std::cerr << m_Answer;
#endif
//                    exit ( EXIT_FAILURE ); 
		break;
	    }
	}			//  if ( retc == 0xFF )
    }
//  std::cout << "[cc_evid.cpp] leaving CEVID30::Receive\n" << std::endl;



    return err;
}
