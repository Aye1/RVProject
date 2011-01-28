
#ifndef ELEMENTBAT__H
#define ELEMENTBAT__H

#include <cmath>
#include "QGLViewer/qglviewer.h"
#include "material.h"

using namespace qglviewer;

class ElementBat
{

private:

	qglviewer::Vec center_; // a mettre a jour quand on cree l'objet

	qglviewer::Vec positionCenterBat_; // a mettre a jour quand on dessine a chaque fois
	
	//A lire avec le .scn
	float rayonBat_;
	float heightBat_;
	float heightPied_;
	float rayonPied_;
	float rayonBase_;

	//initialement a 0, a regler pour chaque élément
	float inclinaison_;
	float angleRotationBat_;

	//float pour la translation générale de l'élément
	float TX,TY,TZ;

	GLUquadric* batterie_;
	GLUquadric* pied_;
	GLUquadric* base_;
	
public:

	/* constructors destructors */
	ElementBat();
	~ElementBat();

	/*creation et initialisation de l'élément*/
	void createElement(Vec centre, Vec posBat = Vec(0.0,0.0,0.0),
					   float RBat = 0, float HBat = 0, float RPied = 0, float HPied = 0, float RBase = 0);

	/* draw */
	virtual void draw(bool validated, Color c) ;
	virtual float boundingRadius() const { return radius(); }

	// faire un scale général de l'élément
	virtual void scaleBat(float s);
	virtual void translateBat(float x,float y, float z);
	virtual void rotateBatInclinaison(float angle);
	virtual void rotateBatAngleTambour(float angle);

	/* getters */
	Vec getCenter();
	Vec getPositionCenterBat();
	float getRayonBat();
	float getHeightBat();
	float getHeightPied();
	float getRayonPied();
	float getRayonBase();	
	float getInclinaison();
	float getAngleRotationBat();

	float radius() const { return 1000.0; }

	/*setters */
	void setCenter(Vec r);
	void setPositionCenterBat(Vec r);
	void setRayonBat(float r);	
	void setHeightBat(float r);	
	void setHeightPied(float r);
	void setRayonPied(float r);
	void setRayonBase(float r);
	void setInclinaison(float r);
	void setAngleRotationBat(float r);


	virtual void initFromDOMElement(const QDomElement& e);

	const qglviewer::Frame& frame() const { return frame_; }
	const Material& material() const { return material_; }

protected :
	// Tous les objets ont un repere et un materiau.
	qglviewer::Frame frame_;
	Material material_;
};


#endif // ELEMENTBAT__H
