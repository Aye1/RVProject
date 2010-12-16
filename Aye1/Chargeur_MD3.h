#ifndef _Chargeur_MD3_H
#define _Chargeur_MD3_H


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Mesh.h"

/** Structures permettant le chargement rapide du fichier MD3 */
// MD3 header
struct Header_MD3
{
  int  ident;
  int  version;

  char name[64];
  int  flags;

  int  num_frames;
  int  num_tags;
  int  num_meshes;
  int  num_skins;

  int  ofs_frames;
  int  ofs_tags;
  int  ofs_surfaces;
  int  ofs_eof;
};

// Frame
struct Frame
{
  float min_bounds[3];
  float max_bounds[3];
  float local_origin[3];
  float radius;
  char  name[16];
};

// Tag
struct Tag
{
  char  name[64];
  float origin[3];
  float axis[3][3];
};

// Surface
struct Surface
{
  int  ident;
  char name[64];
  int  flags;

  int  num_frames;
  int  num_shaders;
  int  num_verts;
  int  num_triangles;

  int  ofs_triangles;
  int  ofs_shaders;
  int  ofs_st;
  int  ofs_xyznormal;
  int  ofs_end;
};

// Shader
struct Shader
{
  char name[64];
  int  shader_index;
};

// Triangle
struct Triangle
{
  int indexes[3];
};

// TexCoord
struct TexCoord
{
  float	st[2];
};

// Vertex
struct Vertexes
{
  short coord[3];
  unsigned char normal[2];
};



/** Methode amie de MESH.h */
/** Charge un mesh à partir d'un fichier MD3 et renvoie le Mesh correspondant */
    Mesh* Charger_MD3(std::string __nom_du_fichier);

#endif



