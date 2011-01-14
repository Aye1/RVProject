///////////////////////////////////////////////////////////////////////////////
//                                                           
// File name : FobFlockPerf.h
//
// Creation : 2001/01/25
//
// Version : 2001/01/25
//
// Author : Raphael Grasset
//
// email : Raphael.Grasset@imag.fr
//
// Purpose : interface for the FobFlock with Performer.
//
// Distribution :
//
// Use :
//	??
//
// History :
//	2001/25/10 : Mr Grasset : creation of the file
//
// Rcs Id : "@(#)class FobFlockPerf declaration."
//                                                           
///////////////////////////////////////////////////////////////////////////////

#if defined FobFlockPerf_CYCLE

#error Header cyclic inclusion detected in FobFlockPerf.h
#else // defined FobFlockPerf_CYCLE
#define FobFlockPerf_CYCLE

#if !defined FobFlockPerf_h
#define FobFlockPerf_h

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

//standard include

#include <iostream.h>
#include <iostream.h>
#include <fstream.h>

#include <assert.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
//graphics include
#include <Performer/pf.h>
#include <Performer/pfdu.h>
#include <Performer/pfutil.h>
#include <Performer/pfui.h>
#include <Performer/pr/pfLinMath.h>

//personnal include
#include "FobFlock.h"

///////////////////////////////////////////////////////////////////////////////
// class FobFlockPerf
///////////////////////////////////////////////////////////////////////////////

#define NB_MAX_FLOCK 2

class FobFlockPerf
{
public:        
// Standard Services
    
    // default constructor
    FobFlockPerf(int nb_flock=1);
    // copy constuctor
    FobFlockPerf(const FobFlockPerf &);
    // destructor
    ~FobFlockPerf();    
    // affectation operator
    FobFlockPerf& operator=(const FobFlockPerf &);
    
// Interface
    
    // information interface
    void SelfDisplay(ostream& stream) const;

    bool OK(void) const;

    // mutator

    // services
    void Init();
    void Start();
    void Stop();
    void Close();
    
    enum SerialType {SERIAL_SOFT,SERIAL_HARD};

    void Set(SerialType type,int serial);
    void SetPen(int number=0);
    void SetIGlasses(int number=0);
    void GetPos(pfVec3 &,int number=0);
    void GetQuat(pfQuat &,int number=0);
    void GetPosQuat(pfVec3 &,pfQuat &,int number=0);
    void InfoPosQuat(int number=0);
    bool GetButton(int number);

// Public Type
    
// Public Datas
    
    
protected: 
    
//Protected Services

//Protected Type

//Protected Data
	
private:

//Private Services
    void Run();
    int Signe(float valeur);
    static void RunProc(void *data);

//Private Type

//Private Data
    int nbFob;
    FobFlock *fob;
    int idServer;
    float signe[NB_MAX_FLOCK];
    ulock_t lock[NB_MAX_FLOCK];
    ulock_t buttonlock;
    float lastPos[6];
    float lastQuat[8];
    SerialType fobType;
    int serialPort;
    int nbMeasure;
};

///////////////////////////////////////////////////////////////////////////////
// inline fonction
///////////////////////////////////////////////////////////////////////////////

inline ostream& operator <<(ostream& stream, const FobFlockPerf& object_display);

ostream& operator <<(ostream& stream, const FobFlockPerf& object_display)
{
    object_display.SelfDisplay(stream);
    return stream;
}

///////////////////////////////////////////////////////////////////////////////

#endif // !defined FobFlockPerf_h

#undef FobFlockPerf_CYCLE
#endif // else defined FobFlockPerf_CYCLE


