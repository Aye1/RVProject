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
}

AGHMainWindow::~AGHMainWindow() {
}

void AGHMainWindow::createMenu() {
	QMenu * menuFichier = menuBar()->addMenu("&Fichier");
	
	/* Ouvrir un fichier */
	QAction * actionOuvrir = new QAction("&Ouvrir", this);
	menuFichier->addAction(actionOuvrir);
	actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
	connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(openFile()));
	
	/* Fermer un fichier */
	QAction * actionFermer = new QAction("&Fermer", this);
	menuFichier->addAction(actionFermer);
	actionFermer->setShortcut(QKeySequence("Ctrl+W"));
	connect(actionFermer, SIGNAL(triggered()), this, SLOT(closeFile()));
	
	/* Quitter le programme */
	QAction * actionQuitter = new QAction("&Quitter", this);
	menuFichier->addAction(actionQuitter);
	actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
	connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void AGHMainWindow::createDock() {
	QDockWidget * paramDock = new QDockWidget("Parametres", this);
	addDockWidget(Qt::LeftDockWidgetArea, paramDock);
	
	QWidget * paramDockContent = new QWidget;
	paramDock->setWidget(paramDockContent);
	
	QLabel * speedLabel = new QLabel("Vitesse");
	_speedBox = new QSpinBox();
	_speedBox->setMaximum(400);
	_speedBox->setValue(100);
	
	QGridLayout * paramLayout = new QGridLayout;
	paramLayout->addWidget(speedLabel, 0, 0, Qt::AlignTop);
	paramLayout->addWidget(_speedBox, 1, 0, Qt::AlignTop);
	
	paramDockContent->setLayout(paramLayout);
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
	float T = 60/_speedBox->value();
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
