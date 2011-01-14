//#include "viewer.h"
#include <qapplication.h>
#include "AGHMainWindow.h"

int main(int argc, char** argv)
{
  // Read command lines arguments.
  QApplication application(argc,argv);
/*
	QGLFormat format;
	format.setStereo(true);
	QGLFormat::setDefaultFormat(format);
*/
  // Instantiate the viewer.
  //Viewer v;
  AGHMainWindow window;


  // Make the viewer window visible on screen.
  //v.show();
  window.show();
  
#if QT_VERSION < 0x040000
  // Set the viewer as the application main widget.
  application.setMainWidget(&window);
#endif
    
  // Run main loop.
  return application.exec();
}
