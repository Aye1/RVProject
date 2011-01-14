#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <qimage.h>
#include <QtOpenGL/QGLWidget>

namespace arv {

    class Vertex {
        public:
            float x,y,z;
    };

    class Normal {
        public:
            float x,y,z;
    };

    class TexCoord {
        public:
            float u,v,w;
    };

    class Face {
        public:
        std::vector<int> vertex;
        std::vector<int> normal;
        std::vector<int> texCoord;
    };

    class Triangle {
        public:
            int vertex[3];
            int normal[3];
            int texCoord[3];
    };

    class Material {
        public:
            float Ka[4];
            float Kd[4];
            float Ks[4];
            float d;
            float Ns;
            int illum;
            std::string map_Kd;
            GLuint texture;
            std::string name;
    };

    class Pair{
        public:
            int id_mat;
            int id_face;
    };
    class ObjLoader {
        public:
            ObjLoader(char * objfile);
            ~ObjLoader();
            void render();
            bool isInit() {return loaded;};

        private:
            std::vector<Vertex> vertices;
            std::vector<Normal> normals;
            std::vector<TexCoord> texCoords;
            std::vector<Face> faces;
            std::vector<Material> materials;
            std::vector<Pair> pairs;

            Vertex * verticesA;
            Normal * normalsA;
            TexCoord * texCoordsA;
            Triangle * facesA;
            unsigned int nbFaces;
            unsigned int nbVertices;
            unsigned int nbNormals;
            unsigned int nbTexCoords;


            std::ifstream f;
            bool loaded;
            bool hasmtllib;
            std::string mtllib;
            std::string path;

            void init();
            bool load();
            void alloc();

            bool extractTexCoord(std::string s);
            bool extractNormal(std::string s);
            bool extractVertex(std::string s);
            bool extractFace(std::string s);
            bool extractmtllib(std::string s);
            bool extractusemtl(std::string s);
            void loadTexture(Material & mat);
            void pushMaterial(Material const & m);

    };




};
#endif
