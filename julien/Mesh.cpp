

#include <iostream>
#include <cmath>
#include "Mesh.h"
#include "Debug.h"

#include "Ressource_Manager.h"



/*------------------------------------------------------------------------------
-     Constructeur
------------------------------------------------------------------------------*/
Mesh::Mesh(){
        Debug::_trace(3, "constructeur Mesh Vide ", this);
        _visible = false;
        _nb_vertex = 0;
        _nb_triangles = 0;
        _tab_vertex = 0;
        _tab_triangles = 0;

        // Normales
        _tab_vertex_normales    = 0;
        _tab_triangles_normales = 0;

        // Materiau
        _tab_map                = 0;
        _materiau = Ressource_Manager::get_singleton()->recuperer_materiau("Defaut");
        if (_materiau == 0){
            std::cerr << "Echec du chargement du materiau de base \n";
            return;
        }
        // Ombres
        _shadow_vertex = 0;
        _shadow_triangles = 0;
        _nb_shadow_triangles = 0;

}


Mesh::Mesh(int __nb_vert, int __nb_tri){
        Debug::_trace(3, "constructeur Mesh  ", this);
        _visible = false;
        _nb_vertex = __nb_vert;
        _nb_triangles = __nb_tri;
        _tab_vertex = new Vertex[__nb_vert];
        _tab_triangles = new Triangles[__nb_tri];

        // Normales
        _tab_vertex_normales    = new Normales[__nb_vert];
        _tab_triangles_normales = new Normales[__nb_tri];

        // Materiau
        _tab_map                = new Mapping[__nb_vert];
        _materiau = Ressource_Manager::get_singleton()->recuperer_materiau("Defaut");
        if (_materiau == 0){
            std::cerr << "Echec du chargement du materiau de base \n";
            return;
        }
        // Ombres
        _shadow_vertex = 0;
        _shadow_triangles = 0;
        _nb_shadow_triangles = 0;

}

/*------------------------------------------------------------------------------
-     Destructeur
------------------------------------------------------------------------------*/
Mesh::~Mesh(){
        Debug::_trace(3, "destructeur Mesh", this);
        RAZ();
}

// -------------------------------------------------------------------------
//       Primitives
//--------------------------------------------------------------------------
/** RAZ du mesh */
void Mesh::RAZ(){


    _nb_vertex = 0;
    _nb_triangles = 0;
    _nb_shadow_triangles = 0;

    delete[] _tab_vertex;
    delete[] _tab_triangles;
    delete[] _tab_vertex_normales;
    delete[] _tab_triangles_normales;
    delete[] _tab_map;
    delete[] _shadow_vertex;
    delete[] _shadow_triangles;

    _tab_vertex = 0;
    _tab_triangles = 0;
    _tab_vertex_normales = 0;
    _tab_triangles_normales = 0;
    _tab_map = 0;
    _shadow_vertex = 0;
    _shadow_triangles = 0;



}

/** Cree un plan */
void Mesh::Creer_Plan(float __cote,
                      int __x_texture_repeat,
                      int __y_texture_repeat){

    Debug::_trace(4, "Primitive Plan", this);

    // verification de la taille
    if(__cote == 0){ exit(1);}

    //RAZ
    RAZ();

    // Allocation
    _nb_vertex = 4;
    _nb_triangles = 2;

    _tab_vertex = new Vertex[4];
    _tab_triangles = new Triangles[2];
    _tab_vertex_normales = new Normales[4];
    _tab_triangles_normales = new Normales[2];
    _tab_map = new Mapping[4];

    // Creation

    _tab_vertex[0]._position={ -__cote/2, -__cote/2, 0};
    _tab_map[0]._map={0,0};

    _tab_vertex[1]._position={ __cote/2, -__cote/2, 0};
    _tab_map[1]._map={(float)__x_texture_repeat, 0};

    _tab_vertex[2]._position={ __cote/2, __cote/2, 0};
    _tab_map[2]._map={(float)__x_texture_repeat, (float)__y_texture_repeat};

    _tab_vertex[3]._position={ -__cote/2, __cote/2, 0};
    _tab_map[3]._map={0, (float)__y_texture_repeat};

    _tab_triangles[0]._vertexIndices = { 0, 1, 3};
    _tab_triangles[1]._vertexIndices = { 2, 3, 1};


    _visible = true;
}


