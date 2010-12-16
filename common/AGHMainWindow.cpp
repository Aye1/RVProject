/*
 *  AGHMainWindow.cpp
 *  RV
 *
 *  Created by Adrien on 20/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include "AGHMainWindow.h"

AGHMainWindow::AGHMainWindow() {
	
	ui.setupUi(this);

	//connect des actions
	connect(ui.actionOuvrir, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionFermer, SIGNAL(triggered()), this, SLOT(closeFile()));

/*
	createMenu();
	createDock();
		
	QWidget * mainWidget = new QWidget;
	mainWidget->resize(800,600);
	QGridLayout * mainLayout = new QGridLayout();
	
	_fileLabel = new QLabel("Pas de fichier ouvert");
	mainLayout->addWidget(_fileLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	
	_playPauseButton = new QPushButton("Play");
	_playPauseButton->setEnabled(false);
	mainLayout->addWidget(_playPauseButton, 1, 0, Qt::AlignLeft);
	connect(_playPauseButton, SIGNAL(clicked()), this, SLOT(play()));
	
	_currentNoteLabel = new QLabel("Note : ");
	mainLayout->addWidget(_currentNoteLabel, 0, 2, Qt::AlignRight | Qt::AlignTop);
	
	mainWidget->setLayout(mainLayout);
		
	setCentralWidget(mainWidget);
*/
}

AGHMainWindow::~AGHMainWindow() {
}


void AGHMainWindow::openFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open Image", ".", "LEJEA files (*.lejea)");
	_currentFile = new AGHFile(filename);
	_currentFile->init();
	_fileLabel->setText("Fichier ouvert : " + filename);
	_playPauseButton->setEnabled(true);
}

void AGHMainWindow::closeFile() {
	_currentFile->close();
	_playPauseButton->setEnabled(false);
	_fileLabel->setText("Pas de fichier ouvert");
}

void AGHMainWindow::play() {
	_playPauseButton->setText("Pause");
	float T = 60/((ui.vitesse_spinbox)->value());
	_timer = new QTimer();
	_timer->start(T*1000);
	connect(_timer, SIGNAL(timeout()), this, SLOT(playNote()));
}

void AGHMainWindow::playNote() {
	int note = _currentFile->nextNote();
	if (note == -1) {
		_timer->stop();
	}
	printf("note : %d \n", note);
}

Ui_MainWindow AGHMainWindow::getUI() {
	return ui;
}
