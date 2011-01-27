#ifndef LISTE_TOUCHES_H_
#define LISTE_TOUCHES_H_
#include "touche.h"
#include <QList>

class AGHListeTouches : public QList<Touche> {
public:
	AGHListeTouches();
	void addNotes(int notes);
	void updateNotesPos();
};

#endif