#ifndef COULEUR_H
#define COULEUR_H



#include <GL/glut.h>

/** Classe de base pour une couleur en RGB */
class Couleur{
    public :
        GLfloat rouge;
        GLfloat bleu;
        GLfloat vert;

        Couleur(GLfloat __r, GLfloat __b, GLfloat __v)
                     : rouge(__r), bleu(__b), vert(__v){}

        Couleur (const Couleur& __col )
                     : rouge(__col.rouge), bleu(__col.bleu), vert(__col.vert){}

};

/** Classe de base pour une couleur en RGBA */
class Couleur_Alpha : public Couleur{
    public :
        GLfloat alpha;

        Couleur_Alpha(GLfloat __r, GLfloat __b, GLfloat __v, GLfloat __a)
                     : Couleur(__r,__b,__v), alpha(__a){}

};

#endif

