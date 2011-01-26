#ifndef WIDGETSCENE_H
#define WIDGETSCENE_H

#include <QtGui>
#include <QString>
#include "AGHFile.h"
#include "viewerG.h"
#include "viewerD.h"
#include "scene.h"

class WidgetScene : public QWidget {
	Q_OBJECT
public:
	WidgetScene();
	~WidgetScene();

	void init();

private:

	Scene*   _scene;
	ViewerG* _viewerG;
	ViewerD* _viewerD;
	qglviewer::Camera*  _cameraCenter;

};


#endif
