
#ifndef SIMPLEVIEWER_H_
#define SIMPLEVIEWER_H_

#include <QGLViewer/qglviewer.h>
#include <qstring.h>

class Viewer : public QGLViewer
{
public:


protected:
  virtual void draw();
  virtual void init();
  virtual QString helpString() const;

private:

};

#endif // SIMPLEVIEWER_H_
