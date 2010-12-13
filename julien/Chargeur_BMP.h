#ifndef _Chargeur_BMP_H
#define _Chargeur_BMP_H


#include "Ressource_Manager.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

/** Structures permettant le chargement rapide du fichier BMP */
struct Header_Type{
  char  signature[2];
};

struct Header_BMP
{
  unsigned int   taille_fichier;

  int   reserve;

  unsigned int   ofs_image;

  unsigned  int   taille_header;
  int   largeur;
  int   hauteur;
  unsigned short int plans;
  unsigned short int format;

  unsigned int   compression;

  unsigned int   taille;
  int   resolution_h;
  int   resolution_v;

  unsigned int   nb_couleurs;
  unsigned int   nb_couleurs_importantes;
};




/** Charge un fichier BMP et renvoie la structure contenant les données */
    Image* Charger_Texture_BMP(std::string __nom);

#endif

