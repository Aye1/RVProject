#include "light.h"
#include "material.h"
#include "scene.h"
#include <qmessagebox.h>

using namespace qglviewer;

void Light::initFromDOMElement(const QDomElement& e)
{
  QDomNode n = e.firstChild();
  while (!n.isNull())
    {
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "Color")
	    color_.initFromDOMElement(e);
	}
      else
	QMessageBox::warning(NULL, "Light XML error", "Error while parsing Light XML document");
      n = n.nextSibling();
    }
}

////////////////////////////////////////////////////////////////////////////////

DirectionalLight::DirectionalLight()
{
  setDirection(Vec(1.0, 0.0, 0.0));
}

void DirectionalLight::initFromDOMElement(const QDomElement& e)
{
  Light::initFromDOMElement(e);
  
  QDomNode n = e.firstChild();
  while (!n.isNull())
    {
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "Direction")
	    {
	      Vec dir;
	      dir.initFromDOMElement(e);
	      setDirection(dir);
	    }
	}
      else
	QMessageBox::warning(NULL, "DirectionalLight XML error", "Error while parsing DirectionalLight XML document");
      n = n.nextSibling();
    }
}

////////////////////////////////////////////////////////////////////////////////

void PointLight::initFromDOMElement(const QDomElement& e)
{
  Light::initFromDOMElement(e);
  
  QDomNode n = e.firstChild();
  while (!n.isNull())
    {
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "Position")
	    position_.initFromDOMElement(e);
	}
      else
	QMessageBox::warning(NULL, "PointLight XML error", "Error while parsing PointLight XML document");
      n = n.nextSibling();
    }
}
