//////////////// ///////////////////////////////////////////////////////////////
// File name : FobFlockPerf.C
//
// Creation : 20
//
// Version : 2001/01/25
//
// Author : Raphael Grasset
//
// email : Raphael.Grasset@imag.fr
//
// Purpose : implement the FobFlock for Performer with sproc.
//
// Distribution :
//
// Use :
//	show the sample
//
// Todo :
//	O a virtual fobflock
//	/
//	X
//
// History :
//	2001/01/25 : Mr Grasset : Creation of the file
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

#include "FobFlockPerf.h"

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

const char* const FobFlockPerf_RCS_ID = "@(#)class FobFlockPerf definition.";

///////////////////////////////////////////////////////////////////////////////
// class FobFlockPerf
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Static variable
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// PUBLIC: Standard services 
///////////////////////////////////////////////////////////////////////////////

FobFlockPerf::FobFlockPerf(int nb_fob)
{
    nbFob=nb_fob;
    signe[0]=1.;
    signe[1]=1.;
    fobType=SERIAL_SOFT;
    serialPort=1;
    nbMeasure=0;
}


FobFlockPerf::FobFlockPerf(const FobFlockPerf &)
{
    cerr<<"not yet implemented !!"<<endl;
}

FobFlockPerf::~FobFlockPerf(void)
{
    
}

