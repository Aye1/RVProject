// $Id: cc_evid.hpp,v 1.4 2000/06/20 16:31:40 schwerdt Exp $
// $Log: cc_evid.hpp,v $
//


#ifndef __EVID30__
#define __EVID30__




#define _OLD_TERMIOS      // to make the code compatible to IRIX 6
#include <termios.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __Linux__
#include <sys/ioctl.h>    // contains ioctl() on Linux
#include <sys/ttydefaults.h>
#include <asm/ioctls.h>
#endif

#include <unistd.h>       // contains ioctl() on IRIX
#include <fcntl.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>



#define SERIAL1  "/dev/ttyS0"
#define SERIAL2  "/dev/ttyS1"





////  answer struct 
/////////////////////////////////////////////////////////// 
class CEVID30ans
{ 
friend class CEVID30;

  //////////////////////////////////////////////////////////
public:
  inline          CEVID30ans          ( void );
  inline          CEVID30ans          ( const CEVID30ans& );

  inline
  CEVID30ans&     operator=           ( const CEVID30ans& );

  //////////////////////////////////////
  inline void     Set                 ( unsigned char* i_msg = 0,
                                        const int& i_msg_len = 0 );
  inline void     Reset               ( void );


  inline void     Write               ( std::ostream& ostr ) const;

  //////////////////////////////////////////////////////////
private:
  unsigned char   header;             
  unsigned char   message[14];      
  unsigned char   terminator;       
  int             message_length;       
};

////////////////////////////////////////////////////////////
inline CEVID30ans::CEVID30ans ( void )  { this->Reset(); }
inline CEVID30ans::CEVID30ans ( const CEVID30ans& ans )  { *this = ans; }

inline CEVID30ans& CEVID30ans::operator= ( const CEVID30ans& i_ans )
{ if ( this != &i_ans ) {
  header          = i_ans.header;
  memcpy ( message, i_ans.message, 14 * sizeof(char) );
  terminator      = i_ans.terminator;
  message_length  = i_ans.message_length; 
  } return *this; 
}

inline void CEVID30ans::Set ( unsigned char* i_msg, const int& i_msg_len )
{ memset ( (void*)&header, 0, 16 );
  message_length = i_msg_len;
  if ( i_msg && !(message_length > 16) )
    memcpy ( &header, i_msg, message_length ); }

inline void CEVID30ans::Reset ( void )
{ this->Set(); }

inline void CEVID30ans::Write ( std::ostream& ostr ) const
{ ostr << "Header\t\t:\t" << std::hex << (int)header << std::endl;
  ostr << "message\t\t:\t";
  for (long i = 0; i < 14; i++)
    ostr << std::hex << (int)message[i] << "  ";
  ostr << std::endl << "Terminator\t:\t" << std::hex << (int)terminator << std::endl;
  ostr << "M-Length\t:\t" << std::dec << message_length << std::endl << std::endl; }

inline std::ostream& operator<< ( std::ostream& s, const CEVID30ans& i_ea )
{ i_ea.Write(s); s.flush(); return s; }










////////////////////////////////////////////////////////////////////////////////
//  functions override  functions in CCamCommand
class CEVID30 
{
////////////////////////////////////////////////////////////
public:
  inline          CEVID30             ( const long& i_line_num = 0 );
  inline          CEVID30             ( const CEVID30& );
   inline  ~CEVID30            ( void );

  //////////////////
  CEVID30&        operator=           ( const CEVID30& );


  //////////////////////////////////////
   int    Init                ( const long& i_line_num );


  ////  general functions  /////////////
  int            addressSet          ( void );
  int            ifClear             ( void );
   int    commandCancel       ( const int& i_socket = 0 );
  ////  
   int    turnPowerOn         ( void );
   int    turnPowerOff        ( void );
   int    cameraReset         ( void );

   int    backLightOn         ( void );
   int    backLightOff        ( void );

  int            dataScreenOn        ( void );
  int            dataScreenOff       ( void );
  int            dataScreenOnOff     ( void );
    
