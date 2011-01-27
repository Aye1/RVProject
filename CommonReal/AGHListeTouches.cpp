#include "AGHListeTouches.h"
#include "elementBat.h"
#include <stdio.h>

using namespace qglviewer;

AGHListeTouches::AGHListeTouches(QList<ElementBat *> liste) {
	_posElemVert = liste[0]->getPositionCenterBat();
	_posElemRouge = liste[1]->getPositionCenterBat();	
	_posElemJaune = liste[2]->getPositionCenterBat();
	_posElemBleu = liste[3]->getPositionCenterBat();
}

void AGHListeTouches::addNotes(int notes, QList<ElementBat *> liste) {
	_posElemVert = liste[0]->getPositionCenterBat();
	_posElemRouge = liste[1]->getPositionCenterBat();	
	_posElemJaune = liste[2]->getPositionCenterBat();
	_posElemBleu = liste[3]->getPositionCenterBat();
	Vec dir;
	if (notes & 2) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemVert) - _posElemVert);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemVert));
		tmp->setColor(Color(0.0f, 1.0f, 0.0f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 4) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemRouge) - _posElemRouge);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemRouge));
		tmp->setColor(Color(1.0f, 0.0f, 0.0f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 8) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemJaune) - _posElemJaune);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemJaune));
		tmp->setColor(Color(1.0f, 1.0f, 0.0f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 16) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemBleu) - _posElemBleu);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemBleu));
		tmp->setColor(Color(0.0f, 0.0f, 1.0f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
}

void AGHListeTouches::updateNotesPos() {
	Touche * t;	
	foreach(t, *this) {
		Vec pos = t->getPosition();
		t->setPosition(pos + t->getDirection());	
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

Vec AGHListeTouches::calcPosition(Vec posBat){
	float a=-((float)(posBat.y+30)/(float)posBat.x);
	float b=30;
	qglviewer::Vec newPos;
	newPos.y=-(posBat.y-200);
	newPos.x=(float)(newPos.y-b)/(float)a;
	newPos.z=posBat.z;
	return newPos;
}
