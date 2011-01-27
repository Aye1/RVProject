#ifndef LISTE_TOUCHES_H_
#define LISTE_TOUCHES_H_
#include "touche.h"
#include <QList>
#include "vec.h"

class AGHListeTouches : public QList<Touche> {
public:
	AGHListeTouches();
	void addNotes(int notes);
	void updateNotesPos();
	void setPosElemVert(Vec pos);
	void setPosElemRouge(Vec pos);
	void setPosElemJaune(Vec pos);
	void setPosElemBleu(Vec pos);
	void setPosElems(Vec posV, Vec posR, Vec posJ, Vec posB);
private:
	Vec _posElemVert;
	Vec _posElemRouge;
	Vec _posElemJaune;
	Vec _posElemBleu;
};

#endif