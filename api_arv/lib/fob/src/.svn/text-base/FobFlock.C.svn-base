///////////////////////////////////////////////////////////////////////////////
// File name : FobFlock.C
//
// Creation : 20??/??/??
//
// Version : 20??/??/??
//
// Author : Raphael Grasset
//
// email : Raphael.Grasset@imag.fr
//
// Purpose : ??
//
// Distribution :
//
// Use :
//	??
//
// Todo :
//	O ??
//	/
//	X
//
// History :
//	20??/??/?? : Mr Grasset : Creation of the file
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

#include "FobFlock.h"

///////////////////////////////////////////////////////////////////////////////
// Macro 
///////////////////////////////////////////////////////////////////////////////

#if defined(NO_DEBUG)
#define ASSERT(x)
#else //defined(NO_DEBUG)
#define ASSERT(x) if(!(x)) \
    { cerr << "Assertion failed : (" << #x << ')' << endl \
    << "In file : " << __FILE__ << "at line #" << __LINE__ << endl \
    << "Compiled the " << __DATE__ << " at " << __TIME__ << endl; abort();}
#endif // else defined(NO_DEBUG)

const char* const FobFlock_RCS_ID = "@(#)class FobFlock definition.";

///////////////////////////////////////////////////////////////////////////////
// class FobFlock
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Static variable
///////////////////////////////////////////////////////////////////////////////

const int FLOCK_BROADCAST=0x1f;
#define ITCM	(float)2.54

///////////////////////////////////////////////////////////////////////////////
// PUBLIC: Standard services 
///////////////////////////////////////////////////////////////////////////////

FobFlock::FobFlock(Fob_RData type,Fob_HData hemisphere,const char
*device_file,RS232_Mode mode_flux,int nb_fob):FobCommand(device_file,mode_flux)
{
    mode=type;
    hem=hemisphere;
    scale_x=scale_y=scale_z=1.0;
    pos_x=pos_y=pos_z=0.0; 
    nbFob=nb_fob;
}

FobFlock::FobFlock(const FobFlock &)
{
    
}

FobFlock::~FobFlock(void)
{
    SetRData(POSITION_ANGLES);
}

FobFlock& 
FobFlock::operator=(const FobFlock &)
{
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC : Interface 
///////////////////////////////////////////////////////////////////////////////

void 
FobFlock::SelfDisplay(ostream& stream) const
{
    stream << "< class FobFlock >" << endl;
}

bool 
FobFlock::OK(void) const
{
    return true;
}

void
FobFlock::Init()
{   
    init();
    clear();
    //   GetConfig();
    // on se place dans le modele graphique de base x a droite y vers le haut z vers nous
    for (int i=1;i<=nbFob;i++)
    {
	ToFbb(i);
	if (i==1) //just for the first flock
	    SetButtonMode(ON);
	SetPositionReferenceFrame(ON);
	ToFbb(i);
	SetReferenceFrame(-90.0,0.0,-90.0);
	ToFbb(i);
	SetHemisphere(RotateHem(hem,i-1));   
	ToFbb(i);
	SetAngleAlign(180.0,-90.0,-90.0);
	ToFbb(i);
	SetRData(mode);//fix data type
    }
    SetAutoConfiguration(nbFob);
    sleep(1);
}

void 
FobFlock::SetOrientation(float azimuth,float elevation,float roll,int number)
{
    ToFbb(number);
    SetAngleAlign(azimuth,elevation,roll);
    sleep(1);
}


Fob_HData 
FobFlock::RotateHem(Fob_HData h,int number)
{
    
    Fob_HData res;
    switch (h)
    {
    case FORWARD:res=LOWER;axis_1[number]=0;axis_2[number]=1;break;
    case AFT:res=UPPER;axis_1[number]=0;axis_2[number]=1;break;
    case UPPER:res=RIGHT;axis_1[number]=0;axis_2[number]=2;break;
    case LOWER:res=LEFT;axis_1[number]=0;axis_2[number]=2;break;
    case LEFT:res=FORWARD;axis_1[number]=1;axis_2[number]=2;break;
    case RIGHT:res=AFT;axis_1[number]=1;axis_2[number]=2;break;
    }
    return res;
}

void
FobFlock::RunDriver()
{
    SetGroupMode(ON);
    Stream();
}

void
FobFlock::StopDriver()
{
    //ToFbb(1);
    Point();
    clear();
    SetGroupMode(OFF);
}

void 
FobFlock::GetSPosQuaternion(float pos[3],float quat[4],int number)
{
// 	pthread_mutex_lock(&mutex);
    //le lock se fait avec FobPerf

    //   FobCommand::GetPosQuaternionF(shared_pos,shared_quat);
    pos[0]=shared_pos[0+3*number]*ITCM*scale_x+pos_x;
    pos[1]=shared_pos[1+3*number]*ITCM*scale_y+pos_y;
    pos[2]=shared_pos[2+3*number]*ITCM*scale_z+pos_z;
    quat[0]=shared_quat[1+4*number];
    quat[1]=shared_quat[2+4*number];
    quat[2]=shared_quat[3+4*number];
    quat[3]=-shared_quat[0+4*number];
// 	pthread_mutex_unlock(&mutex);

}

void
FobFlock::GetSButton(int* button)
{
    *button=shared_button;
}

bool
FobFlock::IsLeftPressed()
{
return (FobCommand::IsLeftPressed(shared_button));
}

bool
FobFlock::IsMiddlePressed()
{
return (FobCommand::IsMiddlePressed(shared_button));
}

bool
FobFlock::IsRightPressed()
{
return (FobCommand::IsRightPressed(shared_button));
}


///////////////////////////////////////////////////////////////////////////////
// PROTECTED : Services
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// PRIVATE : Services
///////////////////////////////////////////////////////////////////////////////



