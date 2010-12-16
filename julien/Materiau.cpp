
#include "Ressource_Manager.h"
#include "Materiau.h"

    /** Constructeur avec une seule couleur d'affichage */
    Materiau::Materiau(const Couleur& __col){
        // En couleur
        _en_couleur = true;
        // On alloue la couleur
        _tab_couleurs = new Couleur(__col);
        _nb_couleurs = 1;
    }

    /** Constructeur avec une texture d'affichage */
    Materiau::Materiau(string __display_file){
        // Pas en couleur
        _en_couleur = false;
        // On recupere l'id de la texture
        _display_texture = Ressource_Manager::get_singleton()->get_id_texture(__display_file);
    }