/** Cree un cube */
void Mesh::Creer_Cube(float __cote,
                int __x_texture_repeat,
                int __y_texture_repeat){

    Debug::_trace(4, "Primitive Cube", this);

    // verification de la taille
    if(__cote == 0){ exit(1);}

    //RAZ
    RAZ();

    // Allocation
    _nb_vertex = 8;
    _nb_triangles = 12;

    _tab_vertex = new Vertex[8];
    _tab_triangles = new Triangles[12];
    _tab_vertex_normales = new Normales[8];
    _tab_triangles_normales = new Normales[12];
    _tab_map = new Mapping[8];

    // Creation

    _tab_vertex[0]._position={ 0, 0, 0};
    _tab_map[0]._map={0,0};

    _tab_vertex[1]._position={ __cote, 0, 0};
    _tab_map[1]._map={(float)__x_texture_repeat,0};

    _tab_vertex[2]._position={__cote, __cote, 0};
    _tab_map[2]._map={(float)__x_texture_repeat, (float)__y_texture_repeat};

    _tab_vertex[3]._position={0, __cote, 0};
    _tab_map[3]._map={0,(float)__y_texture_repeat};

    _tab_vertex[4]._position={0, 0, - __cote};
    _tab_map[4]._map={0,0};

    _tab_vertex[5]._position={__cote, 0, - __cote};
    _tab_map[5]._map={(float)__x_texture_repeat,0};

    _tab_vertex[6]._position={__cote, __cote, - __cote};
    _tab_map[6]._map={(float)__x_texture_repeat, (float)__y_texture_repeat};

    _tab_vertex[7]._position={0, __cote, - __cote};
    _tab_map[7]._map={0, (float)__y_texture_repeat};

    _tab_triangles[0]._vertexIndices = { 0, 1, 3};
    _tab_triangles[1]._vertexIndices = { 2, 3, 1};

    _tab_triangles[2]._vertexIndices = { 0, 5, 1};
    _tab_triangles[3]._vertexIndices = { 0, 4, 5};

    _tab_triangles[4]._vertexIndices = { 0, 3, 7};
    _tab_triangles[5]._vertexIndices = { 0, 7, 4};


    _tab_triangles[6]._vertexIndices = { 1, 5, 6};
    _tab_triangles[7]._vertexIndices = { 1, 6, 2};

    _tab_triangles[8]._vertexIndices = { 5, 7, 6};
    _tab_triangles[9]._vertexIndices = { 5, 4, 7};

    _tab_triangles[10]._vertexIndices = { 3, 2, 6};
    _tab_triangles[11]._vertexIndices = { 3, 6, 7};


    _visible = true;

}


// -------------------------------------------------------------------------
//      Visiblitité
//--------------------------------------------------------------------------

void Mesh::Rendre_Invisible(){
        Debug::_trace(4, "Mesh Rendre_Invisible");
        _visible = false;
}

void Mesh::Rendre_Visible(){
        Debug::_trace(4, "Mesh Rendre_Visible");
        _visible = true;
}



/*------------------------------------------------------------------------------
-     Affichage Debug
------------------------------------------------------------------------------*/



void Mesh::Detailler_Vertex(){
     if (_tab_vertex != 0 ){
            int i;
            std::cout<<"- Vertex - "<< _nb_vertex<< std::endl;
            for (i = 0; i < _nb_vertex; i++){
                std::cout<<_tab_vertex[i]._position[0];
                std::cout<<" ,  "<<_tab_vertex[i]._position[1];
                std::cout<<" ,  "<<_tab_vertex[i]._position[2]<<std::endl;
            }
    }
}
void Mesh::Detailler_Triangles(){
        if ( _tab_triangles !=0 ){
            int i;
            std::cout<<"- Triangles - "<<_nb_triangles<< std::endl;
            for (i = 0; i < _nb_triangles; i++){
                std::cout<<_tab_triangles[i]._vertexIndices[0];
                std::cout<<" ,  "<<_tab_triangles[i]._vertexIndices[1];
                std::cout<<" ,  "<<_tab_triangles[i]._vertexIndices[2]<<std::endl;
            }
            std::cout<<std::endl;
        }
}


