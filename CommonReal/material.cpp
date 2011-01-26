#include "material.h"
#include <qmessagebox.h>

Material::Material()
{
  setSpecularCoefficient(0.0);
  setTextureScale(1.0);
}

void Material::initFromDOMElement(const QDomElement& e)
{
  if (e.tagName() != "Material")
    {
      QMessageBox::critical(NULL, "Material init error", "Material::initFromDOMElement, bad DOM tagName.\nExpecting 'Material', got "+e.tagName());
      return;
    }

  QDomNode n = e.firstChild();
  while (!n.isNull())
    {
      QDomElement e = n.toElement();
      if (!e.isNull())
	{
	  if (e.tagName() == "DiffuseColor")
	    diffuseColor_.initFromDOMElement(e);
	  else
	    if (e.tagName() == "SpecularColor")
	      {
		specularColor_.initFromDOMElement(e);
		if (!e.hasAttribute("exponent"))
		  QMessageBox::warning(NULL, "Material error", "No Material specular coefficient provided. Using 0.0 instead");
		setSpecularCoefficient(e.attribute("exponent", "0.0").toFloat());
	      }
	    else
	      if (e.tagName() == "ReflectiveColor")
		reflectiveColor_.initFromDOMElement(e);
	      else
		if (e.tagName() == "Texture")
		  {
		    setTextureScale(e.attribute("scale", "1.0").toFloat());
		    if (e.hasAttribute("scaleU"))
		      setTextureScaleU(e.attribute("scaleU", "1.0").toFloat());
		    if (e.hasAttribute("scaleV"))
		      setTextureScaleV(e.attribute("scaleV", "1.0").toFloat());
		    loadTextureFromFile(e.attribute("file"));
		    if (e.attribute("mode") == "MODULATE") setTextureMode(MODULATE);
		    if (e.attribute("mode") == "BLEND")    setTextureMode(BLEND);
		    if (e.attribute("mode") == "REPLACE")  setTextureMode(REPLACE);
		  }
		else
		  QMessageBox::warning(NULL, "Material child error", "Unsupported Material child : "+e.tagName());
	}
      else
	QMessageBox::warning(NULL, "Material XML error", "Error while parsing Material XML document");

      n = n.nextSibling();
    }
}

void Material::loadTextureFromFile(const QString& fileName)
{
  if (fileName.isNull())
    QMessageBox::warning(NULL, "Material texture error", "Material error : no texture file name provided");
  else
    if (!texture_.load(fileName))
      QMessageBox::warning(NULL, "Material texture error", "Unable to load Material texture from "+fileName);
}

Color Material::diffuseColor(float u, float v) const
{
  if (texture().isNull())
    return diffuseColor();

  // Take texture scaling into account
  u /= textureScaleU();
  v /= textureScaleV();

  // Compute pixel coordinates
  const int i = int(fabs(u - floor(u)) * texture().width());
  const int j = int(fabs(v - floor(v)) * texture().height());

  // Compute color
  switch(textureMode())
    {
    case MODULATE : return Color(texture().pixel(i,j)) * diffuseColor();
    case BLEND    :
      {
	const float alpha = qAlpha(texture().pixel(i,j)) / 255.0;
	return alpha*Color(texture().pixel(i,j)) + (1.0f-alpha)*diffuseColor();
      }
    case REPLACE  : return Color(texture().pixel(i,j));
    }
  return Color();
}
