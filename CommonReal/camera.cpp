
#include "camera.h"
#include "scene.h"
#include <qmessagebox.h>
#include <iostream>

using namespace std;

Camera::Camera()
{
  setXResolution(64);
  setYResolution(64);
  setFieldOfView(M_PI / 4.0);
}

void Camera::initFromDOMElement(const QDomElement& e)
{
  if (e.tagName() != "Camera")
    {
      QMessageBox::critical(NULL, "Camera init error", "Camera::initFromDOMElement, bad DOM tagName.\nExpecting 'Camera', got "+e.tagName());
      return;
    }

  if (!e.hasAttribute("fieldOfView"))
    QMessageBox::warning(NULL, "Camera error", "Camera has undefined fieldOfView. Using pi/4.");
  setFieldOfView(e.attribute("fieldOfView", "0.7854").toFloat());

  if (!e.hasAttribute("xResolution"))
    QMessageBox::warning(NULL, "Camera error", "Camera has undefined xResolution. Using 64.");
  setXResolution(e.attribute("xResolution", "64").toInt());

  if (!e.hasAttribute("yResolution"))
    QMessageBox::warning(NULL, "Camera error", "Camera has undefined yResolution. Using 64.");
  setYResolution(e.attribute("yResolution", "64").toInt());

  QDomNode n = e.firstChild();
  while (!n.isNull())
    {
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "Frame")
	    frame_.initFromDOMElement(e);
	  else
	    QMessageBox::warning(NULL, "Camera child error", "Unsupported Camera child : "+e.tagName());
	}
      else
	QMessageBox::warning(NULL, "Camera XML error", "Error while parsing Camera XML document");
      n = n.nextSibling();
    }
}

void Camera::setXResolution(int xResolution)
{
  if (xResolution <= 0)
    {
      QMessageBox::warning(NULL, "Camera error", "Negative image xResolution - Skipping");
      return;
    }
  xResolution_ = xResolution;
}

void Camera::setYResolution(int yResolution)
{
  if (yResolution <= 0)
    {
      QMessageBox::warning(NULL, "Camera error", "Negative image yResolution - Skipping");
      return;
    }
  yResolution_ = yResolution;
}

void Camera::draw(float radius) const
{

	float plan_near = 2.0f;	
	qglviewer::Vec a1,a2,a3,a4;

	qglviewer::Vec up    = frame_.inverseTransformOf(qglviewer::Vec(0.0, 1.0, 0.0)); 
	qglviewer::Vec view  = frame_.inverseTransformOf(qglviewer::Vec(0.0, 0.0, -1.0));
	qglviewer::Vec right = frame_.inverseTransformOf(qglviewer::Vec(1.0, 0.0, 0.0));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixd(frame().matrix());
 	//glScalef(0.1f,0.1f,0.1f);
	glColor3f(0.0f,1.0f,1.0f);
	glBegin(GL_LINES);
		//les 3 axes de la camera
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0,0.0,0.0); 
		glVertex3f(up.x,up.y,up.z);
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0,0.0,0.0); 
		glVertex3f(view.x,view.y,view.z);
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0,0.0,0.0); 
		glVertex3f(right.x,right.y,right.z);

		glColor3f(0.0f,1.0f,1.0f);
		//le frustum de la camera
		//origine->coin haut gauche
		glVertex3f(0.0,0.0,0.0);  
		a1 = plan_near*view + tan(fieldOfView_/2.0)*plan_near*up - tan(fieldOfView_*yResolution()/(2.0*xResolution()))*plan_near*right;
		glVertex3f(a1.x,a1.y,a1.z);
		//origine->coin haut droit
		glVertex3f(0.0,0.0,0.0);  
		a2 = plan_near*view + tan(fieldOfView_/2.0)*plan_near*up + tan(fieldOfView_*yResolution()/(2.0*xResolution()))*plan_near*right;	
		glVertex3f(a2.x,a2.y,a2.z);
		//origine->coin bas gauche
		glVertex3f(0.0,0.0,0.0);  
		a3 = plan_near*view - tan(fieldOfView_/2.0)*plan_near*up - tan(fieldOfView_*yResolution()/(2.0*xResolution()))*plan_near*right;	
		glVertex3f(a3.x,a3.y,a3.z);
		//origine->coin bas droit
		glVertex3f(0.0,0.0,0.0); 
		a4 = plan_near*view - tan(fieldOfView_/2.0)*plan_near*up + tan(fieldOfView_*yResolution()/(2.0*xResolution()))*plan_near*right;	
		glVertex3f(a4.x,a4.y,a4.z);
			
	    // le contour du cadre de visé
		glVertex3f(a1.x,a1.y,a1.z);
		glVertex3f(a2.x,a2.y,a2.z);
		glVertex3f(a2.x,a2.y,a2.z);		
		glVertex3f(a4.x,a4.y,a4.z);
		glVertex3f(a4.x,a4.y,a4.z);
		glVertex3f(a3.x,a3.y,a3.z);
		glVertex3f(a3.x,a3.y,a3.z);
		glVertex3f(a1.x,a1.y,a1.z);

	glEnd();		

	//on va dessinner le quadrillage

	//on stocke dans des tableaux les indices des points sur les bords du cadre.
	qglviewer::Vec* lignehaut    = new qglviewer::Vec[xResolution()];
	qglviewer::Vec* lignebas     = new qglviewer::Vec[xResolution()];
	qglviewer::Vec* lignegauche  = new qglviewer::Vec[yResolution()];
	qglviewer::Vec* lignedroite  = new qglviewer::Vec[yResolution()];


	//20=xres ou yRes
	for (int i=0 ; i<xResolution() ; i++ ) {
		lignehaut[i] = a1 + i*(a2-a1)/xResolution() ;
		lignebas[i]  = a3 + i*(a4-a3)/xResolution() ;
	}
	for (int j=0 ; j<yResolution() ; j++ ) {
		lignegauche[j] = a1 + j*(a3-a1)/yResolution() ;
		lignedroite[j] = a2 + j*(a4-a2)/yResolution() ;
	}

	
	for (int i=0 ; i<xResolution() ; i++ ) {
		glBegin(GL_LINES);
		glVertex3f(lignehaut[i].x,lignehaut[i].y,lignehaut[i].z);
		glVertex3f(lignebas[i].x,lignebas[i].y,lignebas[i].z);
		glEnd();
	}
	for (int i=0 ; i<yResolution()  ; i++ ) {
		glBegin(GL_LINES);
		glVertex3f(lignegauche[i].x,lignegauche[i].y,lignegauche[i].z);
		glVertex3f(lignedroite[i].x,lignedroite[i].y,lignedroite[i].z);
		glEnd();
	}

	glBegin(GL_QUADS);
		glColor4f(0.0f,1.0f,1.0f,0.05f);
		glVertex3f(a1.x,a1.y,a1.z);
		glVertex3f(a2.x,a2.y,a2.z);
		glVertex3f(a4.x,a4.y,a4.z);
		glVertex3f(a3.x,a3.y,a3.z);
	glEnd();


	glPopMatrix();
}


void Camera::drawAllRays() const
{

}
