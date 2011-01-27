#include "AGHListeTouches.h"
#include <stdio.h>
using namespace qglviewer;

AGHListeTouches::AGHListeTouches() {
	_posElemVert = Vec(0,0,0);
	_posElemRouge = Vec(0,0,0);	
	_posElemJaune = Vec(0,0,0);
	_posElemBleu = Vec(0,0,0);
}

void AGHListeTouches::addNotes(int notes) {
	printf("add note \n");
	Vec dir;
	if (notes & 2) {
		Touche tmp;
		dir = calcPosition(_posElemVert) - _posElemVert;
		dir.normalize();
		tmp.setColor(Color(0.0f, 1.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 4) {
		Touche tmp;
		dir = calcPosition(_posElemRouge) - _posElemRouge;
		dir.normalize();
		tmp.setColor(Color(1.0f, 0.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 8) {
		Touche tmp;
		dir = calcPosition(_posElemJaune) - _posElemJaune;
		dir.normalize();
		tmp.setColor(Color(1.0f, 1.0f, 0.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	if (notes & 16) {
		Touche tmp;
		dir = calcPosition(_posElemBleu) - _posElemBleu;
		dir.normalize();
		tmp.setColor(Color(0.0f, 0.0f, 1.0f));
		tmp.setDirection(dir);
		this->append(tmp);
	}
	printf("fin add note \n");
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

Vec AGHListeTouches::calcPosition(Vec posBat){
	float a=(float)(posBat.y+30)/(float)posBat.x;
	float b=30;
	qglviewer::Vec newPos;
	newPos.y=posBat.y+200;
	newPos.x=(float)(newPos.y-b)/(float)a;
	newPos.z=posBat.z;
	return newPos;
}
