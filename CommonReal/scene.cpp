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
	//updateWiimote();
	if (_shouldPlay) {
		updateTime();
	}
	_listeTouches->updateNotesPos(liste_batterie_);
	//retourne l'indice du tambour validé
	int validDrum1=3;
	int validDrum2;
	//validate(validDrum1,validDrum2);
	bool drum1;
	bool drum2;
	int i1;
	int i2;
	Color c1;
	Color c2;
	isValid(validDrum1,validDrum2,drum1,drum2,i1,i2,c1,c2);
	//env_->SkyBox_Draw(-50, -50, -50, 100, 100, 100);	
	//parcours de la liste d'object
	int i=1;
	foreach(ElementBat* ele,liste_batterie_){
		
		if(i==validDrum1 && drum1){
		 //  std::cout<<"tralala"<<std::endl;
		   ele->draw(true,c1);	   
		   playSoundFromIndex(i);
		}else if(i==validDrum2 && drum2){
		   ele->draw(true,c2);	   
		  
		}else{		
		   ele->draw(false,ele->material().diffuseColor());
		}
		i++;
	}
  	foreach(Baguette* bag,liste_baguette_){
    		bag->draw();
  	}
	if(validDrum1!=0 && drum1){
		//_listeTouches->removeAt(i1);
	}
	if(validDrum2!=0 && drum2){
		//_listeTouches->removeAt(i2);
	}
	Touche * touch;
  	foreach(touch, *_listeTouches){
    		touch->draw();
  	}

}

void Scene::initTouches()
{
	_listeTouches = new AGHListeTouches(liste_batterie_); 
	_oldTime = 0.0f;
}

void Scene::initSkybox()
{
	env_ = new Skybox();
	env_->SkyBox_CreateTexture();
}

void Scene::initSounds() {
	_vert = new QSound("cymbale.wav");
	_rouge = new QSound("claire.wav");
	_jaune = new QSound("charleston.wav");
	_bleu = new QSound("tome.wav");
}

void Scene::playSoundFromIndex(int index) {
	QSound * soundToPlay;
	if (index == 1) {
		soundToPlay = _vert;
	} else if (index == 2) {
		soundToPlay = _rouge;
	} else if (index == 3) {
		soundToPlay = _jaune;
	} else if (index == 4) {
		soundToPlay = _bleu;
	}
	soundToPlay->play();
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
	float newTime = (float)clock()/(float)CLOCKS_PER_SEC;
	float dt = newTime - _oldTime; 
	if(_file != NULL) {
		_timeSinceLastNote += dt;
		if (_timeSinceLastNote >= _timeBetweenNotes) {
			_timeSinceLastNote = 0.0f;
			int newNotes = _file->nextNote();
			if (newNotes == -1) {
				_file->seek(0);
				_shouldPlay = false;
			} else if (newNotes != 0) {
				_listeTouches->addNotes(newNotes,liste_batterie_);
			}
		}	
	}
	_oldTime = newTime;
}

void Scene::setTimeBetweenNotes(float time) {
	_timeBetweenNotes = time;
}
void Scene::isValid(int nbdrum1, int nbdrum2,bool& drum1,bool& drum2,int& i1, int& i2,Color& c1,Color& c2){
	drum1=false;
	drum2=false;
	bool d1=false;
	bool d2=false;
	qglviewer::Vec center1;
	qglviewer::Vec center2;
	Color color1;
	Color color2;
	if(nbdrum1!=0){
		center1=liste_batterie_[nbdrum1-1]->getPositionCenterBat();
		color1=liste_batterie_[nbdrum1-1]->material().diffuseColor();
		d1=true;
	}
	if(nbdrum2!=0){
		center2=liste_batterie_[nbdrum2-1]->getPositionCenterBat();
		color2=liste_batterie_[nbdrum2-1]->material().diffuseColor();
		d2=true;
	}
	
	if(d1 | d2){
	//Parcours de la liste des touches
	float seuilPerfect=0.5;
	float seuilMoyen=3.0;
	float seuilBof=6.0;
	Touche* touch;
	//std::cout<<"nb de touches: "<<_listeTouches->size()<<std::endl;
	int i=0;
	foreach(touch, *_listeTouches){
		qglviewer::Vec pos=touch->getPosition();
		if(d1){
		//std::cout<<"color1: "<<color1.r<<" "<<color1.g<<" "<<color1.b<<std::endl;
		if(color1.r==touch->getColor().r && color1.g==touch->getColor().g && color1.b==touch->getColor().b ){
			//std::cout<<"jaune"<<std::endl;
			float dist1=sqrt(pow(pos.x-center1.x,2)+pow(pos.y-center1.y,2)+pow(pos.z-center1.z,2));
		
			if(dist1<=seuilPerfect){
				c1.r=0.4; c1.g=0.2; c1.b=0.4;
				i1=i;
	   			drum1=true;
			}
			if(dist1<=seuilMoyen){
				c1.r=1.0; c1.g=0.6; c1.b=1.0;
				i1=i;
	   			drum1=true;
			}
			if(dist1<=seuilBof){
				c1.r=1.0; c1.g=1.0; c1.b=1.0;
				i1=i;
	   			drum1=true;
			}
		}
		}
		if(d2){
		if(color2.r==touch->getColor().r && color2.g==touch->getColor().g && color2.b==touch->getColor().b){
			float dist2=sqrt(pow(pos.x-center2.x,2)+pow(pos.y-center2.y,2)+pow(pos.z-center2.z,2));
    			if(dist2<=seuilPerfect){
				c2.r=0.4; c2.g=0.2; c2.b=0.4;
				i2=i;
	   			drum2=true;
			}
			if(dist2<=seuilMoyen){
				c2.r=1.0; c2.g=0.6; c2.b=1.0;
				i2=i;
	   			drum2=true;
			}
			if(dist2<=seuilBof){
				c2.r=1.0; c2.g=1.0; c2.b=1.0;
				i2=i;
	   			drum2=true;
			}
		}
		}
	i++;	
  	}
}	
	
}