void Mesh::Detailler(){
        if (_tab_vertex != 0 && _tab_triangles !=0 ){
            std::cout<<std::endl<<"-- Mesh detail --"<<std::endl;
            int i;
            std::cout<<"- Vertex - "<< _nb_vertex<< std::endl;
            for (i = 0; i < _nb_vertex; i++){
                std::cout<<_tab_vertex[i]._position[0];
                std::cout<<" ,  "<<_tab_vertex[i]._position[1];
                std::cout<<" ,  "<<_tab_vertex[i]._position[2]<<std::endl;
            }
            std::cout<<"- Triangles - "<<_nb_triangles<< std::endl;
            for (i = 0; i < _nb_triangles; i++){
                std::cout<<_tab_triangles[i]._vertexIndices[0];
                std::cout<<" ,  "<<_tab_triangles[i]._vertexIndices[1];
                std::cout<<" ,  "<<_tab_triangles[i]._vertexIndices[2]<<std::endl;
            }
            std::cout<<std::endl;
        }
}


/*------------------------------------------------------------------------------
-     Affichage
------------------------------------------------------------------------------*/



void Mesh::Afficher(Vecteur3D& __position,
                    Quaternion& __orientation,
                    Vecteur3D& __echelle) {


            // sauvegarde la matrice actuelle
            glPushMatrix();

            // change le mode de matrice en mode modelview
            glMatrixMode (GL_MODELVIEW);

            //effecture les transformations sur le model
            glTranslatef(__position._x, __position._y, __position._z);
            glRotated (__orientation._Nr, __orientation._Ni, __orientation._Nj, __orientation._Nk);
            glScalef(__echelle._x, __echelle._y, __echelle._z);

            // On active le mode Array
            glEnableClientState(GL_VERTEX_ARRAY);

            if (_materiau->_est_en_couleur()){


                //std::cout<<"Couleur\n";
                //glEnableClientState(GL_COLOR_ARRAY);

                if (_recoit_lumiere){
                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 0, _tab_vertex_normales);
                }

                /*GLfloat colors[] =   { 0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                   0.0, 0.0, 1.0,
                                       0.0, 0.0, 1.0}; */
                //glColorPointer(3, GL_FLOAT, 1, _materiau->_color);

                /*
                glColor3f(_materiau->get_tab_couleurs()[0].rouge,
                          _materiau->get_tab_couleurs()[0].bleu,
                          _materiau->get_tab_couleurs()[0].vert);
                          */

                //Detailler();

                glColor3f(_materiau->get_tab_couleurs()[0].rouge,
                          _materiau->get_tab_couleurs()[0].bleu,
                          _materiau->get_tab_couleurs()[0].vert);

                glVertexPointer(3, GL_FLOAT, 0, _tab_vertex);

                glDrawElements(GL_TRIANGLES, 3 * _nb_triangles, GL_UNSIGNED_INT, _tab_triangles);


                glDisableClientState(GL_VERTEX_ARRAY);
                //glDisableClientState(GL_COLOR_ARRAY);

                if (_recoit_lumiere){
                    glDisableClientState(GL_NORMAL_ARRAY);
                }


            }
            else
            {

                glEnable( GL_POLYGON_SMOOTH );

                // Params OpenGL
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_NEAREST);

                //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_TEXTURE_2D);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                glBindTexture(GL_TEXTURE_2D, _materiau->get_display_texture());


                glEnableClientState(GL_TEXTURE_2D);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                if (_recoit_lumiere){
                    glEnableClientState(GL_NORMAL_ARRAY);
                    glNormalPointer(GL_FLOAT, 3, _tab_vertex_normales);
                }

                glTexCoordPointer(2, GL_FLOAT, 0, _tab_map);
                glVertexPointer(3, GL_FLOAT, 0, _tab_vertex);

                glDrawElements(GL_TRIANGLES, 3 * _nb_triangles, GL_UNSIGNED_INT, _tab_triangles);

                glDisableClientState(GL_VERTEX_ARRAY);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);

                if (_recoit_lumiere){
                    glDisableClientState(GL_NORMAL_ARRAY);
                }

                glDisable(GL_TEXTURE_2D);
            }


            // restaure la matrice
            glPopMatrix();
        //Dessiner_Normales(__position, __angle, __orientation, __echelle);

}






