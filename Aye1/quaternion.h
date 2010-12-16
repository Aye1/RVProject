#ifndef _QUATERNION_
#define _QUATERNION_




class Quaternion {

public:
    /** coordonnées du quaternion*/
    float _Nr,_Ni,_Nj,_Nk;

public :
    /** Quaternions constructeurs*/
    Quaternion();
    Quaternion(float __Nr ,float __Ni,float __Nj,float __Nk);
    ~Quaternion();

friend Quaternion operator+(const Quaternion &_Q1, const Quaternion &_Q2);

friend Quaternion operator*(const Quaternion &_Q1, const Quaternion &_Q2);


    /** Constantes */
    static const Quaternion ZERO;
    static const Quaternion UNIT;

};

#endif

