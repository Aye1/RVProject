#ifndef LISTE_TOUCHES_H_
#define LISTE_TOUCHES_H_
#include "touche.h"
#include <QList>
#include <QGLViewer/qglviewer.h>
#include "elementBat.h"

using namespace qglviewer;

class AGHListeTouches : public QList<Touche *> {
public:
	AGHListeTouches(QList<ElementBat *> liste);
	void addNotes(int notes,QList<ElementBat *> liste);
	void updateNotesPos();
	void setPosElemVert(Vec pos);
	void setPosElemRouge(Vec pos);
	void setPosElemJaune(Vec pos);
	void setPosElemBleu(Vec pos);
	void setPosElems(Vec posV, Vec posR, Vec posJ, Vec posB);
	Vec calcPosition(Vec posBat);
private:
	Vec _posElemVert;
	Vec _posElemRouge;
	Vec _posElemJaune;
	Vec _posElemBleu;
};

#endif
