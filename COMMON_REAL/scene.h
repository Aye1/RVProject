#ifndef SCENE_H
#define SCENE_H

//#include "object.h"
#include "camera.h"
//#include "elementBat.h"

class ElementBat;
class Baguette ;

class Scene
{
public :
  void draw() const;
  void loadFromFile(const QString& filename);

  qglviewer::Vec center() const;
  float radius(const qglviewer::Vec& center) const;

  void setCameraDroite(const Camera& camera) { cameraDroite_ = camera; }
  void setCameraGauche(const Camera& camera) { cameraGauche_ = camera; }
  const Camera& cameraDroite() const { return cameraDroite_; }
  const Camera& cameraGauche() const { return cameraGauche_; }
  Camera& cameraDroite() { return cameraDroite_; }
  Camera& cameraGauche() { return cameraGauche_; }

  QList<ElementBat *> getListeBatterie(); 
  QList<Baguette *> getListeBaguette();  

  void addElement(ElementBat* e);
  void addBaguette(Baguette* e);
private:
  Camera cameraDroite_;
  Camera cameraGauche_;
  QList<ElementBat *> liste_batterie_;
  QList<Baguette *> liste_baguette_; //en theorie que 2 baguettes suffisent
  // A FAIRE
};

#endif // SCENE_H