void Mesh::Afficher_Volume_Ombre(Vecteur3D& __position,
                    Quaternion& __orientation,
                    Vecteur3D& __echelle) {


        Debug::_trace(6, "Mesh Afficher Volume ombre", this);

        if ( _visible && _shadow_vertex != 0 && _shadow_triangles !=0 ){


            //glColor4f(0.9f,0.1f,0.1f,0.9f);


            // sauvegarde la matrice actuelle
            glPushMatrix();

            // change le mode de matrice en mode modelview
            glMatrixMode (GL_MODELVIEW);

            //effecture les transformations sur le model
            glTranslatef(__position._x, __position._y, __position._z);
            glRotated (__orientation._Nr, __orientation._Ni, __orientation._Nj, __orientation._Nk);
            glScalef(__echelle._x, __echelle._y, __echelle._z);






            // couleur
            /*GLfloat colors[] =   {     1.0, 0.0, 0.0,
                                       0.0, 0.0, 0.0,
                                       0.0, 0.5, 0.0,
                                       1.0, 0.2, 0.0,
                                       0.0, 0.2, 0.0,
                                       0.0, 0.7, 0.0};
            */

            // On active le mode Array
            glEnableClientState(GL_VERTEX_ARRAY);
            //glEnableClientState(GL_COLOR_ARRAY);

            glVertexPointer(3, GL_FLOAT, 0, _shadow_vertex);
            //glColorPointer(3, GL_FLOAT, 0, colors);
            glColor4f(0.9f,0.1f,0.1f,0.7f);
                //glColorPointer(3, GL_FLOAT, 1, _materiau->_color);

            glDrawElements(GL_TRIANGLES, 3 * _nb_shadow_triangles, GL_UNSIGNED_INT, _shadow_triangles);

           // glDrawElements(GL_POINTS, 3 * _nb_shadow_triangles, GL_UNSIGNED_INT, _shadow_triangles);
           // glDrawElements(GL_LINES, 3 * _nb_shadow_triangles, GL_UNSIGNED_INT, _shadow_triangles);


            glDisableClientState(GL_VERTEX_ARRAY);
            //glDisableClientState(GL_COLOR_ARRAY);

            // restaure la matrice
            glPopMatrix();

        }
}





void Mesh::Dessiner_Normales(Vecteur3D& __position,
                             Quaternion& __orientation,
                             Vecteur3D& __echelle){
    std::cout << " Dessiner_Normales \n";
    if ( _visible && _tab_vertex != 0 && _tab_vertex_normales !=0 ){

            // sauvegarde la matrice actuelle
            glPushMatrix();

            // change le mode de matrice en mode modelview
            glMatrixMode (GL_MODELVIEW);

            //effecture les transformations sur le model
            glTranslatef(__position._x, __position._y, __position._z);
            glRotated (__orientation._Nr, __orientation._Ni, __orientation._Nj, __orientation._Nk);
            glScalef(__echelle._x, __echelle._y, __echelle._z);


            Vertex *vertex = _tab_vertex;
            Normales *normales = _tab_vertex_normales;
            glColor3f(1.0f,0.0f,0.0f);
            for( int i = 0; i < _nb_vertex; i ++){
                glBegin(GL_LINES);
                    glVertex3f( vertex->_position[0], vertex->_position[1], vertex->_position[2]);
                    glVertex3f( vertex->_position[0] + normales->_normale[0] * 1,
                                vertex->_position[1] + normales->_normale[1] * 1,
                                vertex->_position[2] + normales->_normale[2] * 1);
                glEnd();
                vertex ++;
                normales ++;
            }

            glPopMatrix();
    }
}


    // -------------------------------------------------------------------------
    //       Texture
    //--------------------------------------------------------------------------


