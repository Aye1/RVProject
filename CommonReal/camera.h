
#ifndef CAMERA_H
#define CAMERA_H

#include <QGLViewer/qglviewer.h>
class Scene;

class Camera : public qglviewer::Camera
{
public :
  Camera();
  
  qglviewer::Frame& frame() { return frame_; };  
  const qglviewer::Frame& frame() const { return frame_; };
  float fieldOfView() const { return fieldOfView_; };
  int xResolution() const { return xResolution_; };
  int yResolution() const { return yResolution_; };
  
  void setFrame(const qglviewer::Frame& frame) { frame_ = frame; };
  // fieldOfView is the VERTICAL value (in radians).
  // Horizontal field of view is infered from xResolution/yResolution ratio.
  void setFieldOfView(float fieldOfView) { fieldOfView_ = fieldOfView; };
  void setXResolution(int xResolution);
  void setYResolution(int yResolution);
  
  void initFromDOMElement(const QDomElement& e);
  void draw(float radius) const;

  
  void drawAllRays() const;
  
private:  
  qglviewer::Frame frame_;
  float fieldOfView_;
  int xResolution_, yResolution_;
};

#endif // CAMERA_H
