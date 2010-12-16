
#include "Chargeur_BMP.h"

#include "Ressource_Manager.h"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////              Specifique au singleton                     ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Ressource_Manager *Ressource_Manager::_instance = NULL;

    /** Constructeur privé */
    Ressource_Manager::Ressource_Manager(){
        // On charge le materiau de base
        Materiau *materiau = new Materiau(Couleur(0.9, 0.9, 1.0));
        _materiaux_charges["Defaut"] = materiau;
    }

    /** Destructeur privé */
    Ressource_Manager::~Ressource_Manager(){
    }

    /** Creer le manager */
	void Ressource_Manager::creer(){
	    _instance = new Ressource_Manager();
	}

    /** Detruire le manager */
	void Ressource_Manager::detruire(){
	    delete _instance;
	}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////              Specifique a la classe                        ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    // -------------------------------------------------------------------------
    //      La gestion des meshs chargés
    //--------------------------------------------------------------------------

    /** Charger un mesh et l'ajouter au manager*/
    bool Ressource_Manager::charger_mesh_a_partir_dun_fichier(const std::string& nom){
        return false;
    }

    /** Recuperer un mesh
        Null si le mesh n'existe pas
    */
    Mesh* Ressource_Manager::recuperer_mesh(const std::string& nom){
        return NULL;
    }

    /** Dupliquer un mesh
        Retourne une copie du mesh
        Null si le mesh n'existe pas
    */
    Mesh* Ressource_Manager::dupliquer_mesh(const std::string& nom){
        return NULL;
    }


    // -------------------------------------------------------------------------
    //      La gestion des materiaux chargés
    //--------------------------------------------------------------------------

    /** Ajouter un materiau */
    bool Ressource_Manager::ajouter_materiau(const std::string& nom, Materiau* __mat){
            // On vérifie que le materiau n'est pas déja chargé
        _it_materiaux = _materiaux_charges.find( nom );
        if( _it_materiaux == _materiaux_charges.end() ) {
            _materiaux_charges[ nom ] = __mat;
            return true;
        }
        return false;
    }

    /** Recuperer un materiau */
    Materiau* Ressource_Manager::recuperer_materiau(const std::string& nom){
        _it_materiaux = _materiaux_charges.find( nom );
        if( _it_materiaux != _materiaux_charges.end() ) {
            return _it_materiaux->second;
        }
        return _materiaux_charges["Defaut"];
    }

    // -------------------------------------------------------------------------
    //      La gestion des textures chargées
    //--------------------------------------------------------------------------


    /** Ajouter une texture */
    bool Ressource_Manager::charger_texture_a_partir_dun_fichier(const std::string& nom){
        // On vérifie que la texture n'est pas déja chargée
        _it_textures = _textures_chargees.find( nom );
        if( _it_textures == _textures_chargees.end() ) {

            // alloue la structure
            Texture *text = new Texture;

            // charge l'image de la texture
            text->image = Charger_Texture_BMP(nom);

            // si le chargement est un echec on renvoie faux
            if (text->image == 0){
                return false;
            }

            // on ajoute la texture à la liste
            _textures_chargees[ nom ] = text;

        }
        return true;
    }

    /** Ajouter une texture deja cree*/
    bool Ressource_Manager::ajouter_texture_deja_cree(const std::string& nom, Texture* __texture){
        // On vérifie que la texture n'est pas déja chargée
        _it_textures = _textures_chargees.find( nom );
        if( _it_textures == _textures_chargees.end() ) {
            // on ajoute la texture à la liste
            _textures_chargees[ nom ] = __texture;
            return true;
        }
        return false;
    }

    /** Charger les ressources dans la carte*/
    bool Ressource_Manager::charger_textures_dans_CG(){
        GLuint textureId;
        _it_textures = _textures_chargees.begin();


        for( ; _it_textures !=  _textures_chargees.end() ; ++_it_textures ) {
            // Trouve un pointer
            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // RGB ou RGBA ?
            try{
                if (_it_textures->second->image->format == 24){
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                                 _it_textures->second->image->width,
                                 _it_textures->second->image->height,
                                 0, GL_RGB, GL_UNSIGNED_BYTE,
                                 _it_textures->second->image->texture);
                }

                if (_it_textures->second->image->format == 32){
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                                 _it_textures->second->image->width,
                                 _it_textures->second->image->height,
                                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                                 _it_textures->second->image->texture);
                }
             }catch(...){
                return false;
             }
            // met à jour l'id de la texture
            _it_textures->second->index = textureId;
        }
        cout << "Chargement des textures dans la carte graphique réussit" << endl;
        return true;
    }





    /** Renvoie l'Id de la texture */
    GLuint Ressource_Manager::get_id_texture(const std::string& nom){
        _it_textures = _textures_chargees.find( nom );
        if( _it_textures != _textures_chargees.end() ) {
            return _it_textures->second->index;
        }
        return 0;
    }






