#include <ApiArv/Fob.h>

/* Initialise le FOB et tente d'établir la connexion physique */
int init(Fob* fob);

/* Récupère la position et l'orientation actuelles du FOB */
int getPosAndOri(Fob* fob, Vec& pos, Quaternion& ori);

/* Ferme la connexion avec le FOB */
int close(Fob* fob);

