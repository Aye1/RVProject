#ifndef LIGHT_H
#define LIGHT_H

#include "color.h"
#include "hit.h"

class Scene;

class Light
{
public :
  Color color() const { return color_; }
  void setColor(const Color& color) { color_ = color; }

  virtual void initFromDOMElement(const QDomElement& e);
  
private:
  Color color_;
};


class AmbientLight : public Light
{
public :
  virtual Color illuminatedColor(const qglviewer::Vec& eye, const Hit& hit) const;
};


class DirectionalLight : public Light
{
public :
  DirectionalLight();

  qglviewer::Vec direction() const { return direction_; }
  void setDirection(const qglviewer::Vec& direction) { direction_ = direction; direction_.normalize(); }

  virtual void initFromDOMElement(const QDomElement& e);

private:
  qglviewer::Vec direction_;
};


class PointLight : public Light
{
public :
  qglviewer::Vec position() const { return position_; }
  void setPosition(const qglviewer::Vec& position) { position_ = position; }

  virtual void initFromDOMElement(const QDomElement& e);

private:
  qglviewer::Vec position_;
};

#endif // LIGHT_H
