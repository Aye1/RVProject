#include "viewer.h"
#include "scene.h"
#include <qfiledialog.h>
#include <QKeyEvent>
#include <math.h>
#include "elementBat.h"

using namespace std;
using namespace qglviewer;



Viewer::~Viewer()
{
}

void Viewer::draw()
{
  // A FAIRE
  /*
  //Dessin de la sphere 1,2,3
  Sphere   s1,s2,s3;
  Frame    f1,f2,f3;
  Material m1,m2,m3;
  
  m1 = s1.material();
  f1 = s1.frame();
  m2 = s2.material();
  f2 = s2.frame();
  m3 = s3.material();
  f3 = s3.frame();

  m1.setDiffuseColor( Color(1.0 , 0.0 , 0.2) );
  m2.setDiffuseColor( Color(1.0 , 0.6 , 0.0) );
  m3.setDiffuseColor( Color(0.0 , 0.5 , 0.8) );
  
  f1.setPosition(0.0,0.0,0.0);
  f2.setPosition(2.0,0.0,0.0);
  f3.setPosition(0.0,2.0,0.0);

  s1.setMaterial(m1);
  s2.setMaterial(m2);
  s3.setMaterial(m3);

  s1.setFrame(f1);
  s2.setFrame(f2);
  s3.setFrame(f3);
  
  s1.draw();
  s2.draw();
  s3.draw();





  const float nbSteps = 200.0;

  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8*ratio;
      float r2 = 0.8 - 0.8*ratio;
      float alt = ratio - 0.5;
      const float nor = .5;
      const float up = sqrt(1.0-nor*nor);
      glColor3f(1.0-ratio, 0.2 , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05, r2*s);
    }
  glEnd();
  */

  
  if ( scene_ ) {
	scene_->draw();
  }
 
  	

}

void Viewer::init()
{
		isSelected1 = false;
        isSelected2 = false;
  		isSelected3 = false;
  		isSelected4 = false;

  // Key description for help window (press 'H')
  setKeyDescription(Qt::Key_L, "Loads a new scene");
  setKeyDescription(Qt::Key_S, "Shoot rays in the scene and saves the result");
  setKeyDescription(Qt::SHIFT+Qt::Key_S, "Shoot rays from <i>current</i> view point");

  setScene(new Scene());

  // So that transparent materials are correctly displayed.
  // Disable if rendering is too slow.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Restore previous viewer state (camera, key frames...)
  restoreStateFromFile();

  // Loads scene (prevents from pressing 'L' at each start).
  loadScene("troisSpheres.scn");

  // Set Camera to scene Camera. Set scene center and radius.
  initFromScene();
}

QString Viewer::helpString() const
{
  QString text("<h2>L a n c e r   d e   r a y o n s</h2>");
  text += "Ajoutez ici toute aide utile à l'utilisateur. ";
  text += "N'oubliez pas de commenter vos raccourcis clavier avec setKeyDescription().<br>";
  text += "Appuyez sur <b>Escape</b> pour quitter.";
  return text;
}

void Viewer::keyPressEvent(QKeyEvent *e)
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

void Viewer::loadScene(const QString& name)
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
void Viewer::initFromScene()
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

void Viewer::select(const QPoint& point)
{
  
  Vec orig,dir;
  camera()->convertClickToLine(point,orig,dir);
}
