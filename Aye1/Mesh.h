#ifndef _Mesh_H
#define _Mesh_H


#ifdef USE_GLEW
	#include <GL/glew.h>	// OpenGL extensions
#endif
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <GL/glut.h>
#endif

#include <map>
#include <set>
#include <string>
#include <iostream>
#include "Vecteur3D.h"
#include "Materiau.h"
#include "quaternion.h"
#include "Lumiere.h"

/**
        Structure des Vertex
*/
typedef struct St_Vextex{
    GLfloat _position[3];
} Vertex;

/**
        Structure des Triangles
*/
typedef struct St_Triangles
{
        GLuint _vertexIndices[3];
} Triangles;

/**
        Structure du mapping
*/
typedef struct St_Mapping
{
    GLfloat _map[2];
} Mapping;

/**
        Structure des Normales
*/
typedef struct St_Normales
{
    GLfloat _normale[3];
} Normales;

/**
    Classe des Mesh
*/
class Mesh{
    /** Le mesh est visible */
    bool _visible;
    /** Le mesh est illuminé */
    bool _recoit_lumiere;
    /** Le mesh projette une ombre*/
    bool _projete_ombre;


private :
    // Geométrie
    int _nb_vertex;
    Vertex *_tab_vertex; // tab des vertex (positions)

    int _nb_triangles;
    Triangles *_tab_triangles; // tab des faces (indices des vertex)

    // Normales
    Normales *_tab_vertex_normales; // tab des normales des vertex
    Normales *_tab_triangles_normales; // tab des normales des faces

    // Texture
    Mapping * _tab_map; // tab de mapping des vertex
    Materiau *_materiau; // pointeur vers le materiau utilisé

    // Shadows volume
    Vertex *_shadow_vertex; // tab des vertex (pour le volume de l'ombre)
    int _nb_shadow_triangles;
    Triangles *_shadow_triangles; // tab des triangles ( pour le volume de l'ombre)



    // Correspondance : pour chaque vertex on sait à quelles faces il appartient
    /*  clef   : indice du vertex,
        valeur : set (indice : indice du triangle,
                        valeur : pointeur vers le triangle)
    */
    std::map<int, std::map<int, Triangles*> > _vertex;
    std::map<int, std::map<int, Triangles*> >::iterator _it_vertex;
    std::map<int, Triangles*>::iterator _it_triangle;



    // -------------------------------------------------------------------------
    //      Constructeurs
    //--------------------------------------------------------------------------
public :

    Mesh();

    Mesh(int __nb_vert, int __nb_tri);

    ~Mesh();

    /** Supprime tout le contenu du mesh */
    void RAZ();

    /** Amie du chargeur de fichiers collada */
    friend Mesh* Charger_Collada(std::string __nom_du_fichier);

    /** Amie du chargeur de fichiers MD3 */
    friend Mesh* Charger_MD3(std::string __nom_du_fichier);


    // -------------------------------------------------------------------------
    //       Primitives
    //--------------------------------------------------------------------------
public:
    /** Cree un plan */
    void Creer_Plan(float __cote,
                    int __x_texture_repeat,
                    int __y_texture_repeat);

    /** Cree un cube */
    void Creer_Cube(float __cote,
                    int __x_texture_repeat,
                    int __y_texture_repeat);

    // -------------------------------------------------------------------------
    //      Visiblitité
    //--------------------------------------------------------------------------
public :
    /** Le mesh ne sera plus affiché */
    void Rendre_Invisible();
    /** Le mesh sera affiché */
    void Rendre_Visible();

    // -------------------------------------------------------------------------
    //      Affichage
    //--------------------------------------------------------------------------
    /** Affiche le mesh
        @param position du mesh
        @param orientation du mesh
        @param echelle du mesh
    **/
    void Afficher(Vecteur3D& __position,
                  Quaternion& __orientation,
                  Vecteur3D& __echelle);

    /** Affiche le volume d'ombre du mesh
        @param position du volume d'ombre
        @param orientation du volume d'ombre
        @param echelle du volume d'ombre
    **/
    void Afficher_Volume_Ombre(Vecteur3D& __position,
                    Quaternion& __orientation,
                    Vecteur3D& __echelle);

    /** Affiche les normales du mesh
        @param position du mesh
        @param orientation du mesh
        @param echelle du mesh
    **/
    void Dessiner_Normales(Vecteur3D& __position,
                            Quaternion& __orientation,
                            Vecteur3D& __echelle);

    // -------------------------------------------------------------------------
    //      Debug
    //--------------------------------------------------------------------------
    /** Affiche le détail du mesh ( vertex, indices ...) */
    void Detailler();
    void Detailler_Vertex();
    void Detailler_Triangles();


    // -------------------------------------------------------------------------
    //       Texture
    //--------------------------------------------------------------------------
    /* Change le materiau du mesh*/
    void Changer_Materiau(const std::string& nom);

    // -------------------------------------------------------------------------
    //       Lumiere
    //--------------------------------------------------------------------------
public:
    /** Prepare l'illumination ( Calcule les correspondances et les normales du mesh  ) */
    void Preparer_Illumination();
    /** Le mesh subira la lumiere */
    void Sensible_Lumiere();
    /** Le mesh ne subit pas la lumiere */
    void Insensible_Lumiere();

private :
    /** Trouve à quel faces appartient un vertex */
    void _Calculer_Correspondances();

    /** Calcule les normales du mesh */
    void _Calculer_Normales();
    /** Calcule les normales du triangle donné */
    void _Calculer_Normales_Triangles(int __id_triangle);
    /** Calcule les normales des vertex
          Le calcul est basé sur le resultat du calcul des correspondances. */
    void _Calculer_Normales_Vertex();

public :
    /**
        Construit le volume d'ombre du mesh
        @param Point source de la lumiere
        @param type de la lumiere
        @param fermer le volume du côté objet
        @param fermer le volume du côté infini
        */
    void _Construire_Volume_Ombre(const Vecteur3D& lumiere,
                                    int type_lumiere,
                                    bool source,
                                    bool fermeture);





};//class Mesh



/**
    Classe de MeshPtr
*/
class MeshPtr{
    Mesh *meshptr;
};


#endif





