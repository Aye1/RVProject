#include "scene.h"
#include "elementBat.h"
#include "baguette.h"
#include "touche.h"
#include <qfile.h>
#include <QList>
#include <iostream>
#include <qdom.h>
#include "myfob.h"
#include "time.h"

using namespace std;

using namespace qglviewer;

void Scene::draw() 
{
	updateCamera();
	updateWiimote();
	updateTime();
	//retourne l'indice du tambour validé
	int validDrum1;
	int validDrum2;
	validate(validDrum1,validDrum2);
	//env_->SkyBox_Draw(-50, -50, -50, 100, 100, 100);	
	//parcours de la liste d'object
	int i=1;
	foreach(ElementBat* ele,liste_batterie_){
		bool valid=false;		
		if(i==validDrum1 || i==validDrum2){
		   valid=true;
		}		
		ele->draw(valid);
		i++;
	}
  	foreach(Baguette* bag,liste_baguette_){
    		bag->draw();
  	}
  	/*foreach(Touche touch, _listeTouches){
    		touch->draw();
  	}*/

}

void Scene::initTouches()
{
/*	for(unsigned int i=0;i<500;++i){
		Touche * t = new Touche();
		t->setInclinaison(i);
		t->setPosition(Vec(i,0.0,0.0));
		addTouches(t);
	}
*/
}

void Scene::initSkybox()
{
	env_ = new Skybox();
	env_->SkyBox_CreateTexture();
}

void Scene::loadFromFile(const QString& filename)
{
  
 QDomDocument doc("mydocument");
 QFile file(filename);
 if (!file.open(QIODevice::ReadOnly))
     return;
 if (!doc.setContent(&file)) {
     file.close();
     return;
 }
 file.close();

 // print out the element names of all elements that are direct children
 // of the outermost element.
 QDomElement docElem = doc.documentElement();

 QDomNode n = docElem.firstChild();
 while(!n.isNull()) {
     QDomElement e = n.toElement(); // try to convert the node to an element.
     if(!e.isNull()) {
         cout << qPrintable(e.tagName()) << endl; // the node really is an element.
	if (QString::compare(QString("Batterie"),e.tagName())==0) 
	{
		//creation de la baterie			
		ElementBat* S = new ElementBat();
		//initialisation avec QdomElement
		S->initFromDOMElement(e);
		//mise a jour de la position du centre
		S->setCenter(S->frame().position());
		//ajout a la liste d'objets
		this->addElement(S);			
	}
	else if (QString::compare(QString("Baguette"),e.tagName())==0) 
	{
		//creation de la baterie			
		Baguette* S = new Baguette();
		//initialisation avec QdomElement
		S->initFromDOMElement(e);
		//mise a jour de la position du centre
		S->setCenter(S->frame().position());
		S->setDirectionBaguette(S->frame().orientation().axis());
		//ajout a la liste d'objets
		this->addBaguette(S);			
	}
	else if (QString::compare(QString("Camera"),e.tagName())==0)
	{
		//initialisation avec QdomElement
		camera_.initFromDOMElement(e);	
	}
     }
     n = n.nextSibling();
 }

 // Here we append a new element to the end of the document
 QDomElement elem = doc.createElement("img");
 elem.setAttribute("src", "myimage.png");
 docElem.appendChild(elem);

  // Pour ne pas avoir de warnings "unused parameter" - A supprimer
}

Vec Scene::center() const
{
  Vec res;
  res = Vec(0.0,0.0,0.0);
  int n = 0;

  foreach(ElementBat* ele,liste_batterie_){
    res += ele->frame().position();
    n++;
  }
  
  res /= n;

  return res;
}

float Scene::radius(const Vec& center) const
{
  float res = 0.0f;
  
  foreach(ElementBat* ele,liste_batterie_){
    float dist_center = (center - ele->frame().position()).norm();
    float R;
    R = dist_center + ele->boundingRadius();

    if (R>res)
      res=R;
  }

  
  return res;
}


QList<ElementBat *> Scene::getListeBatterie()
{
 return liste_batterie_;
}

QList<Baguette *> Scene::getListeBaguette()
{
 return liste_baguette_;
}


void Scene::addElement(ElementBat* e)
{
	liste_batterie_.push_back(e);
}
void Scene::addBaguette(Baguette* e)
{
	liste_baguette_.push_back(e);
}
void Scene::addTouches(Touche* c)
{
}

void Scene::updateWiimote()
{
   float pos1x;
   float pos1y;
   float pos2x;
   float pos2y;
   double acc1x;
   double acc2x;
   double acc1y;
   double acc2y;
   double acc1z;
   double acc2z;
//On récupère les positions des wiimotes
   	_wii->update();
	_wii->getPos(pos1x,pos2x,pos1y,pos2y,acc1x,acc2x,acc1y,acc2y,acc1z,acc2z);
//La profondeur reste constante
   qglviewer::Vec pg=liste_baguette_[0]->getPositionBoutBaguette();
   qglviewer::Vec pd=liste_baguette_[1]->getPositionBoutBaguette();
//mise à jourqglviewer::Vec
   pg.x= -10.0;
   pg.y=(pos1x - 500)*3.0/25.0;
   pg.z=-(pos1y-600)*1.0/10.0; 
   pd.x= -10.0;
   pd.y=(pos2x - 500)*3.0/25.0; 
   pd.z=-(pos2y-600)*1.0/10.0; 

	
	std::cout << pg.x << " " << pg.z << " " << pd.x << " " << pd.z << std::endl;
//Baguette gauche
   liste_baguette_[0]->setCenter(pg);
//Baguette droite
   liste_baguette_[1]->setCenter(pd);

}


void Scene::setFile(AGHFile * file) 
{
	_file = file;
}

void Scene::validate(int& drum1,int& drum2)
{	
	drum1=0;
	drum2=0;
	_wii->update();
	_wii->getVal();
	if(_wii->getValid1()){
		drum1=_wii->getWiiZone1();
	}
	if(_wii->getValid1()){
		drum2=_wii->getWiiZone2();
	}	
}

void Scene::updateCamera(){
	qglviewer::Vec pos;
	qglviewer::Quaternion q;
	_fob->getPosAndOri(pos,q);
	camera_.setPosition(pos);
	camera_.setOrientation(q);
}

void Scene::updateTime() {
	float dt = (float)clock()/(float)CLOCKS_PER_SEC;
	_timeSinceLastNote += dt;
	if (_timeSinceLastNote >= _timeBetweenNotes) {
		_timeSinceLastNote = 0.0f;
	}
}

void Scene::setTimeBetweenNotes(float time) {
	_timeBetweenNotes = time;
}
