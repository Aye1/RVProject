/*
 * touche.cpp
 *
 *  Created on: 26 janv. 2011
 *      Author: laura
 */

#include "touche.h"
#include <iostream>

Touche::Touche() {
	touche_=gluNewQuadric();
	position_=qglviewer::Vec(0.0,0.0,0.0);
	rayon_=1.0;
	color_ = Color(1.0,0.0,0.0);//couleur rouge de base
}

void Touche::draw(){
	const int slices = 100;
	const int stacks = 50 ;

	glPushMatrix();
	glTranslated(position_.x,position_.y,position_.z);
	glColor3f(color_.r,color_.g,color_.b);
	glRotated(angleRotation_,0,0,1);
	glRotated(-inclinaison_,0,1,0);
	gluDisk(touche_,0,rayon_,slices,stacks);
	glPopMatrix();
}

qglviewer::Vec Touche::getDirection() {
	return direction_;
}

void Touche::initPosition(qglviewer::Vec posBat){
	float a=(float)(posBat.y+30)/(float)posBat.x;
	float b=30;
	qglviewer::Vec newPos;
	newPos.y=posBat.y+200;
	newPos.x=(float)(newPos.y-b)/(float)a;
	newPos.z=posBat.z;
	this->setPosition(newPos);

}
