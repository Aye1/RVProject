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

class ElementBat;
class Baguette ;
class Touche;
//class Wiim;
class myFob;

using namespace qglviewer;

class Scene
{
public :
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
  void addTouches(Touche* c);

  void initTouches();
  void initSkybox();
  void setWii(Wiim* w){_wii=w;};
  void updateWiimote(int cpt1, int cpt2,float _pos1x,float _pos2x,float _pos1y,float _pos2y,double _acc1x,double _acc2x,double _acc1y,double _acc2y,double _acc1z,double _acc2z);
  void setFob(myFob* f){_fob=f;};
  void updateCamera();

  void setFile(AGHFile * file);
  void setTimeBetweenNotes(float time);
  //retourne l'indice du tambour validé
  void validate(int& drum1,int& drum2);
  void isValid(int nbdrum1, int nbdrum2,bool& drum1,bool& drum2,int& i1,int& i2,Color& c1,Color& c2);



int cpt1;
int cpt2;
float _pos1x;
float _pos1y;
float _pos2x;
float _pos2y;
double _acc1x;
double _acc2x;
double _acc1y;
double _acc2y;
double _acc1z;
double _acc2z;

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
  QList<Baguette *> liste_baguette_; //en theorie que 2 baguettes suffisent
  // A FAIRE

  void updateTime();
};

#endif // SCENE_H
