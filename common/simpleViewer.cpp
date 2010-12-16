#include "simpleViewer.h"

using namespace std;

void Viewer::init()
{
  setSceneRadius(5.0);
  showEntireScene();
  setGridIsDrawn(true);
}

QString Viewer::helpString() const
{
  QString text("<h2>S i m p l e V i e w e r</h2>");
  text += "Use the mouse to move the camera around the object. ";
  text += "You can revolve around, zoom and translate with the three buttons. ";
  text += "Left and middle buttons pressed together rotate around the camera z axis<br><br>";
  text += "Pressing <b>Alt</b> and one of the function key (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
  text += "Simply press the function key again to restore it. Several keyFrames define a ";
  text += "camera path. Paths are saved when you quit the application.<br><br>";
  text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
  text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot.<br><br>";
  text += "A double click aligns the closer axis with the camera (left, if close enough), fits ";
  text += "the zoom of the camera (middle) or re-center the scene (right).<br>";
  text += "A left double click while right button is pressed defines the <i>revolveAroundPoint</i>.";
  text += "With middle button pressed instead, it zooms on the pixel.<br><br>";
  text += "See also the <b>Shortcuts</b> and <b>Mouse</b> tabs and the documentation web pages.<br>";
  text += "Press <b>Escape</b> to exit the viewer.";
  return text;
}

void Viewer::draw()
{
 //TODO
}

