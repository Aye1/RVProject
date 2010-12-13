#ifndef _Materiau_H
#define _Materiau_H




#include <stdio.h>
#include <string.h>


#include "Couleur.h"

using namespace std;

class Materiau{
private :
    /* Indique si on doit afficher la couleur ou la texture */
    bool _en_couleur;
    /* Tableau des couleurs associées au materiau */
    Couleur *_tab_couleurs;
    /* Nombre de couleurs */
    int _nb_couleurs;
    /* Numéro de la texture d'affichage */
    GLuint _display_texture;

public:
    // ACCESSEURS
    bool _est_en_couleur(){
        return _en_couleur;
    }

    GLuint get_display_texture(){
        return _display_texture;
    }

    Couleur *get_tab_couleurs(){
        return _tab_couleurs;
    }

    int get_nb_couleurs(){
        return _nb_couleurs;
    }



public:
    /** Constructeur avec une seule couleur d'affichage */
    Materiau(const Couleur& __col);

    /** Constructeur avec une texture d'affichage */
    Materiau(string __display_file);



};

#endif

