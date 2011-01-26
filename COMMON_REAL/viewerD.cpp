#include "viewerD.h"
#include "scene.h"
#include <qfiledialog.h>
#include <QKeyEvent>
#include <math.h>
#include "elementBat.h"
#include "baguette.h"

using namespace std;
using namespace qglviewer;



ViewerD::~ViewerD()
{
}

void ViewerD::draw()
{
 
 // glViewport (0, 0, (GLsizei)(400), (GLsizei)(400));
	glTranslatef(5.0,0.0,0.0);
		
  
  if ( scene_ ) {
	scene_->draw();
  }

	

}

void ViewerD::init()
{
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;

  // Key description for help window (press 'H')
  setKeyDescription(Qt::Key_L, "Loads a new scene");
  setKeyDescription(Qt::Key_S, "Shoot rays in the scene and saves the result");
  setKeyDescription(Qt::SHIFT+Qt::Key_S, "Shoot rays from <i>current</i> view point");
	
  //setStereoDisplay(true);

  //setScene(new Scene());

  // So that transparent materials are correctly displayed.
  // Disable if rendering is too slow.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  // Restore previous viewer state (camera, key frames...)
  //restoreStateFromFile();

	//setFullScreen();

  // Loads scene (prevents from pressing 'L' at each start).
 // loadScene("troisSpheres.scn");

  // Set Camera to scene Camera. Set scene center and radius.
  initFromScene();

}

QString ViewerD::helpString() const
{
  QString text("<h2>Simulation de batterie ARV</h2>");
  text += "Ajoutez ici toute aide utile à l'utilisateur. ";
  text += "N'oubliez pas de commenter vos raccourcis clavier avec setKeyDescription().<br>";
  text += "Appuyez sur <b>Escape</b> pour quitter.";
  return text;
}

