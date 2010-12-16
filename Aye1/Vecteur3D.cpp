

#include "Vecteur3D.h"



//----------------------------------------------------------------------
//        Constantes
//----------------------------------------------------------------------
const Vecteur3D Vecteur3D::ZERO(0,0,0);
const Vecteur3D Vecteur3D::X(1,0,0);
const Vecteur3D Vecteur3D::Y(0,1,0);
const Vecteur3D Vecteur3D::Z(0,0,1);
const Vecteur3D Vecteur3D::UNIT(1,1,1);



//----------------------------------------------------------------------
//        Constructeurs
//----------------------------------------------------------------------

Vecteur3D::Vecteur3D (float __x,float __y, float __z) :
    _x(__x), _y(__y), _z(__z){}


//----------------------------------------------------------------------
//        Operations
//----------------------------------------------------------------------
Vecteur3D& operator+(const Vecteur3D &__V1, const Vecteur3D &__V2){
    Vecteur3D *v = new Vecteur3D(__V1._x+__V2._x,
                                                        __V1._y+__V2._y,
                                                        __V1._z+__V2._z);
    return *v;
}

Vecteur3D& Vecteur3D::operator=(const Vecteur3D &__V2){
    _x = __V2._x;
    _y = __V2._y;
    _z = __V2._z;
    return (*this);
}










