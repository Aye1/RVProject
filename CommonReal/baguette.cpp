#include "material.h"
#include <iostream>
#include "baguette.h"

#include <qmessagebox.h>

using namespace std;

// Pour ne pas avoir a mettre qglviewer:: devant les classes
// definies dans le namespace qglviewer.
using namespace qglviewer;


Baguette::Baguette()
{
	center_               = Vec(0,0,0);
	positionBoutBaguette_ = Vec(0,0,0);
	directionBaguette_    = Vec(0,1,0);// vecteur direction non nul please

	
	rayonBaguette_   = 1;
	heightBaguette_  = 1;

	baguette_     = gluNewQuadric();
	
	TX=0;
	TY=0;
	TZ=0;
}

Baguette::~Baguette()
{
	gluDeleteQuadric(baguette_);
}


void Baguette::draw() const
{
	const int slices = 100;
    const int stacks = 50 ;

	Vec Normdir = directionBaguette_.unit();
	float arg = Normdir.x/sqrt(Normdir.x*Normdir.x+Normdir.y*Normdir.y);
	float angletheta , anglephi;

	//cout << "X=" << Normdir.x << " Y=" << Normdir.y << " Z=" << Normdir.z << " norm=" << Normdir.norm() << endl;

	if(Normdir.x == 0) arg = 0;

	if(Normdir.y>=0) {
		angletheta = acos(arg);
	} else {
		angletheta = 2*M_PI - acos(arg);
	}
	anglephi = acos(Normdir.z);
	
	anglephi *= 180.0/M_PI;
	angletheta *= 180.0/M_PI;
	//cout << "angletheta=" << angletheta << endl;
	//cout << "anglephi=" << anglephi << endl;
	//gluQuadricDrawStyle(baguette_,GLU_LINE);

	glPushMatrix();//transfo reversibles	
	//on se place au centre de l'élément = centre du pied
	glMultMatrixd(frame().matrix());
	glTranslated(TX,TY,TZ);
	glColor3fv(material().diffuseColor());

	//on s'oriente selon Normdir
	glRotated(angletheta,0,0,1);
	glRotated(anglephi,0,1,0);
	
	/* 
	//DEBUG
	glBegin(GL_LINES);
	glColor3f(1,0,0);
	glVertex3f(0,0,0);glVertex3f(1,0,0);
	glColor3f(0,1,0);
	glVertex3f(0,0,0);glVertex3f(0,1,0);
	glColor3f(0,0,1);
	glVertex3f(0,0,0);glVertex3f(0,0,1);
	glEnd();
	*/	

	//on commence par le bas de la baguette:
	gluDisk(baguette_,0,rayonBaguette_,slices,stacks);
	gluCylinder(baguette_,rayonBaguette_,rayonBaguette_,0.8*heightBaguette_,slices,stacks);
	glTranslatef(0 ,
				 0 ,
				 0.8*heightBaguette_   
				);
	gluDisk(baguette_,0,rayonBaguette_,slices,stacks);
	gluCylinder(baguette_,rayonBaguette_,rayonBaguette_/1.5,0.2*heightBaguette_,slices,stacks);
 	gluDisk(baguette_,0,rayonBaguette_/1.5,slices,stacks);
	glTranslatef(0 ,
				 0 ,
				 0.2*heightBaguette_   
				);
	gluSphere(baguette_,rayonBaguette_/1.3,slices,stacks);

	glPopMatrix();
	
	//MISE A JOUR DE LA POSITION DU BOUT DE LA BAGUETTE.
	//TODO

}

void Baguette::scaleBaguette(float s)
{
	rayonBaguette_   /= s;
	heightBaguette_  /= s;
}
void Baguette::translateBaguette(float x,float y, float z)
{
	center_            += Vec(x,y,z);
	directionBaguette_ += Vec(x,y,z);
	TX=x;
	TY=y;
	TZ=z;
}


void Baguette::initFromDOMElement(const QDomElement& e)
{
	QDomNode n = e.firstChild();
    while (!n.isNull())
    {      
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "Material")
	    material_.initFromDOMElement(e);
	  else
	    if (e.tagName() == "Frame")
	      {
		frame_.initFromDOMElement(e);

		// Patch : Make sure the orientation is normalized.
		// Absolutely needed to correctly rotate rays.
		qglviewer::Quaternion o = frame_.orientation();
		o.normalize();
		frame_.setOrientation(o);
	      }
	}
    else
	QMessageBox::warning(NULL, "Object XML error", "Error while parsing Object XML document");
      n = n.nextSibling();
    }

	//Object::initFromDOMElement(e);
	if (e.hasAttribute(QString("rayBaguette"))){
		setRayonBaguette(e.attribute(QString("rayBaguette")).toFloat());
	}
	if (e.hasAttribute(QString("heiBaguette"))){
		setHeightBaguette(e.attribute(QString("heiBaguette")).toFloat());
	}
}



	/* getters */
Vec Baguette::getCenter()
{
	return center_;
}
Vec Baguette::getPositionBoutBaguette()
{
	return positionBoutBaguette_;
}
Vec Baguette::getDirectionBaguette()
{
	return directionBaguette_;
}	
float Baguette::getRayonBaguette()
{
	return rayonBaguette_;
}	
float Baguette::getHeightBaguette()
{
	return heightBaguette_;
}

	/*setters */
void Baguette::setCenter(Vec r)
{
	center_ = r ;
}
void Baguette::setPositionBoutBaguette(Vec r)
{ 
	positionBoutBaguette_ = r ;
}
void Baguette::setDirectionBaguette(Vec r)
{
	directionBaguette_ = r;
}	
void Baguette::setRayonBaguette(float r)
{
	rayonBaguette_ = r ;
}	
void Baguette::setHeightBaguette(float r)
{
	heightBaguette_ = r ;
}