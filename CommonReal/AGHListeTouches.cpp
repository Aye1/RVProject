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

void AGHListeTouches::addNotes(int notes,QList<ElementBat *> liste) {
	//printf("add note \n");
	_posElemVert = liste[0]->getPositionCenterBat();
	_posElemRouge = liste[1]->getPositionCenterBat();	
	_posElemJaune = liste[2]->getPositionCenterBat();
	_posElemBleu = liste[3]->getPositionCenterBat();
	Vec dir;
	//std::cout<<"position vert: "<<calcPosition(_posElemVert).x<<" "<<calcPosition(_posElemVert).y<<" "<<calcPosition(_posElemVert).z<<std::endl;
	//std::cout<<"position rouge: "<<calcPosition(_posElemRouge).x<<" "<<calcPosition(_posElemRouge).y<<" "<<calcPosition(_posElemRouge).z<<std::endl;
	//std::cout<<"position jaune: "<<calcPosition(_posElemJaune).x<<" "<<calcPosition(_posElemJaune).y<<" "<<calcPosition(_posElemJaune).z<<std::endl;
	//std::cout<<"position bleu: "<<calcPosition(_posElemBleu).x<<" "<<calcPosition(_posElemBleu).y<<" "<<calcPosition(_posElemBleu).z<<std::endl;
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
	//printf("fin add note \n");
}

void AGHListeTouches::updateNotesPos() {
	Touche * t;
	int i=0;	
	foreach(t, *this) {
		Vec pos = t->getPosition();
		t->setPosition(pos + t->getDirection());
		if(t->getPosition().y < _posElemVert.y - 30){
		  this->removeAt(i);
		}
	i++;	
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
