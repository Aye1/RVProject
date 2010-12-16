
#include "quaternion.h"


#include <cmath>
#include <stdio.h>

Quaternion::Quaternion() : _Nr(1), _Ni(1), _Nj(1), _Nk(1) {};

Quaternion::Quaternion(float __Nr, float __Ni, float __Nj, float __Nk) : _Nr(__Nr), _Ni(__Ni), _Nj(__Nj), _Nk(__Nk) {};

Quaternion::~Quaternion() {};
/*
float quaternion::get_Nr() {return _Nr};
float quaternion::get_Ni() {return _Ni};
float quaternion::get_Nj() {return _Nj};
float quaternion::get_Nk() {return _Nk};

float quaternion::set_Nr(__Nr) {_Nr = __Nr};
float quaternion::set_Ni(__Ni) {_Ni = __Ni};
float quaternion::set_Nj(__Nj) {_Nj = __Nj};
float quaternion::set_Nk(__Nk) {_Nk = __Nk};
*/

Quaternion operator+(const Quaternion &_Q1, const Quaternion &_Q2){


    Quaternion *q = new Quaternion(_Q1._Nr+_Q2._Nr,
                    _Q1._Ni+_Q2._Ni,
                    _Q1._Nj+_Q2._Nj,
                    _Q1._Nk+_Q2._Nk);
    return *q;
}

Quaternion operator*(const Quaternion &_Q1, const Quaternion &_Q2){
   Quaternion *q= new Quaternion(_Q1._Nr*_Q2._Nr-_Q1._Ni*_Q2._Ni-_Q1._Nj*_Q2._Nj-_Q1._Nk*_Q2._Nk,
                            _Q1._Nr*_Q2._Ni + _Q2._Nr*_Q1._Ni + _Q1._Nj*_Q2._Nk - _Q1._Nk*_Q2._Nj,
                            _Q1._Nr*_Q2._Nj + _Q2._Nr*_Q1._Nj + _Q1._Nk*_Q2._Ni - _Q1._Ni*_Q2._Nk,
                            _Q1._Nr*_Q2._Nk + _Q2._Nr*_Q1._Nk + _Q1._Ni*_Q2._Nj - _Q1._Nj*_Q2._Ni
                            );
    printf("dans *1: %f\n",_Q1._Nr*_Q2._Nr-_Q1._Ni*_Q2._Ni-_Q1._Nj*_Q2._Nj-_Q1._Nk*_Q2._Nk);
    printf("%f\t%f\t%f\t%f\n",_Q1._Nr*_Q2._Nr,_Q1._Ni*_Q2._Ni,_Q1._Nj*_Q2._Nj,_Q1._Nk*_Q2._Nk);
    printf("dans *2: %f\n",_Q1._Nr*_Q2._Ni + _Q2._Nr*_Q1._Ni + _Q1._Nj*_Q2._Nk - _Q1._Nk*_Q2._Nj);
    printf("dans *3: %f\n",_Q1._Nr*_Q2._Nj + _Q2._Nr*_Q1._Nj + _Q1._Nk*_Q2._Ni - _Q1._Ni*_Q2._Nk);
    printf("dans *4: %f\n",_Q1._Nr*_Q2._Nk + _Q2._Nr*_Q1._Nk + _Q1._Ni*_Q2._Nj - _Q1._Nj*_Q2._Ni);
   return *q;
}


//----------------------------------------------------------------------
//        Constantes
//----------------------------------------------------------------------
const Quaternion Quaternion::ZERO(0,0,0,0);
const Quaternion Quaternion::UNIT(1,1,1,1);