void Mesh::Changer_Materiau(const std::string& nom){
    _materiau = Ressource_Manager::get_singleton()->recuperer_materiau(nom);
}




    // -------------------------------------------------------------------------
    //       Lumiere
    //--------------------------------------------------------------------------

void Mesh::Preparer_Illumination(){
    _Calculer_Correspondances();
    _Calculer_Normales();
}


// Correspondance
void Mesh::_Calculer_Correspondances(){
    //std::cout << "Calculer correspondances \n";
    Triangles *triangles = _tab_triangles;
    //std::cout << "Il y a  " << _nb_triangles << " triangles \n";
    for ( int i = 0; i < _nb_triangles ; i ++ )
    {
      // pour chaque vertex associé au triangle
      for ( int j = 0; j < 3 ; j ++ )
      {
         _it_vertex = _vertex.find(  triangles->_vertexIndices[j] );
         // Si le vertex n'est pas listé, on l'ajoute
         if( _it_vertex == _vertex.end())
         {
             std::map<int, Triangles*> triangle_set;
             triangle_set[i] = triangles;
             _vertex[triangles->_vertexIndices[j]] = triangle_set;
             /*
             std::cout << "Creation du point "
                       << triangles->_vertexIndices[j] << "\n";
             std::cout << "Ajout de " << i << " pour le point "
                       << triangles->_vertexIndices[j] << "\n";
             */
         }
         // Le vertex est listé, on ajoute le triangle à la liste associée
         else
         {
             /*
             std::cout << "Ajout de " << i << " pour le point "
                       << triangles->_vertexIndices[j] << "\n";
             */
             (_it_vertex->second)[i] = triangles;
         }
      }
   triangles++;
   }
}


// Normales

void Mesh::_Calculer_Normales()
{
    for ( int i = 0; i < _nb_triangles ; i ++ )
    {
        _Calculer_Normales_Triangles(i);
    }
    _Calculer_Normales_Vertex();
}

void Mesh::_Calculer_Normales_Triangles(int __id_triangle)
{
    // recuperation des points du triangle
    Vertex p1 = _tab_vertex[_tab_triangles[__id_triangle]._vertexIndices[0]];
    Vertex p2 = _tab_vertex[_tab_triangles[__id_triangle]._vertexIndices[1]];
    Vertex p3 = _tab_vertex[_tab_triangles[__id_triangle]._vertexIndices[2]];

    // calcul des vecteurs p1->p2  et p1->p3
    float v1_x = p2._position[0] - p1._position[0];
    float v1_y = p2._position[1] - p1._position[1];
    float v1_z = p2._position[2] - p1._position[2];

    float v2_x = p3._position[0] - p1._position[0];
    float v2_y = p3._position[1] - p1._position[1];
    float v2_z = p3._position[2] - p1._position[2];

    // calcul de la normale

    float n_x = 0, n_y = 0, n_z = 0;
    float norme;

    n_x = ((v1_y * v2_z) - (v1_z * v2_y));
    n_y = ((v1_z * v2_x) - (v1_x * v2_z));
    n_z = ((v1_x * v2_y) - (v1_y * v2_x));

    norme = sqrt(n_x * n_x + n_y * n_y + n_z * n_z);

    n_x = n_x / norme;
    n_y = n_y / norme;
    n_z = n_z / norme;

    _tab_triangles_normales[__id_triangle]._normale[0] = n_x;
    _tab_triangles_normales[__id_triangle]._normale[1] = n_y;
    _tab_triangles_normales[__id_triangle]._normale[2] = n_z;

    /*
    std::cout << "Normale du triangle " << __id_triangle << std::endl;
    std::cout << " x : " << _tab_triangles_normales[__id_triangle]._normale[0];
    std::cout << " , y : " << _tab_triangles_normales[__id_triangle]._normale[1];
    std::cout << " , z : " << _tab_triangles_normales[__id_triangle]._normale[2];
    std::cout << std::endl;
    */

}

