#include "widgetScene.h" 
#include  <QGLViewer/qglviewer.h>

WidgetScene::WidgetScene(){
	_scene = new Scene();
	_scene->loadFromFile("troisSpheres.scn");

	_viewerG = new ViewerG();
	_viewerD = new ViewerD();


	/////////////////////CAMERA
	_cameraCenter = new qglviewer::Camera();
	_cameraCenter->setPosition(_scene->camera().frame().position());
  	_cameraCenter->setOrientation(_scene->camera().frame().orientation());
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
	qglviewer::Vec cameraDirOrth;
	cameraDirOrth = _cameraCenter->rightVector();
	qglviewer::Vec dirOrth = cameraDirOrth.unit();
	//ecartementOeil

	_viewerD->setCamera(_cameraCenter);
	//_viewerD->setParametersStereo(dirOrth,ecartementYeux);
	_viewerG->setCamera(_cameraCenter);
	//_viewerG->setParametersStereo(dirOrth,);
	


}

WidgetScene::~WidgetScene(){
	delete _scene ;
	delete _viewerG ;
	delete _viewerD ;
}

void WidgetScene::init()
{
	//initialisation
	bool pb;
	wii=initWii(pb);
	if(pb){
	    cout<<"Connection échouée....GAME OVER"<<endl;
	}
	initFob(fob);
	QHBoxLayout * layout = new QHBoxLayout();
	layout->addWidget(_viewerG);
	layout->addWidget(_viewerD);

	layout->setContentsMargins(0.0,0.0,0.0,0.0);
	layout->setSpacing(0);

	this->setLayout(layout);	

}
