#ifndef _Vecteur3D_H
#define _Vecteur3D_H



/**
    Classe De gestion de Vecteurs3D
*/


class Vecteur3D {

public :
        float _x, _y, _z;

public :
        //----------------------------------------------------------------------
        //        Constructeurs
        //----------------------------------------------------------------------
        Vecteur3D (float __x = 0,float __y = 0, float __z = 0);

        //----------------------------------------------------------------------
        //        Operations
        //----------------------------------------------------------------------
        /**
         *      Translater un vecteur
         */
        friend Vecteur3D& operator+(const Vecteur3D &__V1, const Vecteur3D &__V2);

        /**
         *      Egalité d'un vecteur
         */
        Vecteur3D& operator=(const Vecteur3D &__V2);

public :
        //----------------------------------------------------------------------
        //        Constantes
        //----------------------------------------------------------------------
        static const Vecteur3D ZERO;
        static const Vecteur3D X;
        static const Vecteur3D Y;
        static const Vecteur3D Z;
        static const Vecteur3D UNIT;


        void Set_XYZ(float x, float y, float z){
            _x = x;
            _y = y;
            _z = z;
        }


}; // Vecteur3D

#endif

