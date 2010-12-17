#include "Chargeur_MD3.h"
#include "Debug.h"

#include <iostream>
#include <fstream>

Mesh* Charger_MD3(std::string __nom_du_fichier){

    // Ouverture du fichier
    ifstream fichier (__nom_du_fichier.c_str());

    if (!fichier.is_open()){
            cout << "Impossible d'ouvrir le fichier "<< __nom_du_fichier << " en écriture !" << endl;
        return NULL;
    }

    // HEADER
    Header_MD3 header;
    fichier.read ((char*)&header, sizeof (Header_MD3));

    // Surfaces
    Surface surface;
    fichier.seekg ( header.ofs_surfaces, std::ios::beg);
    fichier.read ((char*)&surface, sizeof (Surface));
    // Nouveau mesh
    Mesh *objet_charge = new Mesh(surface.num_verts, surface.num_triangles);

    // Vertexes
    fichier.seekg ( header.ofs_surfaces + surface.ofs_xyznormal, std::ios::beg);
    Vertexes vert;
    for (int i = 0; i < surface.num_verts; ++i)
    {
        fichier.read ((char*)&vert, sizeof (Vertexes));
        //std::cout << "Vertex 0 " << vert.coord[0] << std::endl;
        //std::cout << "Vertex 1 " << vert.coord[1] << std::endl;
        //std::cout << "Vertex 2 " << vert.coord[2] << std::endl;
        objet_charge->_tab_vertex[i]._position[0] = (float) vert.coord[0]*1.0/64;
        objet_charge->_tab_vertex[i]._position[1] = (float) vert.coord[2]*1.0/64;
        objet_charge->_tab_vertex[i]._position[2] = (float) vert.coord[1]*1.0/64;
    }

    // Triangle
    fichier.seekg ( header.ofs_surfaces + surface.ofs_triangles, std::ios::beg);
    Triangle tri;
    for (int i = 0; i < surface.num_triangles; ++i)
    {
        fichier.read ((char*)&tri, sizeof (Triangle));
        objet_charge->_tab_triangles[i]._vertexIndices[0] = tri.indexes[0];
        objet_charge->_tab_triangles[i]._vertexIndices[1] = tri.indexes[1];
        objet_charge->_tab_triangles[i]._vertexIndices[2] = tri.indexes[2];
    }

    // TextCoord
    fichier.seekg ( header.ofs_surfaces + surface.ofs_st, std::ios::beg);
    TexCoord tex;
    for (int i = 0; i < surface.num_verts; ++i)
    {
        fichier.read ((char*)&tex, sizeof (TexCoord));
        objet_charge->_tab_map[i]._map[0] = tex.st[0];
        objet_charge->_tab_map[i]._map[1] = tex.st[1];
    }


    fichier.close();

    objet_charge->_visible = true;

    return objet_charge;


}

