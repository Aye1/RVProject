#ifndef VIEWER_H
#define VIEWER_H

#include <QGLViewer/qglviewer.h>


class Scene;

class Viewer : public QGLViewer
{
public:
  ~Viewer();
  
  void loadScene(const QString& name);

  const Scene* const scene() const { return scene_; };

  void setScene(Scene* const scene) { scene_ = scene; };

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

  bool isSelected1;
  bool isSelected2;
  bool isSelected3;
  bool isSelected4;

  Scene* scene_;
};

#endif // VIEWER_H
