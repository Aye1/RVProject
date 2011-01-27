#include "touche.h"
#include <QList>

class AGHListeTouches : public QList<Touche> {
public:
	AGHListeTouches();
	void addNotes(int notes);
	void updateNotesPos();
};