#ifndef VIEWERD_H
#define VIEWERD_H

#include <QGLViewer/qglviewer.h>


class Scene;

class ViewerD : public QGLViewer
{
public:
  ~ViewerD();
  
  void loadScene(const QString& name);

  const Scene* const scene() const { return scene_; };

  void setScene(Scene* const scene) { scene_ = scene; };

  void setParametersStereo(qglviewer::Vec dirRight,qglviewer::Vec dirUp, float& ecart, float& divergence ) {divergence_=divergence; ecart_=ecart; dirRight_=dirRight; dirUp_=dirUp;};
  void updateCameraViewer();


protected :
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;
  virtual void keyPressEvent(QKeyEvent *e);
  virtual void select(const QPoint& point);

  Scene* const scene() { return scene_; };

private:
  // Definit la position F1 comme étant celle de la camera.
  void initFromScene();



  qglviewer::Vec dirRight_;
  qglviewer::Vec dirUp_;
  float ecart_;
  float divergence_;


  Scene* scene_;
};

#endif // VIEWER_H
