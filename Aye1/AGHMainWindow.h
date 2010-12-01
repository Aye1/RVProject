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
//#include <QGLViewer/QGLViewer.h>

class AGHMainWindow : public QMainWindow {
	Q_OBJECT
public:
	AGHMainWindow();
	~AGHMainWindow();
	void createMenu();
public slots:
	void openFile();
	void closeFile();
	void play();
private:
	QFile * _currentFile;
	QLabel * _fileLabel;
	QPushButton * _playPauseButton;
};