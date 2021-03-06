/*
 *  AGHMainWindow.h
 *  RV
 *
 *  Created by Adrien on 20/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#ifndef AGHMAINWINDOW_H__
#define AGHMAINWINDOW_H__

#include <QtGui>
#include <QString>
#include "AGHFile.h"
//#include <QGLViewer/QGLViewer.h>

#include "ui_AGHMainWindow.h"

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

private:

	AGHFile * _currentFile;
	QLabel * _fileLabel;
	QLabel * _currentNoteLabel;
	QPushButton * _playPauseButton;
	QSpinBox * _speedBox;
	QTimer * _timer;

private:
private:
/*  Qt GUI exported from QtDesigner ********************************************************/
	 Ui_MainWindow ui;
};

#endif
