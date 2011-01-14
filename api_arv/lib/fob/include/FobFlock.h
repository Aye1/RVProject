///////////////////////////////////////////////////////////////////////////////
//                                                           
// File name : FobFlock.h
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
// History :
//	20??/??/?? : Mr Grasset : creation of the file
//
// Rcs Id : "@(#)class FobFlock declaration."
//                                                           
///////////////////////////////////////////////////////////////////////////////

#if defined FobFlock_CYCLE

#error Header cyclic inclusion detected in FobFlock.h
#else // defined FobFlock_CYCLE
#define FobFlock_CYCLE

#if !defined FobFlock_h
#define FobFlock_h

///////////////////////////////////////////////////////////////////////////////
// include file
///////////////////////////////////////////////////////////////////////////////

//standard include

#include <iostream.h>

//graphics include


//personnal include
#include "FobCommand.h"

///////////////////////////////////////////////////////////////////////////////
// class FobFlock
///////////////////////////////////////////////////////////////////////////////

class FobFlock:public FobCommand
{
public:        
// Standard Services
    
    // default constructor
    FobFlock(Fob_RData type=POSITION_ANGLES,Fob_HData hemisphere=FORWARD,const char
*device_file="/dev/ttyf2",RS232_Mode mode_flux=CONTROL_HARD,int nb_fob=1);
    // copy constuctor
    FobFlock(const FobFlock &);
    // destructor
    ~FobFlock();    
    // affectation operator
    FobFlock& operator=(const FobFlock &);
    
// Interface
    
    // information interface
    void SelfDisplay(ostream& stream) const;

    bool OK(void) const;

    // mutator

    // services
    void Init();
    void RunDriver();
    void StopDriver();
    void GetSPosQuaternion(float pos[3],float quat[4],int number);
    void SetOrientation(float azimuth,float elevation,float roll,int number);

    void GetSButton(int* button);
    bool IsLeftPressed();
    bool IsMiddlePressed();
    bool IsRightPressed();

// Public Type
    
// Public Datas
    
    float shared_pos[6],shared_quat[8];//donnees partagees
    int axis_1[2],axis_2[2];//axe de changement d'hemisphere 0=x 1=y 2=z
    int shared_button;
protected: 
    
//Protected Services

//Protected Type

//Protected Data
   
private:

//Private Services

//Private Type

//Private Data
    Fob_HData RotateHem(Fob_HData h,int number);
    int nbFob;//nb fob in the flock
    Fob_RData mode;//mode d'utilisation du FOB
    Fob_HData hem;//hemisphere de travail
    float scale_x,scale_y,scale_z;//echelle entre le repere du transmetteur et le repere geometrique
    float pos_x,pos_y,pos_z;//position du repere transmetteur.
    int FLOCK_BROADCAST;
};

///////////////////////////////////////////////////////////////////////////////
// inline fonction
///////////////////////////////////////////////////////////////////////////////

inline ostream& operator <<(ostream& stream, const FobFlock& object_display);

ostream& operator <<(ostream& stream, const FobFlock& object_display)
{
    object_display.SelfDisplay(stream);
    return stream;
}

///////////////////////////////////////////////////////////////////////////////

#endif // !defined FobFlock_h

#undef FobFlock_CYCLE
#endif // else defined FobFlock_CYCLE


