/*
 * touche.h
 *
 *  Created on: 26 janv. 2011
 *      Author: laura
 */

#ifndef TOUCHE_H_
#define TOUCHE_H_
#include <QGLViewer/qglviewer.h>
#include "color.h"
#include <iostream>

using namespace std;


class Touche {
protected:
	qglviewer::Vec position_;
	qglviewer::Vec direction_;
	float rayon_;
	Color color_;
	float angleRotation_;
	float inclinaison_;
	GLUquadric* touche_;


public:
	Touche();

	~Touche(){gluDeleteQuadric(touche_);};
	void setPosition(qglviewer::Vec pos){
		cout << "creation touche" << endl;
		position_=pos;
	};
	void setInclinaison(float incl){
		inclinaison_=incl;
	};
	void setAngleRotation(float angle){
		angleRotation_=angle;
	};
	void setColor(Color& c){
		color_=c;
	};
	void draw();
	void setDirection(qglviewer::Vec dir) {
		direction_ = dir;
	}

	qglviewer::Vec getDirection();
};


#endif /* TOUCHE_H_ */
