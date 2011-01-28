#include "material.h"
#include <iostream>
#include "elementBat.h"

#include <qmessagebox.h>

using namespace std;

// Pour ne pas avoir a mettre qglviewer:: devant les classes
// definies dans le namespace qglviewer.
using namespace qglviewer;


ElementBat::ElementBat()
{
	center_ = Vec(0,0,0);

	positionCenterBat_ = Vec(0,0,0);
	rayonBat_ = 1.0;
	heightBat_ = 1.0;
	heightPied_ = 1.0;
	rayonPied_ = 1.0;
	rayonBase_ = 1.0;
	
	angleRotationBat_ = 0.0;
	inclinaison_ = 0.0;

	batterie_ = gluNewQuadric();
	pied_     = gluNewQuadric();
	base_     = gluNewQuadric();
	
	TX=0;TY=0;TZ=0;
}

ElementBat::~ElementBat()
{
	gluDeleteQuadric(batterie_);
	gluDeleteQuadric(pied_);
	gluDeleteQuadric(base_);
}

void ElementBat::createElement(Vec centre, Vec posBat, float RBat, float HBat, float RPied, float HPied, float RBase)
{
/*creer tout l'élément : la batterie -> un cylindre
			 le pied   -> un autre cylindre
		         la base   -> un rectangle        
			
			 le centre de l'objet (center_) est au centre du cylindre pied.
*/	
		
	center_            = centre;

	positionCenterBat_ = posBat;
	rayonBat_          = RBat;
	heightBat_         = HBat;
	heightPied_        = HPied;
	rayonPied_         = RPied;
	rayonBase_         = RBase;

	angleRotationBat_ = 0.0;
	inclinaison_ = 0.0;

	TX = 0; 
	TY = 0;
	TZ = 0;
}

void ElementBat::draw(bool validated, Color c) 
{
	const int slices = 100;
	const int stacks = 50 ;

	float Htrepied = heightPied_/2		;
	float angleTetra = 90 - acos(1/sqrt(3))*180/M_PI;

	glPushMatrix();//transfo reversibles	
	//on se place au centre de l'élément = centre du pied
	glMultMatrixd(frame().matrix());
	glTranslated(TX,TY,TZ);

	glColor3fv(material().diffuseColor());

	// on commence par le bas: le trépied
	// on se place au point d'intersection de la base (trépied) et du pied
	glTranslatef(0, 0, -heightPied_/ 2.0);

	//on fait les 3 pieds:
	//pied 1:
	glTranslated(Htrepied*sqrt(3.0)/3.0, 0.0, -(sqrt(2.0)/sqrt(3.0))*Htrepied);
	glRotated(-angleTetra,0,1,0);
	gluDisk(base_,0,rayonPied_/1.8,slices,stacks);
	gluCylinder(base_,rayonPied_/1.8,rayonPied_/1.8,Htrepied,slices,stacks);
	glRotated(angleTetra,0,1,0);
	glTranslated(-Htrepied*sqrt(3.0)/2.0, Htrepied/2.0, 0.0);	
	glRotated(30.0,0,0,1);
	glRotated(angleTetra,1,0,0);
	//pied 2:
	gluDisk(base_,0,rayonPied_/1.8,slices,stacks);	
	gluCylinder(base_,rayonPied_/1.8,rayonPied_/1.8,Htrepied,slices,stacks);
	glRotated(-angleTetra,1,0,0);
	glRotated(-30.0,0,0,1);
	glTranslated(0.0,-Htrepied,0.0);
	glRotated(60.0,0,0,1);
	glRotated(angleTetra,0,1,0);
	//pied 3:
	gluDisk(base_,0,rayonPied_/1.8,slices,stacks);
	gluCylinder(base_,rayonPied_/1.8,rayonPied_/1.8,Htrepied,slices,stacks);
	
	//on se replace a la base :
	glRotated(-angleTetra,0,1,0);
	glRotated(-60.0,0,0,1);
	glTranslated(Htrepied*sqrt(3.0)/6.0,+Htrepied/2.0,+(sqrt(2.0)/sqrt(3.0))*Htrepied);
	gluSphere(pied_,rayonPied_,slices,stacks);
	gluDisk(pied_,0,rayonPied_,slices,stacks);
	gluCylinder(pied_,rayonPied_,rayonPied_,heightPied_,slices,stacks);
	glTranslated(0.0,0.0,heightPied_);
	gluSphere(pied_,rayonPied_,slices,stacks);
	//on va enfin faire le caisson de la batterie.
	//glTranslated(0.0,0.0, -(heightBat_/2.0));
	glRotated(angleRotationBat_,0,0,1);
	glRotated(-inclinaison_,0,1,0);
	//On change la couleur du caisson si la touche est validée
	if (validated) {
	  glColor3f(1.0,1.0,1.0);
	}
	gluDisk(batterie_,0,rayonBat_,slices,stacks);
	gluCylinder(batterie_,rayonBat_,rayonBat_,heightBat_,slices,stacks);
	glTranslated(0.0,0.0,heightBat_);
	gluDisk(batterie_,0,rayonBat_,slices,stacks);


	glPopMatrix();
	


	//MISE A JOUR DE LA POSITION DU CENTRE DE LA BATTERIE.
	positionCenterBat_ = Vec(center_.x,center_.y,center_.z) + 0.5*Vec(0.0,0.0,1.0)*heightPied_ + heightBat_*cos(inclinaison_*M_PI/180.0)*Vec(0.0,0.0,1.0) 
							+ Vec(0.0,-1.0,0.0)*cos(angleRotationBat_*M_PI/180.0)*heightBat_*sin(inclinaison_*M_PI/180.0)
							+ Vec(1.0,0.0,0.0)*sin(angleRotationBat_*M_PI/180.0)*heightBat_*sin(inclinaison_*M_PI/180.0);

}

