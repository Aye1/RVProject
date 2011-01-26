#include <ApiArv/Fob.h>

class myFob{

public:

myFob(){};
/* Initialise le FOB et tente d'établir la connexion physique */
int initFob();

/* Récupère la position et l'orientation actuelles du FOB */
int getPosAndOriFob(Vec& pos, Quaternion& ori);

/* Ferme la connexion avec le FOB */
int closeFob();

protected:
Fob* fob;
}

