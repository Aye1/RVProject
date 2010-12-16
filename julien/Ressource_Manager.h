#ifndef _Ressource_Manager_H
#define _Ressource_Manager_H



#ifdef USE_GLEW
    #include <GL/glew.h>	// OpenGL extensions
#endif
#include <GL/glut.h>
#include <stdlib.h>
#include <map>
#include <string>

#include "Materiau.h"
#include "Mesh.h"

enum FORMAT { GRAY  = 8,
                            RGB   = 24,
                            RGBA = 32 };

/* Structure definissant une image */
typedef struct Image_Struct{
    char *texture;
    int width, height;
    FORMAT format;
}Image;

/* Structure definissant une image
    Index = numéro de la texture dans la carte graphique
    Image = pointeur vers l'image */
typedef struct Texture_Struct{
    GLuint index;
    Image *image;
}Texture;



/**
 *  Gestionnaire de ressources.
 *  Il permet de gerer les materiaux, les textures, les images
 *  C'est un singleton.
 */
class Ressource_Manager{

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////              Specifique au singleton                     ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

private:
    /** Instance du manager */
    static Ressource_Manager *_instance;

    /** Constructeur privé */
    Ressource_Manager();

    /** Destructeur privé */
    ~Ressource_Manager();

public:
    /** Creer le manager */
	static void creer();

    /** Detruire le manager */
	static void detruire();

	/** Renvoie l'instance */
	static Ressource_Manager* get_singleton(){
	    return _instance;
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

    /** La liste des materiaux chargés */
    std::map<std::string, Mesh*> _meshs_charges;

    /** Un iterateur pour les materiaux chargés */
    std::map<std::string, Mesh*>::iterator _it_meshs;

    /** Charger un mesh et l'ajouter au manager*/
    bool charger_mesh_a_partir_dun_fichier(const std::string& nom);

    /** Recuperer un mesh
        Null si le mesh n'existe pas
    */
    Mesh* recuperer_mesh(const std::string& nom);

    /** Dupliquer un mesh
        Retourne une copie du mesh
        Null si le mesh n'existe pas
    */
    Mesh* dupliquer_mesh(const std::string& nom);


    // -------------------------------------------------------------------------
    //      La gestion des materiaux chargés
    //--------------------------------------------------------------------------

    /** La liste des materiaux chargés */
    std::map<std::string, Materiau*> _materiaux_charges;

    /** Un iterateur pour les materiaux chargés */
    std::map<std::string, Materiau*>::iterator _it_materiaux;

    /** Ajouter un materiau */
    bool ajouter_materiau(const std::string& nom, Materiau* __mat);

    /** Recuperer un materiau */
    Materiau* recuperer_materiau(const std::string& nom);

    // -------------------------------------------------------------------------
    //      La gestion des textures chargées
    //--------------------------------------------------------------------------
private:
    /** La liste des textures chargées */
    std::map<std::string, Texture*> _textures_chargees;

    /** Un iterateur pour les textures chargées */
    std::map<std::string, Texture*>::iterator _it_textures;

public:
    /** Renvoie l'Id de la texture */
    GLuint get_id_texture(const std::string& nom);

    /** Charger une texture et l'ajouter au manager*/
    bool charger_texture_a_partir_dun_fichier(const std::string& nom);

    /** Ajouter une texture deja cree*/
    bool ajouter_texture_deja_cree(const std::string& nom, Texture* __texture);

    /**
        Charger les ressources dans la carte
        DOIT être fait avant de lancer le rendu
    */
    bool charger_textures_dans_CG();



};

#endif

