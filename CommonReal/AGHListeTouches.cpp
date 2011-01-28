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
		tmp->setColor(Color(0.1f, 0.9f, 0.1f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 4) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemRouge) - _posElemRouge);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemRouge));
		tmp->setColor(Color(0.9f, 0.1f, 0.1f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 8) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemJaune) - _posElemJaune);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemJaune));
		tmp->setColor(Color(0.9f, 0.9f, 0.1f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
	if (notes & 16) {
		Touche * tmp;
		tmp = new Touche();
		dir = -(calcPosition(_posElemBleu) - _posElemBleu);
		dir.normalize();
		tmp->setPosition(calcPosition(_posElemBleu));
		tmp->setColor(Color(0.1f, 0.1f, 0.9f));
		tmp->setDirection(dir);
		this->append(tmp);
	}
}

void AGHListeTouches::updateNotesPos(QList<ElementBat *> liste) {
	Touche * t;
	int i=0;	
	foreach(t, *this) {
		ElementBat* associated;
		Vec pos = t->getPosition();
		t->setPosition(pos + t->getDirection());
		//Adaptation des inclinaisons
		ElementBat* bat;
		foreach(bat, liste) {
			Color color1=bat->material().diffuseColor();
			if(color1.r==t->getColor().r && color1.g==t->getColor().g && color1.b==t->getColor().b){
				associated=bat;
				t->setInclinaison(bat->getInclinaison());
				t->setAngleRotation(bat->getAngleRotationBat());
			} 
		}
		//suppression des touches passé une certaine limite
		if(t->getPosition().y < associated->getPositionCenterBat().y - 80){	  
		   this->removeAt(i);
		   i--;
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
	float a2=(1+pow(a,2));
	float b2=2*(a*(b-posBat.y)-posBat.x);
	float c2=pow(posBat.x,2)+pow(b-posBat.y,2)-10000;
	float delta=pow(b2,2)-4*a2*c2;
	float x;
	x=(-b2+sqrt(delta))/(2*a2);	
	qglviewer::Vec newPos;
	newPos.y=a*x+b;
	if(newPos.y<=0){
	  x=(-b2-sqrt(delta))/(2*a2);
	  newPos.y=a*x+b;
	}	
	newPos.x=x;
	newPos.z=posBat.z;	
	return newPos;
}
