#include <ApiArv/Fob.h>
#include  <QGLViewer/qglviewer.h>

class myFob{

public:

myFob(){};
/* Initialise le FOB et tente d'établir la connexion physique */
int initFob();

/* Récupère la position et l'orientation actuelles du FOB */
int getPosAndOriFob(qglviewer::Vec& pos, qglviewer::Quaternion& ori);

/* Ferme la connexion avec le FOB */
int closeFob();

protected:
arv::Fob* fob;
}

