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

	bool isSelected1 ;
    bool isSelected2 ;
  	bool isSelected3 ;
  	bool isSelected4 ;

	Scene*   _scene;
	ViewerG* _viewerG;
	ViewerD* _viewerD;
	qglviewer::Camera*  _cameraCenter;

	float ecartementYeux;
	float inclinaisonStrabisme;

protected:	
	virtual void keyPressEvent(QKeyEvent *e);

};


#endif
