///////////////////////////////////////////////////////////////////////////////
//                                                           
// File name : FobAlonePerf.h
//
// Creation : 2000/11/14
//
// Version : 2000/11/14
//
// Author : Raphael Grasset
//
// email : Raphael.Grasset@imag.fr
//
// Purpose : A Class for use one fob (or two with exclusive connection) in Perf
//           exclusively in stream mode
// Distribution :
//
// Use :
//	??
//
// History :
//	2000/11/14 : Mr Grasset : creation of the file
//
// Rcs Id : "@(#)class FobAlonePerf declaration."
//                                                           
///////////////////////////////////////////////////////////////////////////////

#if defined FobAlonePerf_CYCLE

#error Header cyclic inclusion detected in FobAlonePerf.h
#else // defined  FobAlonePerf_CYCLE
#define FobAlonePerf_CYCLE

#if !defined FobAlonePerf_h
#define FobAlonePerf_h

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

//standard include
#include <stdlib.h>
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
#include "FobAlone.h"


///////////////////////////////////////////////////////////////////////////////
// class FobAlonePerf
///////////////////////////////////////////////////////////////////////////////

const int NB_MAX_FLOCK=8;

class FobAlonePerf
{
public:        
// Standard Services
    
    // default constructor
    FobAlonePerf(int nb_flock=1);
    // copy constuctor
    FobAlonePerf(const FobAlonePerf &);
    // destructor
    ~FobAlonePerf();
    // affectation operator
    FobAlonePerf& operator=(const FobAlonePerf &);
    

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

    void Set(SerialType type,int serial,int number);
    void SetPen(int number=0);
    void SetIGlasses(int number=0);
    void GetPos(pfVec3 &,int number=0);
    void GetQuat(pfQuat &,int number=0);
    void GetPosQuat(pfVec3 &,pfQuat &,int number=0);
    void GetMatrix(pfMatrix &,int number=0);
    void InfoPosQuat(int number=0);


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
    int nbMeasure;
    FobAlone *fob[NB_MAX_FLOCK];
    int idServer;
    float signe[NB_MAX_FLOCK];
    ulock_t lock[NB_MAX_FLOCK];
    float lastPos[NB_MAX_FLOCK][3];
    float lastQuat[NB_MAX_FLOCK][4];
    SerialType fobType[NB_MAX_FLOCK];
    int serialPort[NB_MAX_FLOCK];
};

///////////////////////////////////////////////////////////////////////////////
// inline fonction
///////////////////////////////////////////////////////////////////////////////

inline ostream& operator <<(ostream& stream, const FobAlonePerf& object_display);

ostream& operator <<(ostream& stream, const FobAlonePerf& object_display)
{
    object_display.SelfDisplay(stream);
    return stream;
}

///////////////////////////////////////////////////////////////////////////////

#endif // !defined FobAlonePerf_h

#undef FobAlonePerf_CYCLE
#endif // else defined FobAlonePerf_CYCLE