  // the following 6 commands stem from "the VCC1 library"; 
  // they have been left for compatibility reasons
   int    ready               ( void );
   int    idle                ( void );
  inline  
   int    initialize1         ( void ) { return this->cameraReset(); }
  inline  
   int    initialize2         ( void ) { return this->cameraReset(); }
  inline  
   char*   reqNameOfModel      ( char* = 0 );
   int    reqOpStatus         ( char*& );


  //////////////////////////////////////
  void            reqExposureMode     ( long& );
  int            setFullAutoMode     ( void );
  int            setManualMode       ( void );
  int            setShutterPriority  ( void );
  int            setIrisPriority     ( void );
  int            setBrightMode       ( void );

  int            brightnessReset     ( void );
  int            setBrightnessUp     ( void );
  int            setBrightnessDown   ( void );

  void            reqShutterPosition  ( long& );
  int            shutterReset        ( void );
  int            setShutterUp        ( void );
  int            setShutterDown      ( void );
  int            setShutterPosition  ( const long& );

  void            reqIrisPosition     ( long& );
  int            irisReset           ( void );
  int            setIrisUp           ( void );
  int            setIrisDown         ( void );
  int            setIrisPosition     ( const long& );

  void            reqGainPosition     ( long& );
  int            gainReset           ( void );
  int            setGainUp           ( void );
  int            setGainDown         ( void );
  int            setGainPosition     ( const long& );

  ////  Pan  ///////////////////////////
   int    startPanRight       ( void );
   int    startPanLeft        ( void );
   void    setPanSpeed         ( const long& iPanSpeed );
   void    reqPanSpeed         (       long& oPanSpeed );
   void    reqPanAbsPos        (       long& oPanPos );
   int    panAbsPosition      ( const long& iPanPos );
   int    FastPanAbsPosition  ( const long& iPanPos );
   void    reqMinPanSpeed      (       long& oPanSpeed );
   void    reqMaxPanSpeed      (       long& oPanSpeed );
   void    reqMinAbsPan        (       long& oPanPos );
   void    reqMaxAbsPan        (       long& oPanPos );

  ////  Tilt  //////////////////////////
   int    startTiltDown       ( void );
   int    startTiltUp         ( void );
   void    setTiltSpeed        ( const long& iTiltspeed );
   void    reqTiltSpeed        (       long& oTiltSpeed );
   void    reqTiltAbsPos       (       long& oTiltPos );
   int    tiltAbsPosition     ( const long& iTiltPos );
   int    FastTiltAbsPosition ( const long& iTiltPos );
   void    reqMinTiltSpeed     (       long& oTiltSpeed );
   void    reqMaxTiltSpeed     (       long& oTiltSpeed );
   void    reqMinAbsTilt       (       long& oTiltPos );
   void    reqMaxAbsTilt       (       long& oTiltPos );

  ////  Zoom  //////////////////////////
   int    stopZoom            ( void );
   int    startSlowWideZoom   ( void );
   int    startSlowTeleZoom   ( void );
   int    startFastWideZoom   ( void );
   int    startFastTeleZoom   ( void );
   void    reqAbsZoomPos       (       long& oZoomPos );       
   int    setAbsZoomPos       ( const long& iZoomPos );
   int    FastSetAbsZoomPos   ( const long& iZoomPos );
   void    reqMinZoom          (       long& oZoomPos );
   void    reqMaxZoom          (       long& oZoomPos );
   void    setZoomSpeed        ( const long& iZoomSpeed );
   void    reqZoomSpeed        (       long& oZoomSpeed );
   void    reqMinZoomSpeed     (       long& oZoomSpeed );
   void    reqMaxZoomSpeed     (       long& oZoomSpeed );
  
  ////  Pan & Tilt  ////////////////////
   int    stopPanTilt         ( void );
   int    panTiltAbsPosition  ( const long& iPanPos,
                                        const long& iTiltPos );
   int    FastPanTiltAbsPosition( const long& iPanPos,
                                          const long& iTiltPos );
   int    moveHome            ( void );
   int    startGoDownleft     ( void );
   int    startGoDownright    ( void );
   int    startGoUpleft       ( void );
   int    startGoUpright      ( void );
          
