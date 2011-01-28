
#ifndef BAGUETTE__H
#define BAGUETTE__H

#include <cmath>
#include "QGLViewer/qglviewer.h"
#include "material.h"

using namespace qglviewer;

class Baguette
{

private:

	Vec center_ ; // a mettre a jour quand on creer l'objet

	Vec positionBoutBaguette_ ; // a mettre a jour quand on dessine a chaque fois
	
	Vec directionBaguette_ ;

	//A lire avec le .scn
	float rayonBaguette_;
	float heightBaguette_;

	//float pour la translation générale de la baguette
	float TX,TY,TZ;

	GLUquadric* baguette_;
	
public:

	/* constructors destructors */
	Baguette();
	~Baguette();

	/* draw */
	virtual void draw() ;
    	virtual float boundingRadius() const { return radius(); }

	// faire un scale général de l'élément
	virtual void scaleBaguette(float s);
	virtual void translateBaguette(float x, float y, float z);

	/* getters */
	Vec getCenter();
	Vec getPositionBoutBaguette();
	Vec getDirectionBaguette();
	float getRayonBaguette();
	float getHeightBaguette();
	

	float radius() const { return 1000.0; }

	/*setters */
	void setCenter(Vec r);
	void setPositionBoutBaguette(Vec r);
	void setDirectionBaguette(Vec r);
	void setRayonBaguette(float r);
	void setHeightBaguette(float r);

	virtual void initFromDOMElement(const QDomElement& e);

	const qglviewer::Frame& frame() const { return frame_; }
	const Material& material() const { return material_; }

protected :
	// Tous les objets ont un repere et un materiau.
  	qglviewer::Frame frame_;
  	Material material_;
};


#endif // BAGUETTE__H