void ViewerD::keyPressEvent(QKeyEvent *e)
{
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
	case Qt::Key_5 :
		(scene()->getListeBaguette())[0]->setDirectionBaguette((scene()->getListeBaguette())[0]->getDirectionBaguette()+Vec(0.1,0.1,0.1));
		update();
		break;
	
	//deplacement des elements
	case Qt::Key_Up :
		if(isSelected1) {
			((scene()->getListeBatterie())[0])->setInclinaison(((scene()->getListeBatterie())[0])->getInclinaison()-1);
		} else if(isSelected2) {
			((scene()->getListeBatterie())[1])->setInclinaison(((scene()->getListeBatterie())[1])->getInclinaison()-1);
		} else if(isSelected3) {
			((scene()->getListeBatterie())[2])->setInclinaison(((scene()->getListeBatterie())[2])->getInclinaison()-1);
		} else if(isSelected4) {
			((scene()->getListeBatterie())[3])->setInclinaison(((scene()->getListeBatterie())[3])->getInclinaison()-1);
		}
		update();	
		break;
	case Qt::Key_Down :
		if(isSelected1) {
			((scene()->getListeBatterie())[0])->setInclinaison(((scene()->getListeBatterie())[0])->getInclinaison()+1);
		} else if(isSelected2) {
			((scene()->getListeBatterie())[1])->setInclinaison(((scene()->getListeBatterie())[1])->getInclinaison()+1);
		} else if(isSelected3) {
			((scene()->getListeBatterie())[2])->setInclinaison(((scene()->getListeBatterie())[2])->getInclinaison()+1);
		} else if(isSelected4) {
			((scene()->getListeBatterie())[3])->setInclinaison(((scene()->getListeBatterie())[3])->getInclinaison()+1);
		}	
		update();
		break;
	case Qt::Key_Left :
		if(isSelected1) {
			((scene()->getListeBatterie())[0])->setAngleRotationBat(((scene()->getListeBatterie())[0])->getAngleRotationBat()-1);
		} else if(isSelected2) {
			((scene()->getListeBatterie())[1])->setAngleRotationBat(((scene()->getListeBatterie())[1])->getAngleRotationBat()-1);
		} else if(isSelected3) {
			((scene()->getListeBatterie())[2])->setAngleRotationBat(((scene()->getListeBatterie())[2])->getAngleRotationBat()-1);
		} else if(isSelected4) {
			((scene()->getListeBatterie())[3])->setAngleRotationBat(((scene()->getListeBatterie())[3])->getAngleRotationBat()-1);
		}
		update();
		break;
	case Qt::Key_Right :
		if(isSelected1) {
			((scene()->getListeBatterie())[0])->setAngleRotationBat(((scene()->getListeBatterie())[0])->getAngleRotationBat()+1);
		} else if(isSelected2) {
			((scene()->getListeBatterie())[1])->setAngleRotationBat(((scene()->getListeBatterie())[1])->getAngleRotationBat()+1);
		} else if(isSelected3) {
			((scene()->getListeBatterie())[2])->setAngleRotationBat(((scene()->getListeBatterie())[2])->getAngleRotationBat()+1);
		} else if(isSelected4) {
			((scene()->getListeBatterie())[3])->setAngleRotationBat(((scene()->getListeBatterie())[3])->getAngleRotationBat()+1);
		}
		update();
		break;

    case Qt::Key_L :
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;

#if QT_VERSION < 0x040000
      loadScene(QFileDialog::getOpenFileName("", "Scenes (*.scn);;All files (*)", this));
#else
      loadScene(QFileDialog::getOpenFileName(this, "Select a scene", ".", "Scenes (*.scn);;All files (*)"));
#endif
      break;
    case Qt::Key_S :
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;
#if QT_VERSION < 0x040000
      if ((e->state() == Qt::NoButton) || (e->state() == Qt::ShiftButton))
#else
      if ((e->modifiers() == Qt::NoModifier) || (e->modifiers() == Qt::ShiftModifier))
#endif	
	{
#if QT_VERSION < 0x040000
	  if (e->state() == Qt::ShiftButton)
#else
	  if (e->modifiers() == Qt::ShiftModifier)
#endif
	    {
	      // Shift+S renders image from current view point
	      scene()->camera().frame().setPosition(camera()->position());
	      scene()->camera().frame().setOrientation(camera()->orientation());
	    }
	  
	  // Remplacer cette ligne par : const QString name = "result.jpg";
	  // pour ne pas avoir à choisir un nom à chaque fois.
	  const QString name = QFileDialog::getSaveFileName();
	  
	  
#if QT_VERSION < 0x040000
	  if ((e->state() == Qt::ShiftButton) && (camera()->keyFrameInterpolator(1)))
#else
	  if ((e->modifiers() == Qt::ShiftModifier) && (camera()->keyFrameInterpolator(1)))
#endif
	    {
	      // Restore initial scene camera
	      scene()->camera().frame().setPosition(camera()->keyFrameInterpolator(1)->keyFrame(0).position());
	      scene()->camera().frame().setOrientation(camera()->keyFrameInterpolator(1)->keyFrame(0).orientation());
	    }
	  break;
	}
    default :
	  	isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;
      QGLViewer::keyPressEvent(e);
    }
}

void ViewerD::loadScene(const QString& name)
{
  if (name.isEmpty())
    return;

  scene()->loadFromFile(name);
  
  // Change QGLViewer settings according to scene
  setSceneCenter(scene()->center());
  setSceneRadius(scene()->radius(sceneCenter()));

  // Set first camera key frame (F1) to camera position.
  initFromScene();
}

// Make the first keyFrameInterpolator (binded to F1) contain one position, that cooresponds to the
// scene's camera position. Attention, camera() is moved to camera position.
void ViewerD::initFromScene()
{
  camera()->setPosition(scene()->camera().frame().position());
  camera()->setOrientation(scene()->camera().frame().orientation());
  camera()->setFieldOfView(scene()->camera().fieldOfView());
  
  // Remove previous keyFrames in path 1 (if any)
  if (camera()->keyFrameInterpolator(1))
    camera()->keyFrameInterpolator(1)->deletePath();

  // Add current (i.e. scene camera) position to F1.
  camera()->addKeyFrameToPath(1);

}

void ViewerD::select(const QPoint& point)
{
  Vec orig,dir;
  camera()->convertClickToLine(point,orig,dir);
}
