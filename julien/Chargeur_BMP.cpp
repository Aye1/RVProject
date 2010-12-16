


#include "Chargeur_BMP.h"

/** Charge un fichier BMP et renvoie la structure contenant les données */
    Image* Charger_Texture_BMP(std::string __nom)
    {
        // Ouverture du fichier
        ifstream fichier (__nom.c_str());

        if (!fichier.is_open()){
            cout << "Impossible d'ouvrir le fichier "<< __nom << " en écriture !" << endl;
            return NULL;
        }

        // Creation d'une image
        Image *res = new Image;

        // Verification du format
        Header_BMP header;

        // Header_BMP
        fichier.seekg ( 2, std::ios::beg);
        fichier.read ((char*)(&header), sizeof (Header_BMP));


        std::cout << "Chargement de "
                       << __nom
//                       << " : taille "
//                       << header.taille_fichier << "\n"
//                       << " : reserve "
//                       << header.reserve << "\n"
//                       << " : offset "
//                       << header.ofs_image << "\n"
//                       << " : taille header "
//                       << header.taille_header << "\n"
                       << " : LxH : "
                       << header.largeur
                       << "x"
                       << header.hauteur
//                       << " : plans : "
//                       << header.plans << "\n"
                       << " : format : "
                       << header.format
//                       << header.compression << "\n"
//                       << " : "
//                       << header.taille << "\n"
//                       << " : "
//                       << header.resolution_h << "\n"
//                       << " : "
//                       << header.resolution_v << "\n"
//                       << " : "
//                       << header.nb_couleurs << "\n"
//                       << " : "
//                       << header.nb_couleurs_importantes << "\n"
                       << "  ";



        res->format = (FORMAT)header.format;
        res->width = header.largeur;
        res->height = header.hauteur;

        // Positionnement en debut de données
        fichier.seekg ( header.ofs_image, std::ios::beg);

        // Format RGB
        if (res->format == RGB) {

            // Allocation de la texture
            res->texture = (char*) malloc( res->width * res->height * 3);

            // Copie de la texture en mémoire
            fichier.read ((char*)(res->texture), res->width * res->height * 3);
            //fread(res->texture, res->width * res->height * 3, 1, fichier);

            // Inverse les couleurs (bgr -> rgb)
            char temp;
            for(int i=0; i< res-> width * res->height * 3  ; i+=3){
                temp = res->texture[i];
                res->texture[i] = res->texture[i+2];
                res->texture[i+2] = temp;
            }
            // ferme le fichier
            fichier.close();

            // tout s'est bien passé
            cout << " réussit" << endl;
            return res;
        }

        // Format RGBA
        if (res->format == RGBA) {

            // Allocation de la texture
            res->texture = (char*) malloc( res->width * res->height * 4);

            // Copie de la texture en mémoire
            fichier.read ((char*)(res->texture), res->width * res->height * 4);

            // Inverse les couleurs (bgr -> rgb)
            char temp;
            for(int i=0; i< res-> width * res->height * 4; i+=4){
                temp = res->texture[i];
                res->texture[i] = res->texture[i+2];
                res->texture[i+2] = temp;
            }

            // ferme le fichier
            fichier.close();

            // tout s'est bien passé
            cout << " réussit" << endl;
            return res;
        }

        // Le format est incorrect.
        std::cerr << "Format BMP Incorrect" << endl;
        return NULL;
    }

