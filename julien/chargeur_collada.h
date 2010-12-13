#ifndef _Chargeur_Collada_H
#define _Chargeur_Collada_H



#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Mesh.h"

/** Methode amie de MESH.h */
/** Charge un mesh à partir d'un fichier Collada et renvoie le Mesh correspondant */
    Mesh* Charger_Collada(std::string __nom_du_fichier);

#endif


