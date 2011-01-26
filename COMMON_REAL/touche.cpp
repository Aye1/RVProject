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
	color_=Color(1.0,0.0,0.0);//couleur rouge de base
}

void Touche::draw(){
	const int slices = 100;
	const int stacks = 50 ;

	glPushMatrix();
	glTranslated(position_.x,position_.y,position_.z);
	glColor3b(color_.r,color_.g,color_.b);
	glRotated(angleRotation_,0,0,1);
	glRotated(inclinaison_,0,1,0);
	gluDisk(touche_,0,rayon_,slices,stacks);
	glPopMatrix();
}
