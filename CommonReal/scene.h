#ifndef SCENE_H
#define SCENE_H

//#include "object.h"
//#include "camera.h"
//#include "elementBat.h"
#include  <QGLViewer/qglviewer.h>

#include "Skybox.h"
#include "wiim.h"
#include "AGHFile.h"
#include "AGHListeTouches.h"
#include <QSound>

class ElementBat;
class Baguette ;
class Touche;
//class Wiim;
class myFob;

using namespace qglviewer;

class Scene
{
public :
  bool _shouldPlay;
  float _oldTime;
  void draw();
  void loadFromFile(const QString& filename);

  qglviewer::Vec center() const;
  float radius(const qglviewer::Vec& center) const;

  void setCamera(Camera& camera) { camera_ = camera; }
  const Camera& camera() const { return camera_; } 
  Camera& camera() { return camera_; }
  QList<ElementBat *> getListeBatterie(); 
  QList<Baguette *> getListeBaguette();  

  void addElement(ElementBat* e);
  void addBaguette(Baguette* e);

  void initTouches();
  void initSkybox();
  void initSounds();
  void playSoundFromIndex(int index);
  void setWii(Wiim* w){_wii=w;};
  void updateWiimote();
  void setFob(myFob* f){_fob=f;};
  void updateCamera();

  void setFile(AGHFile * file);
  void setTimeBetweenNotes(float time);
  //retourne l'indice du tambour validé
  void validate(int& drum1,int& drum2);
  void isValid(int nbdrum1, int nbdrum2,bool& drum1,bool& drum2,int& i1,int& i2,Color& c1,Color& c2);
private:
  Camera camera_;
  Skybox * env_;
  Wiim* _wii;
  myFob* _fob;
  AGHFile * _file;

  
  AGHListeTouches * _listeTouches;
  float _timeSinceLastNote;
  float _timeBetweenNotes;
  QList<ElementBat *> liste_batterie_;
  QList<Baguette *> liste_baguette_; //en theorie que 2 baguettes suffisent - en pratique aussi
  QSound * _vert;
  QSound * _rouge;
  QSound * _jaune;
  QSound * _bleu;


  void updateTime();
};

#endif // SCENE_H