FobFlockPerf& 
FobFlockPerf::operator=(const FobFlockPerf &)
{
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC : Interface 
///////////////////////////////////////////////////////////////////////////////

void 
FobFlockPerf::SelfDisplay(ostream& stream) const
{
   stream << "< class FobFlockPerf >" << endl;
   stream << "Fob Number= "<<nbFob<<endl;
   stream << "Serial Port="<< serialPort<<endl;
   stream << *fob <<endl;
}

bool 
FobFlockPerf::OK(void) const
{
    return true;
}

void 
FobFlockPerf::Set(SerialType type,int serial)
{
    fobType=type;
    serialPort=serial;
}

void
FobFlockPerf::SetPen(int number)
{
    fob->SetOrientation(180.,-90.,0.,number+1);
}

void
FobFlockPerf::SetIGlasses(int number)
{
    fob->SetOrientation(180.,90.,0.,number+1);
}

void 
FobFlockPerf::Init()
{

 const char*convert[]={"0","1","2","3","4","5","6","7","8"}; 
  
	char * nameport; 
	nameport=new char[strlen("/dev/ttyx")+1];
	if (fobType==SERIAL_HARD)
	{
	    strcpy(nameport,"/dev/ttyf");
	    strcat(nameport,convert[serialPort]);
	    fob=new FobFlock(POSITION_QUATERNION,FORWARD,nameport,CONTROL_HARD,nbFob);
	}
	else
	{
	    strcpy(nameport,"/dev/ttyd");
	    strcat(nameport,convert[serialPort]);
	    fob=new FobFlock(POSITION_QUATERNION,FORWARD,nameport,CONTROL_SOFT,nbFob);	
	}
	delete[] nameport;
	fob->Init();      
}

void 
FobFlockPerf::Start()
{
    char *file = mktemp("/var/tmp/flockXXXXXX");
    usptr_t *arena = usinit(file);
    
    //init lock
    for (int i=0;i<nbFob;i++)
    {
	if (!(lock[i]= usnewlock(arena)))
	    exit(0); 
	if (!uscsetlock(lock[i],1))
	    exit(0);
	usunsetlock(lock[i]);
    }
    if (!(buttonlock= usnewlock(arena)))
	exit(0); 
    if (!uscsetlock(buttonlock,1))
	exit(0); 
    usunsetlock(buttonlock);
    
    if ( (idServer = sproc(&FobFlockPerf::RunProc,PR_SADDR | PR_SFDS,(void *)this)) == -1)
    {
        cerr << "ERROR->FLOCKFOBPERF/START: Unable to create new process for flock of birds" << endl;
        exit(1);
    }
    
}

void 
FobFlockPerf::Stop()
{
    // for (int i=0;i<nbFob;i++)
    fob->StopDriver();
    kill(idServer,SIGKILL);
    cerr << "NOTIFY->>FLOCKFOBPERF/STOP: stop tracking process ("<<nbMeasure<<")"<<".."<<endl;
}

void 
FobFlockPerf::Close()
{

}

void 
FobFlockPerf::GetPosQuat(pfVec3 &posv,pfQuat &quatv,int number)
{
	static float pos[3];
	static float quat[4];

	ussetlock(lock[number]);
	//cerr<<"lock client "<<number<<endl;
	fob->GetSPosQuaternion(pos,quat,number);
	usunsetlock(lock[number]);
	//cerr<<"unlock client "<<number<<endl;

	posv.set(pos[0],-pos[2],pos[1]);
	quatv.set(quat[0],-quat[2],quat[1],quat[3]);
}


bool
FobFlockPerf::GetButton(int number)
{
    bool button_press;
    ussetlock(buttonlock);
    if (number==1)
	button_press=fob->IsLeftPressed();
    else
	if (number==2)
	    button_press=fob->IsMiddlePressed();
	else
	    button_press=fob->IsRightPressed();

    usunsetlock(buttonlock);
    return button_press;
}


void 
FobFlockPerf::GetPos(pfVec3 &posv,int number)
{
    pfQuat quatv;
    GetPosQuat(posv,quatv,number);
}

void 
FobFlockPerf::GetQuat(pfQuat &quatv,int number)
{
    pfVec3 posv;
    GetPosQuat(posv,quatv,number);
}


void 
FobFlockPerf::Run()
{
    static float pos[6],quat[8];
    static float posf[6],quatf[8];
    static int button;
    cerr<<"NOTIFY->FLOCKFOBPERF/RUN:start tracking process.."<<endl;
    
    //  fob->SetFilter(FILTER_ON,FILTER_OFF,FILTER_ON);
    fob->RunDriver();
    	   
    while (1)
    {	
	//fob->FobCommand::GetPosQuaternionF(pos,quat);//get value
	fob->FobCommand::GetPosQuaternionBF(pos,quat,&button);//get value

	ussetlock(buttonlock);
	fob->shared_button=button;
	//cerr<<"value from the bird"<<button<<endl;
	usunsetlock(buttonlock);  
	for (int i=0;i<nbFob;i++)
  	{
//         	//hemisphere change test 
	    if ((Signe(lastPos[fob->axis_1[i]+i*3])==-Signe(pos[fob->axis_1[i]+i*3]))&&(Signe(lastPos[fob->axis_2[i]+i*3])==-Signe(pos[fob->axis_2[i]+i*3])))
		signe[i]=-signe[i];
	    
	    //update hemisphere
	    posf[0]=signe[i]*pos[0+i*3];
	    posf[1]=signe[i]*pos[1+i*3];
	    posf[2]=signe[i]*pos[2+i*3];				
	    quatf[0]=quat[0+i*4];
	    quatf[1]=quat[1+i*4];
	    quatf[2]=quat[2+i*4];
	    quatf[3]=quat[3+i*4];		
	    
//pass to shared buffer
	    ussetlock(lock[i]);
	    //cerr<<"lock  server "<<i<<endl; 
	    fob->shared_pos[0+3*i]=posf[0];
	    fob->shared_pos[1+3*i]=posf[1];
	    fob->shared_pos[2+3*i]=posf[2];
	    fob->shared_quat[0+4*i]= quatf[0];
	    fob->shared_quat[1+4*i]= quatf[1];
	    fob->shared_quat[2+4*i]= quatf[2];
	    fob->shared_quat[3+4*i]= quatf[3];
	    usunsetlock(lock[i]);
	    //cerr<<"unlock  server "<<i<<endl; 
	//update last value
	    lastPos[0+i*3]=pos[0+i*3];
	    lastPos[1+i*3]=pos[1+i*3];
	    lastPos[2+i*3]=pos[2+i*3];
  	}
	nbMeasure++;
    }
}

void 
FobFlockPerf::RunProc(void *data)
{
    FobFlockPerf *fp = (FobFlockPerf *)data;
    fp->Run();
}

int
FobFlockPerf::Signe(float valeur)
{
	if (valeur<0.0)
		return -1;
	else
		return 1;
}

void 
FobFlockPerf::InfoPosQuat(int number)
{
	static float pos[3];
	static float quat[4];
	ussetlock(lock[number]);
	fob->GetSPosQuaternion(pos,quat,number);
	usunsetlock(lock[number]);
	cout<<"NOTIFY->FLOCKOBPERF/INFOPOSQUAT:(";
	cout<<pos[0]<<","<<-pos[2]<<","<<pos[1]<<")";
	cout<<"("<<quat[0]<<","<<-quat[2]<<","<<quat[1]<<","<<quat[3]<<")"<<endl;	
	cout.flush();
}




///////////////////////////////////////////////////////////////////////////////
// PROTECTED : Services
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// PRIVATE : Services
///////////////////////////////////////////////////////////////////////////////