void Mesh::_Calculer_Normales_Vertex(){
    _it_vertex = _vertex.begin();
    for( ; _it_vertex !=  _vertex.end() ; ++_it_vertex )
    {
        float n_x = 0, n_y = 0, n_z = 0;
        // Pour chaque triangle qui le touche on modifie la normale
        _it_triangle = (_it_vertex->second).begin();
        for( ; _it_triangle !=  (_it_vertex->second).end() ; ++_it_triangle ) {
            n_x = n_x + _tab_triangles_normales[_it_triangle->first]._normale[0];
            n_y = n_y + _tab_triangles_normales[_it_triangle->first]._normale[1];
            n_z = n_z + _tab_triangles_normales[_it_triangle->first]._normale[2];
        }

        float norme;
        norme = sqrt(n_x * n_x + n_y * n_y + n_z * n_z);

        n_x = n_x / norme;
        n_y = n_y / norme;
        n_z = n_z / norme;

        _tab_vertex_normales[_it_vertex->first]._normale[0] = n_x;
        _tab_vertex_normales[_it_vertex->first]._normale[1] = n_y;
        _tab_vertex_normales[_it_vertex->first]._normale[2] = n_z;

        /*
        std::cout << "Normale du vertex " <<  _it_vertex->first << std::endl;
        std::cout << " x : " << n_x;
        std::cout << " , y : " << n_y;
        std::cout << " , z : " << n_z;
        std::cout << std::endl;
        */
    }
}


void Mesh::Sensible_Lumiere(){
    _recoit_lumiere = true;
    _projete_ombre  = true;
}



void Mesh::Insensible_Lumiere(){
    _recoit_lumiere = true;
    _projete_ombre  = true;
}




#define INFINI 20


