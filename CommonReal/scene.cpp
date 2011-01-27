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
#include <math.h>

using namespace std;

using namespace qglviewer;

void Scene::draw() 
{
	//updateCamera();
	updateWiimote();
	//updateTime();
	_listeTouches->updateNotesPos();
	//retourne l'indice du tambour validé
	int validDrum1;
	int validDrum2;

	//validate(validDrum1,validDrum2);
	bool drum1;
	bool drum2;
	isValid(validDrum1,validDrum2,drum1,drum2);
	//env_->SkyBox_Draw(-50, -50, -50, 100, 100, 100);	
	//parcours de la liste d'object
	int i=1;
	foreach(ElementBat* ele,liste_batterie_){
		bool valid=false;		
		if((i==validDrum1 && drum1) || (i==validDrum2 && drum2)){
		   valid=true;
		}		
		ele->draw(valid);
		i++;
	}
  	foreach(Baguette* bag,liste_baguette_){
    		bag->draw();
  	}
	Touche * touch;
  	foreach(touch, *_listeTouches){
    		touch->draw();
  	}

}

void Scene::initTouches()
{
_listeTouches = new AGHListeTouches(liste_batterie_); 
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
   pg.y= -15.0;
   pg.x=-(pos1x - 500)*3.0/25.0;
   pg.z=-(pos1y-800)*60.0/800.0 + 25.0; 
   pd.y= -15.0;
   pd.x=-(pos2x - 500)*3.0/25.0; 
   pd.z=-(pos2y-800)*60.0/800.0 + 25.0; 
	
	
  if(acc1y < _wii->getSeuilPos()) {
	liste_baguette_[0]->setCenter(pg);	
	liste_baguette_[1]->setCenter(pd);		
	liste_baguette_[0]->setDirectionBaguette(Vec(0.0,0.0,1.0));	
	liste_baguette_[1]->setDirectionBaguette(Vec(0.0,0.0,1.0));
  } else if (acc1y > _wii->getSeuilVal()) {
	liste_baguette_[0]->setCenter(pg);	
	liste_baguette_[1]->setCenter(pd);
	Vec dirbagG;
	Vec dirbagD;

	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(100.0,0.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,100.0,0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,0.0,100.0);
	glEnd();

	double angleAlphaMax = 0 ;
	double angleGamma = 0;
	float h = 0;

	if(_wii->getWiiZone1()==1){
		dirbagG = (liste_batterie_[0]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0).unit()*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[0]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm() -liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==2) {
		dirbagG = (liste_batterie_[1]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[1]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==3) {
		dirbagG = (liste_batterie_[2]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[2]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==4) {		
		dirbagG = (liste_batterie_[3]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[3]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	}
	if(_wii->getWiiZone1()!=0){
		if(liste_batterie_[_wii->getWiiZone1()-1]->getPositionCenterBat().x < pg.x){
			pg.x -= h*sin(abs(angleAlphaMax))*sin(angleGamma);
			pg.y += h*sin(abs(angleAlphaMax))*cos(angleGamma);
			pg.z = 20 + h*cos(abs(angleAlphaMax));
		} else {
			pg.x += h*sin(abs(angleAlphaMax))*sin(angleGamma);
			pg.y += h*sin(abs(angleAlphaMax))*cos(angleGamma);
			pg.z = 20 + h*cos(abs(angleAlphaMax));
		}
	}
		
	if(_wii->getWiiZone2()==1){
		dirbagD = (liste_batterie_[0]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0).unit()*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[0]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm() -liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==2) {
		dirbagD = (liste_batterie_[1]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[1]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==3) {
		dirbagD = (liste_batterie_[2]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[2]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==4) {		
		dirbagD = (liste_batterie_[3]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[3]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	}
	if(_wii->getWiiZone2()!=0){
		if(liste_batterie_[_wii->getWiiZone2()-1]->getPositionCenterBat().x < pd.x){
			pd.x -= h*sin(abs(angleAlphaMax))*sin(angleGamma);
			pd.y += h*sin(abs(angleAlphaMax))*cos(angleGamma);
			pd.z = 20 + h*cos(abs(angleAlphaMax));
		} else {
			pd.x += h*sin(abs(angleAlphaMax))*sin(angleGamma);
			pd.y += h*sin(abs(angleAlphaMax))*cos(angleGamma);
			pd.z = 20 + h*cos(abs(angleAlphaMax));
		}
	}


	liste_baguette_[0]->setDirectionBaguette(dirbagG);	
	liste_baguette_[1]->setDirectionBaguette(dirbagD);
	liste_baguette_[0]->setCenter(pg);	
	liste_baguette_[1]->setCenter(pd);
	
  } else {
	liste_baguette_[0]->setCenter(pg);	
	liste_baguette_[1]->setCenter(pd);
	Vec dirbagG;
	Vec dirbagD;


	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(100.0,0.0,0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,100.0,0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,0.0,100.0);
	glEnd();

	double angleAlpha = 0 ;
	double angleAlphaMax = 0 ;
	double angleGamma = 0;
	float h = 0;

	if(_wii->getWiiZone1()==1){
		dirbagG = (liste_batterie_[0]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc1y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0).unit()*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[0]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm() -liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==2) {
		dirbagG = (liste_batterie_[1]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc1y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[1]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==3) {
		dirbagG = (liste_batterie_[2]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc1y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[2]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone1()==4) {		
		dirbagG = (liste_batterie_[3]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagG.x,dirbagG.y,dirbagG.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc1y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagG.x,dirbagG.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[3]->getPositionCenterBat() - Vec(pg.x,pg.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	}
	if(_wii->getWiiZone1()!=0){
		if(liste_batterie_[_wii->getWiiZone1()-1]->getPositionCenterBat().x < pg.x){
			pg.x -= h*sin(abs(angleAlpha))*sin(angleGamma);
			pg.y += h*sin(abs(angleAlpha))*cos(angleGamma);
			pg.z = 20 + h*cos(abs(angleAlpha));
		} else {
			pg.x += h*sin(abs(angleAlpha))*sin(angleGamma);
			pg.y += h*sin(abs(angleAlpha))*cos(angleGamma);
			pg.z = 20 + h*cos(abs(angleAlpha));
		}
	}
		
	if(_wii->getWiiZone2()==1){
		dirbagD = (liste_batterie_[0]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc2y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0).unit()*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[0]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm() -liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==2) {
		dirbagD = (liste_batterie_[1]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc2y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[1]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==3) {
		dirbagD = (liste_batterie_[2]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc2y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[2]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	} else if(_wii->getWiiZone2()==4) {		
		dirbagD = (liste_batterie_[3]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).unit();
		angleAlphaMax = acos((double) (Vec(dirbagD.x,dirbagD.y,dirbagD.z)*Vec(0.0,0.0,1.0)));
		angleAlpha = (acc2y-_wii->getSeuilPos())*angleAlphaMax/(_wii->getSeuilVal()-_wii->getSeuilPos());
		angleGamma = acos((double) (Vec(dirbagD.x,dirbagD.y,0.0)*Vec(0.0,1.0,0.0)));
		h = (liste_batterie_[3]->getPositionCenterBat() - Vec(pd.x,pd.y,20.0)).norm()-liste_baguette_[0]->getHeightBaguette()/2.0;
	}
	if(_wii->getWiiZone2()!=0){
		if(liste_batterie_[_wii->getWiiZone2()-1]->getPositionCenterBat().x < pd.x){
			pd.x -= h*sin(abs(angleAlpha))*sin(angleGamma);
			pd.y += h*sin(abs(angleAlpha))*cos(angleGamma);
			pd.z = 20 + h*cos(abs(angleAlpha));
		} else {
			pd.x += h*sin(abs(angleAlpha))*sin(angleGamma);
			pd.y += h*sin(abs(angleAlpha))*cos(angleGamma);
			pd.z = 20 + h*cos(abs(angleAlpha));
		}
	}


	liste_baguette_[0]->setCenter(pg);	
	liste_baguette_[1]->setCenter(pd);
	liste_baguette_[0]->setDirectionBaguette(Vec(0.0,0.0,1.0));	
	liste_baguette_[1]->setDirectionBaguette(Vec(0.0,0.0,1.0));

  }

	
	//std::cout << pg.x << " " << pg.z << " " << pd.x << " " << pd.z << std::endl;
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
	if(_file != NULL) {
		_timeSinceLastNote += dt;
		if (_timeSinceLastNote >= _timeBetweenNotes) {
			_timeSinceLastNote = 0.0f;
			int newNotes = _file->nextNote();
			if (newNotes != -1) {
				_listeTouches->addNotes(newNotes,liste_batterie_);
			}
		}	
	}
}

void Scene::setTimeBetweenNotes(float time) {
	_timeBetweenNotes = time;
}
void Scene::isValid(int nbdrum1, int nbdrum2,bool& drum1,bool& drum2){
	drum1=false;
	drum2=false;
	qglviewer::Vec center1=liste_batterie_[nbdrum1]->getPositionCenterBat();
	qglviewer::Vec center2=liste_batterie_[nbdrum2]->getPositionCenterBat();
	//Parcours de la liste des touches
	float seuil=3.0;
	Touche* touch;
	foreach(touch, *_listeTouches){
		qglviewer::Vec pos=touch->getPosition();
		float d1=sqrt(pow(pos.x-center1.x,2)+pow(pos.y-center1.y,2)+pow(pos.z-center1.z,2));
		float d2=sqrt(pow(pos.x-center2.x,2)+pow(pos.y-center2.y,2)+pow(pos.z-center2.z,2));

		if(d1<seuil){
	   		drum1=true;
		}
    		if(d2<seuil){
			drum2=true;
		}	
  	}	
	
}
