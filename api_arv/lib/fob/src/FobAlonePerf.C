///////////////////////////////////////////////////////////////////////////////
// File name : AloneFobPerf.C
//
// Creation : 2000/11/14
//
// Version : 2000/11/14
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
//	2000/11/14 : Mr Grasset : Creation of the file
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

#include "FobAlonePerf.h"

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

const char* const XXX_RCS_ID = "@(#)class FobAlonePerf definition.";

///////////////////////////////////////////////////////////////////////////////
// class XXX
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Static variable
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// PUBLIC: Standard services 
///////////////////////////////////////////////////////////////////////////////

FobAlonePerf::FobAlonePerf(int nb_flock)
{
    nbFob=nb_flock; 
    nbMeasure=0;
    for (int i=0;i<nbFob;i++)
    {
	signe[i]=1.0;
	fobType[i]=SERIAL_SOFT;//ttyd by default
	serialPort[i]=i+1;//0 to 1 , 1 to 2
    }
}

FobAlonePerf::FobAlonePerf(const FobAlonePerf &)
{
    
}

FobAlonePerf::~FobAlonePerf(void)
{
    
}

FobAlonePerf& 
FobAlonePerf::operator=(const FobAlonePerf &)
{
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
// PUBLIC : Interface 
///////////////////////////////////////////////////////////////////////////////

void 
FobAlonePerf::SelfDisplay(ostream& stream) const
{
   stream << "< class FobAlonePerf >" << endl;
}

bool 
FobAlonePerf::OK(void) const
{
    return true;
}

void 
FobAlonePerf::Set(SerialType type,int serial,int number)
{
    fobType[number]=type;
    serialPort[number]=serial;
}

void
FobAlonePerf::SetPen(int number)
{
    fob[number]->SetOrientation(180.,-90.,0.);
}

void
FobAlonePerf::SetIGlasses(int number)
{
    fob[number]->SetOrientation(180.,90.,0.);
}

void 
FobAlonePerf::Init()
{

 const char*convert[]={"0","1","2","3","4","5","6","7","8"}; 
    for (int i=0;i<nbFob;i++)
    {
	char * nameport; 
	nameport=new char[strlen("/dev/ttyx")+1];
	if (fobType[i]==SERIAL_HARD)
	{
	    strcpy(nameport,"/dev/ttyf");
	    strcat(nameport,convert[serialPort[i]]);
	    fob[i]=new FobAlone(POSITION_QUATERNION,FORWARD,nameport,CONTROL_HARD);
	}
	else
	{
	    strcpy(nameport,"/dev/ttyd");
	    strcat(nameport,convert[serialPort[i]]);
	    fob[i]=new FobAlone(POSITION_QUATERNION,FORWARD,nameport,CONTROL_SOFT);	
	}
	delete[] nameport;
    }
    if (nbFob==1)
    {
	fob[0]->InitAlone();
	sleep(1);
	
    }
    else
    {
	fob[0]->InitMaster(nbFob);
	for (int i=1;i<nbFob;i++)
	    fob[i]->InitSlave();
    }
}

void 
FobAlonePerf::Start()
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
    if ( (idServer = sproc(&FobAlonePerf::RunProc,PR_SADDR | PR_SFDS,(void *)this)) == -1)
    {
        cerr << "ERROR->ALONEFOBPERF/START: Unable to create new process for flock of birds" << endl;
        exit(1);
    }
    
}

void 
FobAlonePerf::Stop()
{
    for (int i=0;i<nbFob;i++)
	fob[i]->StopDriver();
    kill(idServer,SIGKILL);
    cerr << "NOTIFY->>ALONEFOBPERF/STOP: stop tracking process ("<<nbMeasure<<")"<<".."<<endl;
}

void 
FobAlonePerf::Close()
{

}
void 
FobAlonePerf::GetPosQuat(pfVec3 &posv,pfQuat &quatv,int number)
{
	static float pos[3];
	static float quat[4];

	ussetlock(lock[number]);
	fob[number]->GetSPosQuaternion(pos,quat);
	usunsetlock(lock[number]);
      
	posv.set(pos[0],-pos[2],pos[1]);
	quatv.set(quat[0],-quat[2],quat[1],quat[3]);
}

void 
FobAlonePerf::GetPos(pfVec3 &posv,int number)
{
    pfQuat quatv;
    GetPosQuat(posv,quatv,number);
}

void 
FobAlonePerf::GetQuat(pfQuat &quatv,int number)
{
    pfVec3 posv;
    GetPosQuat(posv,quatv,number);
}


void 
FobAlonePerf::Run()
{
    float pos[3],quat[4];
    float posf[3],quatf[4];
     
    cerr<<"NOTIFY->ALONEFOBPERF/RUN:start tracking process.."<<endl;


    for (int i=0;i<nbFob;i++)
    {
	fob[i]->SetFilter(FILTER_ON,FILTER_OFF,FILTER_ON);
	fob[i]->RunDriver();
    }
    while (1)
    {	
	for (int i=0;i<nbFob;i++)
	{
	//get value
	fob[i]->FobCommand::GetPosQuaternion(pos,quat);
       	//hemisphere change test
	if ((Signe(lastPos[i][fob[i]->axis_1])==-Signe(pos[fob[i]->axis_1]))
	     &&(Signe(lastPos[i][fob[i]->axis_2])==-Signe(pos[fob[i]->axis_2])))     
	    signe[i]=-signe[i];
	
	//update hemisphere
	posf[0]=signe[i]*pos[0];
	posf[1]=signe[i]*pos[1];
	posf[2]=signe[i]*pos[2];				
	quatf[0]=quat[0];
	quatf[1]=quat[1];
	quatf[2]=quat[2];
	quatf[3]=quat[3];		
	//pass to shared buffer
	ussetlock(lock[i]); 
	fob[i]->shared_pos[0]=posf[0];
	fob[i]->shared_pos[1]=posf[1];
	fob[i]->shared_pos[2]=posf[2];
	fob[i]->shared_quat[0]= quatf[0];
	fob[i]->shared_quat[1]= quatf[1];
	fob[i]->shared_quat[2]= quatf[2];
	fob[i]->shared_quat[3]= quatf[3];
	usunsetlock(lock[i]);
	
	//update last value
	lastPos[i][0]=pos[0];
	lastPos[i][1]=pos[1];
	lastPos[i][2]=pos[2];
	}
	nbMeasure++;      
    }
}

void 
FobAlonePerf::RunProc(void *data)
{
    FobAlonePerf *fp = (FobAlonePerf *)data;
    fp->Run();
}

int
FobAlonePerf::Signe(float valeur)
{
	if (valeur<0.0)
		return -1;
	else
		return 1;
}

void 
FobAlonePerf::InfoPosQuat(int number)
{
	static float pos[3];
	static float quat[4];
	ussetlock(lock[number]);
	fob[number]->GetSPosQuaternion(pos,quat);
	usunsetlock(lock[number]);
	cout<<"NOTIFY->ALONEFOBPERF/INFOPOSQUAT:(";
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
