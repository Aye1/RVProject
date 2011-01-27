#include "AGHListeTouches.h"

AGHListeTouches::AGHListeTouches() {
}

void AGHListeTouches::addNotes(int notes) {
	Touche * tmp;
	Vec pos(-5.0f, 0.0f, 0.0f);
	Vec dir();
	if (notes & 2) {
		tmp = new Touche();
		dir = pos - _posElemVert;
		dir.normalize();
		tmp->setColor(0.0f, 1.0f, 0.0f);
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 4) {
		tmp = new Touche();
		dir = pos - _posElemRouge;
		dir.normalize();
		tmp->setColor(1.0f, 0.0f, 0.0f);
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 8) {
		tmp = new Touche();
		dir = pos - _posElemJaune;
		dir.normalize();
		tmp->setColor(1.0f, 1.0f, 0.0f);
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 16) {
		tmp = new Touche();
		dir = pos - _posElemBleu;
		dir.normalize();
		tmp->setColor(0.0f, 0.0f, 1.0f);
		tmp->setDirection(dir);
		this->append(tmp);
	}
}

void AGHListeTouches::updateNotesPos() {
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