void Mesh::_Construire_Volume_Ombre(const Vecteur3D& source,
                                    int type,
                                    bool fermer_source,
                                    bool fermeture){

    // Si déjà alloué alors il y a déjà le bon nombre de vertex alloués
    if (_shadow_vertex == 0){
        _shadow_vertex = new Vertex[6 * _nb_triangles];
    }

    // Allocation du nombre max de triangles nécessaires
    int nb = 0;
    if (fermeture){
        nb++;
    }
    if (fermer_source){
        nb++;
    }

    if (_nb_shadow_triangles != (6+nb) * _nb_triangles){
        // pas le bon nombre alloué
        delete [] _shadow_triangles;
        _nb_shadow_triangles = (6+nb) * _nb_triangles;
        _shadow_triangles = new Triangles[ _nb_shadow_triangles];
    }

    /*
        std::cout<<" Resumé :"<<std::endl;
        std::cout<<" nb de triangles :"<< _nb_triangles <<std::endl;
        std::cout<<" nb de triangles shadow :"<< _nb_shadow_triangles <<std::endl;
        std::cout<<" nb de vertex :"<< _nb_vertex <<std::endl;
        std::cout<<" nb de vertex shadow :"<< 6 * _nb_triangles <<std::endl;
    */

    // Calcul du volume
    Triangles *triangles = _tab_triangles;
    Normales *normales = _tab_triangles_normales;
    Triangles *triangles_volume = _shadow_triangles;
    Vertex *vertex_volume = _shadow_vertex;

    int vertex_shadow_indice = 0;
    _nb_shadow_triangles = 0;


    for ( int i = 0; i < _nb_triangles ; i ++ )
    {

        // Recopie des 3 premiers vertex
        vertex_volume[0] = _tab_vertex[triangles->_vertexIndices[0]];
        vertex_volume[2] = _tab_vertex[triangles->_vertexIndices[1]];
        vertex_volume[1] = _tab_vertex[triangles->_vertexIndices[2]];

        // La face est-elle visible pour la lumiere ?
        // Si non on ne fait rien
        // Si oui on calcule un volume
        if ( normales->_normale[0] * (source._x - (vertex_volume[0]._position[0] + vertex_volume[1]._position[0] + vertex_volume[2]._position[0])/3)
             + normales->_normale[1] * (source._y - (vertex_volume[0]._position[1] + vertex_volume[1]._position[1] + vertex_volume[2]._position[1])/3)
             + normales->_normale[2] * ( source._z - (vertex_volume[0]._position[2] + vertex_volume[1]._position[2] + vertex_volume[2]._position[2])/3) > 0 )
        {

        if (type == LUMIERE_POINT)
        {
            // Lumiere point
            // Positionne les 3 nouveau vertex
            vertex_volume[3]._position[0] = (vertex_volume[0]._position[0] - source._x) * INFINI;
            vertex_volume[3]._position[1] = (vertex_volume[0]._position[1] - source._y) * INFINI;
            vertex_volume[3]._position[2] = (vertex_volume[0]._position[2] - source._z) * INFINI;

            vertex_volume[4]._position[0] = (vertex_volume[1]._position[0] - source._x) * INFINI;
            vertex_volume[4]._position[1] = (vertex_volume[1]._position[1] - source._y) * INFINI;
            vertex_volume[4]._position[2] = (vertex_volume[1]._position[2] - source._z) * INFINI;

            vertex_volume[5]._position[0] = (vertex_volume[2]._position[0] - source._x) * INFINI;
            vertex_volume[5]._position[1] = (vertex_volume[2]._position[1] - source._y) * INFINI;
            vertex_volume[5]._position[2] = (vertex_volume[2]._position[2] - source._z) * INFINI;

        }
        else
        {
            // Lumiere directionnelle
            vertex_volume[3]._position[0] = vertex_volume[0]._position[0] + source._x * INFINI;
            vertex_volume[3]._position[1] = vertex_volume[0]._position[1] + source._y * INFINI;
            vertex_volume[3]._position[2] = vertex_volume[0]._position[2] + source._z * INFINI;

            vertex_volume[4]._position[0] = vertex_volume[1]._position[0] + source._x * INFINI;
            vertex_volume[4]._position[1] = vertex_volume[1]._position[1] + source._y * INFINI;
            vertex_volume[4]._position[2] = vertex_volume[1]._position[2] + source._z * INFINI;

            vertex_volume[5]._position[0] = vertex_volume[2]._position[0] + source._x * INFINI;
            vertex_volume[5]._position[1] = vertex_volume[2]._position[1] + source._y * INFINI;
            vertex_volume[5]._position[2] = vertex_volume[2]._position[2] + source._z * INFINI;
        }

        // Affichage du polygone sombre de la Source
        if (fermer_source){
            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 0;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 2;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 1;
            triangles_volume++;
            _nb_shadow_triangles++;
        }


        // Affichage du polygone sombre de la fermeture
        if (fermeture){
            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 3;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 4;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 5;
            triangles_volume++;
            _nb_shadow_triangles++;
        }

            // Affichage du tube
            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 0;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 3;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 5;
            triangles_volume++;
            _nb_shadow_triangles++;


            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 0;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 5;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 2;
            triangles_volume++;
            _nb_shadow_triangles++;

            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 1;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 4;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 3;
            triangles_volume++;
            _nb_shadow_triangles++;

            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 1;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 3;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 0;
            triangles_volume++;
            _nb_shadow_triangles++;

            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 2;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 5;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 4;
            triangles_volume++;
            _nb_shadow_triangles++;

            triangles_volume->_vertexIndices[0] = vertex_shadow_indice + 2;
            triangles_volume->_vertexIndices[1] = vertex_shadow_indice + 4;
            triangles_volume->_vertexIndices[2] = vertex_shadow_indice + 1;
            triangles_volume++;
            _nb_shadow_triangles++;

            // MAJ des indices vertex
            vertex_volume  += 6;
            vertex_shadow_indice += 6;
            triangles++;
            normales++;

        }
        else{
            triangles++;
            normales++;
        }


    }

}







