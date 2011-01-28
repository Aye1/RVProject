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
	updateCameraViewer();
	float dx,dy,dz;
	dx = dirRight_.x * ecart_;
	dy = dirRight_.y * ecart_;
	dz = dirRight_.z * ecart_;
	glTranslatef(dx,dy,dz);

	float theta = atan(ecart_/divergence_);
	glRotated(theta,dirUp_.x,dirUp_.y,dirUp_.z);
		
  
  if ( scene_ ) {
	  scene_->draw();
  }

	update();
	

}

void ViewerD::init()
{

setFocusPolicy(Qt::NoFocus);
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
	Q_UNUSED(e);
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
  /*camera()->setPosition(scene()->camera().position());
  camera()->setOrientation(scene()->camera().orientation());
  camera()->setFieldOfView(scene()->camera().fieldOfView());
  
  // Remove previous keyFrames in path 1 (if any)
  if (camera()->keyFrameInterpolator(1))
    camera()->keyFrameInterpolator(1)->deletePath();

  // Add current (i.e. scene camera) position to F1.
  camera()->addKeyFrameToPath(1);
*/
}

void ViewerD::select(const QPoint& point)
{
  Vec orig,dir;
  camera()->convertClickToLine(point,orig,dir);
}

void ViewerD::updateCameraViewer(){
	  camera()->setPosition(scene()->camera().position());
  	camera()->setOrientation(scene()->camera().orientation());
  	camera()->setFieldOfView(scene()->camera().fieldOfView());
}
