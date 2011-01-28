#include "widgetScene.h" 
#include  <QGLViewer/qglviewer.h>
#include "elementBat.h"
#include "baguette.h"
#include <iostream>
#include "wiim.h"
#include "myfob.h"

using namespace std; 	

WidgetScene::WidgetScene(){
	_scene = new Scene();
	_scene->loadFromFile("troisSpheres.scn");
	_scene->initSkybox();
	_scene->initTouches();
	_scene->initSounds();
	_scene->_shouldPlay = false;

	_viewerG = new ViewerG();
	_viewerD = new ViewerD();

	_wii = new Wiim();
	_fob = new myFob();



	/////////////////////CAMERA
	_cameraCenter = new qglviewer::Camera();
	_cameraCenter->setPosition(_scene->camera().position());
  	_cameraCenter->setOrientation(_scene->camera().orientation());
  	_cameraCenter->setFieldOfView(_scene->camera().fieldOfView());
  
  	// Remove previous keyFrames in path 1 (if any)
  	if (_cameraCenter->keyFrameInterpolator(1))
    	_cameraCenter->keyFrameInterpolator(1)->deletePath();

	  // Add current (i.e. scene camera) position to F1.	
  	_cameraCenter->addKeyFrameToPath(1);
	

	////////////////////INIT VIEWER ET SET CAMERA
  	_viewerG->setSceneCenter(_scene->center());
  	_viewerG->setSceneRadius(_scene->radius(_viewerG->sceneCenter()));
  	_viewerD->setSceneCenter(_scene->center());
  	_viewerD->setSceneRadius(_scene->radius(_viewerD->sceneCenter()));

	_viewerG->setScene(_scene);
	_viewerD->setScene(_scene);


	//RECUPERATION DE LA DIRECTION DE VISEE DE LA CAMERA:
	qglviewer::Vec cameraDirRight;
	cameraDirRight = _cameraCenter->rightVector();
	qglviewer::Vec dirRight = cameraDirRight.unit();
	qglviewer::Vec cameraDirUp;
	cameraDirUp = _cameraCenter->upVector();
	qglviewer::Vec dirUp = cameraDirUp.unit();
	//ecartementOeil

	ecartementYeux = 0.0;
 	inclinaisonStrabisme = 5.0;

	_viewerD->setCamera(_cameraCenter);
	_viewerD->setParametersStereo(-dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
	_viewerG->setCamera(_cameraCenter);
	_viewerG->setParametersStereo(dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
	


}


WidgetScene::~WidgetScene(){
	_wii->close();
	//_fob->close();
	delete _wii;
	delete _fob;
	delete _scene ;
	delete _viewerG ;
	delete _viewerD ;
}

Scene * WidgetScene::getScene() {
	return _scene;
}

void WidgetScene::init()
{
	//_fob->init();
//On initialise les wiimotes EN COMMENCANT PAR LA GAUCHE!!
	_wii->init();
//Passage des wiimotes à la scène
	_scene->setWii(_wii);
//Passage du fob à la scène
	//_scene->setFob(_fob);
	isSelected1 = false;
        isSelected2 = false;
  	isSelected3 = false;
  	isSelected4 = false;

	setFocusPolicy(Qt::StrongFocus);

	QHBoxLayout * layout = new QHBoxLayout();
	layout->addWidget(_viewerG);
	layout->addWidget(_viewerD);

	layout->setContentsMargins(0.0,0.0,0.0,0.0);
	layout->setSpacing(0);

	this->setLayout(layout);	

}

void WidgetScene::keyPressEvent(QKeyEvent *e)
{
	qglviewer::Vec cameraDirRight;
	cameraDirRight = _cameraCenter->rightVector();
	qglviewer::Vec dirRight = cameraDirRight.unit();
	qglviewer::Vec cameraDirUp;
	cameraDirUp = _cameraCenter->upVector();
	qglviewer::Vec dirUp = cameraDirUp.unit();

  switch (e->key())
    {
	case Qt::Key_1 :
		isSelected1 = true;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;	
		break;
	case Qt::Key_2 :
		isSelected1 = false;
        isSelected2 = true;
  		isSelected3 = false;
  		isSelected4 = false;
		break;
	case Qt::Key_3 :
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = true;
  		isSelected4 = false;
		break;
	case Qt::Key_4 :
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = true;
		break;
	/*case Qt::Key_5 :
		(scene()->getListeBaguette())[0]->setDirectionBaguette((scene()->getListeBaguette())[0]->getDirectionBaguette()+Vec(0.1,0.1,0.1));
		update();
		break;*/
	
	//deplacement des elements
	case Qt::Key_Up :
		if(isSelected1) {
			((_scene->getListeBatterie())[0])->setInclinaison(((_scene->getListeBatterie())[0])->getInclinaison()-1);
		} else if(isSelected2) {
			((_scene->getListeBatterie())[1])->setInclinaison(((_scene->getListeBatterie())[1])->getInclinaison()-1);
		} else if(isSelected3) {
			((_scene->getListeBatterie())[2])->setInclinaison(((_scene->getListeBatterie())[2])->getInclinaison()-1);
		} else if(isSelected4) {
			((_scene->getListeBatterie())[3])->setInclinaison(((_scene->getListeBatterie())[3])->getInclinaison()-1);
		}
		_viewerD->update();	
		_viewerG->update();	
		break;
	case Qt::Key_Down :
		if(isSelected1) {
			((_scene->getListeBatterie())[0])->setInclinaison(((_scene->getListeBatterie())[0])->getInclinaison()+1);
		} else if(isSelected2) {
			((_scene->getListeBatterie())[1])->setInclinaison(((_scene->getListeBatterie())[1])->getInclinaison()+1);
		} else if(isSelected3) {
			((_scene->getListeBatterie())[2])->setInclinaison(((_scene->getListeBatterie())[2])->getInclinaison()+1);
		} else if(isSelected4) {
			((_scene->getListeBatterie())[3])->setInclinaison(((_scene->getListeBatterie())[3])->getInclinaison()+1);
		}	
		_viewerD->update();	
		_viewerG->update();	
		break;
	case Qt::Key_Left :
		if(isSelected1) {
			((_scene->getListeBatterie())[0])->setAngleRotationBat(((_scene->getListeBatterie())[0])->getAngleRotationBat()-1);
		} else if(isSelected2) {
			((_scene->getListeBatterie())[1])->setAngleRotationBat(((_scene->getListeBatterie())[1])->getAngleRotationBat()-1);
		} else if(isSelected3) {
			((_scene->getListeBatterie())[2])->setAngleRotationBat(((_scene->getListeBatterie())[2])->getAngleRotationBat()-1);
		} else if(isSelected4) {
			((_scene->getListeBatterie())[3])->setAngleRotationBat(((_scene->getListeBatterie())[3])->getAngleRotationBat()-1);
		}
		_viewerD->update();	
		_viewerG->update();	
		break;
	case Qt::Key_Right :
		if(isSelected1) {
			((_scene->getListeBatterie())[0])->setAngleRotationBat(((_scene->getListeBatterie())[0])->getAngleRotationBat()+1);
		} else if(isSelected2) {
			((_scene->getListeBatterie())[1])->setAngleRotationBat(((_scene->getListeBatterie())[1])->getAngleRotationBat()+1);
		} else if(isSelected3) {
			((_scene->getListeBatterie())[2])->setAngleRotationBat(((_scene->getListeBatterie())[2])->getAngleRotationBat()+1);
		} else if(isSelected4) {
			((_scene->getListeBatterie())[3])->setAngleRotationBat(((_scene->getListeBatterie())[3])->getAngleRotationBat()+1);
		}
		_viewerD->update();	
		_viewerG->update();	
		break;

	case Qt::Key_Plus :
		ecartementYeux += 0.05;
		cout << "ecartementYeux " << ecartementYeux << endl;
		_viewerG->setParametersStereo(dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->setParametersStereo(-dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->update();	
		_viewerG->update();	
		break;

	case Qt::Key_Minus :
		ecartementYeux -= 0.05;
		cout << "ecartementYeux " << ecartementYeux << endl;
		_viewerG->setParametersStereo(dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->setParametersStereo(-dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->update();	
		_viewerG->update();	
		break;

	case Qt::Key_Asterisk :
		inclinaisonStrabisme += 0.5;
		cout << "inclinaisonStrabisme " << inclinaisonStrabisme << endl;
		_viewerG->setParametersStereo(dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->setParametersStereo(-dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);	
		_viewerD->update();	
		_viewerG->update();	
		break;

	case Qt::Key_Slash :
		inclinaisonStrabisme -= 0.5;
		cout << "inclinaisonStrabisme " << inclinaisonStrabisme << endl;
		_viewerG->setParametersStereo(dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->setParametersStereo(-dirRight,dirUp,ecartementYeux,inclinaisonStrabisme);
		_viewerD->update();	
		_viewerG->update();	
		break;
	case Qt::Key_P :
		cout << "play" << endl;
		_scene->_shouldPlay = true;
		break;

    default :
	  	isSelected1 = false;
        	isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;
      	break;
		//QGLViewer::keyPressEvent(e);
    }
}