void ElementBat::scaleBat(float s)
{
	rayonBat_   /= s;
	heightBat_  /= s;
	rayonBat_   /= s;
	heightBat_  /= s;
	heightPied_ /= s;
	rayonPied_  /= s;
	rayonBase_  /= s;
}
void ElementBat::translateBat(float x,float y, float z)
{
	center_            += Vec(x,y,z);
	positionCenterBat_ += Vec(x,y,z);
	TX = x;
	TY = y;
	TZ = z;
}
void ElementBat::rotateBatInclinaison(float angle)
{
	inclinaison_ = angle;
}
void ElementBat::rotateBatAngleTambour(float angle)
{
	angleRotationBat_ = angle;
}

void ElementBat::initFromDOMElement(const QDomElement& e)
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

	if (e.hasAttribute(QString("rayBat"))) {
		setRayonBat(e.attribute(QString("rayBat")).toFloat());
	}
	if (e.hasAttribute(QString("heiBat"))) {
		setHeightBat(e.attribute(QString("heiBat")).toFloat());
	}
	if (e.hasAttribute(QString("rayPied"))) {
		setRayonPied(e.attribute(QString("rayPied")).toFloat());
	}
	if (e.hasAttribute(QString("heiPied"))) {
		setHeightPied(e.attribute(QString("heiPied")).toFloat());
	}
	if (e.hasAttribute(QString("rayBase"))) {
		setRayonBase(e.attribute(QString("rayBase")).toFloat());
	}
}



	/* getters */
Vec ElementBat::getCenter()
{
	return center_;
}
Vec ElementBat::getPositionCenterBat()
{
	return positionCenterBat_;
}
float ElementBat::getRayonBat()
{
	return rayonBat_;
}	
float ElementBat::getHeightBat()
{
	return heightBat_;
}
float ElementBat::getHeightPied()
{
	return heightPied_;
}
float ElementBat::getRayonPied()
{
	return rayonPied_;
}
float ElementBat::getRayonBase()
{
	return rayonBase_;
}
float ElementBat::getInclinaison()
{
	return inclinaison_ ;
}
float ElementBat::getAngleRotationBat()
{
	return angleRotationBat_ ;
}

	/*setters */
void ElementBat::setCenter(Vec r)
{
	center_ = r ;
}
void ElementBat::setPositionCenterBat(Vec r)
{ 
	positionCenterBat_ = r ;
 }
void ElementBat::setRayonBat(float r)
{
	rayonBat_ = r ;
}	
void ElementBat::setHeightBat(float r)
{
	heightBat_ = r ;
}
void ElementBat::setHeightPied(float r)
{
	heightPied_ = r ;
}
void ElementBat::setRayonPied(float r)
{
	rayonPied_ = r ;
}
void ElementBat::setRayonBase(float r)
{
	rayonBase_ = r ;
}	
void ElementBat::setInclinaison(float r)
{
	inclinaison_ = r ;
}
void ElementBat::setAngleRotationBat(float r)
{
	angleRotationBat_ = r ;
}
