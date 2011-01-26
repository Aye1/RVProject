/*
 * touche.h
 *
 *  Created on: 26 janv. 2011
 *      Author: laura
 */

#ifndef TOUCHE_H_
#define TOUCHE_H_
#include "QGLViewer/qglviewer.h"
#include "color.h"

class Touche {
protected:
	qglviewer::Vec position_;
	float rayon_;
	Color color_;
	float angleRotation_;
	float inclinaison_;
	GLUquadric* touche_;

public:
	Touche();

	virtual ~Touche(){}
	void setPosition(qglviewer::Vec& pos){
		position_=pos;
	}
	void setInclinaison(float incl){
		inclinaison_=incl;
	}
	void setAngleRotation(float angle){
		angleRotation_=angle;
	}
	void setColor(Color& c){
		color_=c;
	}
	void draw();

};

#endif /* TOUCHE_H_ */
