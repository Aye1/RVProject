#ifndef SCENE_H
#define SCENE_H

//#include "object.h"
#include "camera.h"
//#include "elementBat.h"

#include "Skybox.h"
#include "wiim.h"
#include "AGHFile.h"

class ElementBat;
class Baguette ;
class Touche;
//class Wiim;
class myFob;

class Scene
{
public :
  void draw();
  void loadFromFile(const QString& filename);

  qglviewer::Vec center() const;
  float radius(const qglviewer::Vec& center) const;

  void setCamera(const Camera& camera) { camera_ = camera; }
  const Camera& camera() const { return camera_; } 
  Camera& camera() { return camera_; }
  QList<ElementBat *> getListeBatterie(); 
  QList<Baguette *> getListeBaguette();  

  void addElement(ElementBat* e);
  void addBaguette(Baguette* e);
  void addTouches(Touche* c);

  void initTouches();
  void initSkybox();
  void setWii(Wiim* w){_wii=w;};
  void updateWiimote();

  void setFile(AGHFile * file);
private:
  Camera camera_;
  Skybox * env_;
  Wiim* _wii;
  AGHFile * _file;

  QList<Touche *> touches_;

  QList<ElementBat *> liste_batterie_;
  QList<Baguette *> liste_baguette_; //en theorie que 2 baguettes suffisent
  // A FAIRE
};

#endif // SCENE_H