  ////  Focus  /////////////////////////
   int    setAutoFocus        ( void );
   int    setManualFocus      ( void );
   int    moveFocusCloser     ( void );
   int    moveFocusAway       ( void );
   void    reqAbsFocusPos      (       long& oFocusPos );
   int    setAbsFocusPos      ( const long& iFocusPos );
   void    reqMinFocus         (       long& oFocusPos );
   void    reqMaxFocus         (       long& oFocusPos );
   int    switchAutoManFocus  ( void );
   int    stopFocus           ( void );

  ////  Target Tracking  ///////////////
  int            atModeOn            ( void );    
  int            atModeOff           ( void );    
  int            atModeOnOff         ( void );    

  int            atAutoExposureOn    ( void );    
  int            atAutoExposureOff   ( void );    
  int            atAutoExposureOnOff ( void );    

  int            atAutoZoomOn        ( void );    
  int            atAutoZoomOff       ( void );    
  int            atAutoZoomOnOff     ( void );    
  
  ////  Motion Detection  //////////////
  int            mdStartStop         ( void );
  int            mdModeOn            ( void );
  int            mdModeOff           ( void );
  int            mdModeOnOff         ( void );
  int            mdAdjustYlevel      ( const long& ylevel );
  int            mdAdjustHuelevel    ( const long& hue_level );
  int            mdAdjustSize        ( const long& size );
  int            mdDisplayTime       ( const long& display_time );
  int            mdRefreshMode1      ( void );
  int            mdRefreshMode2      ( void );
  int            mdRefreshMode3      ( void );
  int            mdRefreshTime       ( const long& refresh_time );
  

  //////////////////////////////////////////////////////////
private:
  int            Execute             ( char& length, char* comm);
  int            FastExecute         ( char& length, char* comm );
  int            Inquiry             ( char& length, char* comm);
  int            Receive             ( void );
  int            Receive             ( const int& in_handle );
  int            Receive             ( CEVID30ans*, const int& in_handle );
  int            EvaluateAnswer      ( void );
//  inline int     Execute             ( char& length, char* comm);
//  inline int     Inquiry             ( char& length, char* comm);
//  inline int     Receive             ( const int& in_handle);
//  inline int     Receive             ( CEVID30ans*, const int& in_handle);
//  inline int     EvaluateAnswer      ( void );


  //////////////////////////////////////////////////////////
private :
  int             m_Handle;

  CEVID30ans      m_Answer;
  bool            m_CameraSockets[2];   //  sockets in camera busy?
  unsigned char   m_Address;            //  camera address for sending commands
  unsigned char   m_RecvAddress;        //  camera address for answers
  ////
  long            m_PanSpeed;
  long            m_TiltSpeed;
  long            m_ZoomSpeed;
  ////
  // die naechsten 3 sind wohl nicht noetig ...
  long            m_PanPosition;
  long            m_TiltPosition;
  long            m_ZoomPosition;
  ////
  const char*     MODEL_NAME;
};


////////////////////////////////////////////////////////////
inline CEVID30::CEVID30 ( const long& i_line_num ) 
  : m_Address(0), m_RecvAddress(0),
    m_PanSpeed(0), m_TiltSpeed(0), m_ZoomSpeed(0),
    m_PanPosition(0), m_TiltPosition(0), m_ZoomPosition(0), 
    MODEL_NAME("EVID-30")
{ m_CameraSockets[0] = false;  m_CameraSockets[1] = false;
  if ( i_line_num ) this->Init (i_line_num);  }

inline CEVID30::CEVID30(const CEVID30& evid )
  : MODEL_NAME("EVID-30")
{ *this = evid; }

inline CEVID30::~CEVID30( void ) {}

////////////////////
inline char* CEVID30::reqNameOfModel ( char* o_str )
{ if ( o_str ) strcpy ( o_str, MODEL_NAME ); return (char*)MODEL_NAME; }

















#endif

