#include "AGHListeTouches.h"

using namespace qglviewer;

AGHListeTouches::AGHListeTouches() {
}

void AGHListeTouches::addNotes(int notes) {
	Vec pos(-5.0f, 0.0f, 0.0f);
	Vec dir;
	if (notes & 2) {
		Touche tmp;
		dir = pos - _posElemVert;
		dir.normalize();
		tmp.setColor(Color(0.0f, 1.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 4) {
		Touche tmp;
		dir = pos - _posElemRouge;
		dir.normalize();
		tmp.setColor(Color(1.0f, 0.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 8) {
		Touche tmp;
		dir = pos - _posElemJaune;
		dir.normalize();
		tmp.setColor(Color(1.0f, 1.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 16) {
		Touche tmp;
		dir = pos - _posElemBleu;
		dir.normalize();
		tmp.setColor(Color(0.0f, 0.0f, 1.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
}

void AGHListeTouches::updateNotesPos() {
	Touche t;	
	foreach(t, *this) {
		Vec pos = t.getPosition();
		t.setPosition(pos + t.getDirection());	
	}
}

void AGHListeTouches::setPosElemVert(Vec pos) {
	_posElemVert = pos;
}

void AGHListeTouches::setPosElemRouge(Vec pos) {
	_posElemRouge = pos;
}

void AGHListeTouches::setPosElemJaune(Vec pos) {
	_posElemJaune = pos;
}

void AGHListeTouches::setPosElemBleu(Vec pos) {
	_posElemBleu = pos;
}

void AGHListeTouches::setPosElems(Vec posV, Vec posR, Vec posJ, Vec posB) {
	setPosElemVert(posV);
	setPosElemRouge(posR);
	setPosElemJaune(posJ);
	setPosElemBleu(posB);
}
