/*
 *  AGHMainWindow.h
 *  RV
 *
 *  Created by Adrien on 20/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include <QtGui>
#include <QString>
#include "AGHFile.h"
#include "viewerG.h"
#include "viewerD.h"
#include "widgetScene.h"

class AGHMainWindow : public QMainWindow {
	Q_OBJECT
public:
	AGHMainWindow();
	~AGHMainWindow();
	void createMenu();
	void createDock();
public slots:
	void openFile();
	void closeFile();
	void play();
	void playNote();	
	void openWindow();	
private:
	void initSounds();
	void playSoundFromNote(int note);



	bool _isPlaying;
	AGHFile * _currentFile;
	QLabel * _fileLabel;
	QLabel * _currentNoteLabel;
	QPushButton * _playPauseButton;
	QSpinBox * _speedBox;
	QTimer * _timer;
	QSound * _do;
	QSound * _re;
	QSound * _mi;
	QSound * _fa;
	QSound * _sol;
	WidgetScene * _openGLGlobalWindow;

};
