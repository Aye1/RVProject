#ifndef MYFOB_H_
#define MYFOB_H_

#include <ApiArv/Fob.h>
#include  <QGLViewer/qglviewer.h>


class myFob {

public:

	myFob(){};
	/* Initialise le FOB et tente d'établir la connexion physique */
	int init();

	/* Récupère la position et l'orientation actuelles du FOB */
	int getPosAndOri(qglviewer::Vec& pos, qglviewer::Quaternion& ori);

	/* Ferme la connexion avec le FOB */
	int close();

protected:
	arv::Fob* fob;
};

#endif
