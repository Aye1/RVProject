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
	
	createMenu();
	createDock();
	initSounds();
	_currentFile = NULL;
		
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
	//mainWidget->adjustSize();
		
	setCentralWidget(mainWidget);
	
	_isPlaying = false;
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

	/*Ouvrir la fenetre OpenGL*/
	QAction* actionOuvrirOpenGL = new QAction("Ouvrir la fenetre de &jeu", this);
	menuFichier->addAction(actionOuvrirOpenGL);
	actionOuvrirOpenGL->setShortcut(QKeySequence("Ctrl+J"));
	connect(actionOuvrirOpenGL, SIGNAL(triggered()),this, SLOT(openWindow()) );
	
}

void AGHMainWindow::initSounds() {
	_vert = new QSound("cymbale.wav");
	_rouge = new QSound("claire.wav");
	_jaune = new QSound("charleston.wav");
	_bleu = new QSound("tome.wav");
}

void AGHMainWindow::createDock() {
	QDockWidget * paramDock = new QDockWidget("Parametres", this);
	addDockWidget(Qt::LeftDockWidgetArea, paramDock);
	
	QWidget * paramDockContent = new QWidget;
	paramDock->setWidget(paramDockContent);
	
	QLabel * speedLabel = new QLabel("Vitesse");
	_speedBox = new QSpinBox();
	_speedBox->setMaximum(400);
	_speedBox->setValue(50);
	
	QGridLayout * paramLayout = new QGridLayout;
	paramLayout->addWidget(speedLabel, 0, 0, Qt::AlignTop);
	paramLayout->addWidget(_speedBox, 1, 0, Qt::AlignTop);
	
	paramDockContent->setLayout(paramLayout);
}

void AGHMainWindow::openFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Open Song", ".", "LEJEA files (*.lejea)");
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
	if (_isPlaying) {
		_timer->stop();
		_playPauseButton->setText("Play");
		_isPlaying = false;
	} else {
		_playPauseButton->setText("Pause");
		float T = 60/(float)_speedBox->value();
		_timer = new QTimer();
		_timer->start(T*1000);
		connect(_timer, SIGNAL(timeout()), this, SLOT(playNote()));
		_isPlaying = true;
	}
}

void AGHMainWindow::playNote() {
	int note = _currentFile->nextNote();
	if (note == -1) {
		_timer->stop();
		_currentFile->seek(0);
		_playPauseButton->setText("Play");
		_isPlaying = false;
	} else {
		_currentNoteLabel->setText("Note : " + QString::number(note));
		printf("note : %d \n", note);
		playSoundFromNote(note);
	}
}

void AGHMainWindow::playSoundFromNote(int note) {
	QSound * soundToPlay;
	if (note & 2) {
		soundToPlay = _vert;
		soundToPlay->play();
	}
	if (note & 4) {
		soundToPlay = _rouge;
		soundToPlay->play();
	}
	if (note & 8) {
		soundToPlay = _jaune;
		soundToPlay->play();
	}
	if (note & 16) {
		soundToPlay = _bleu;
		soundToPlay->play();
	}
}

void AGHMainWindow::openWindow(){
	QDesktopWidget desktop;
	QRect screensize = desktop.screenGeometry(desktop.primaryScreen());
	_openGLGlobalWindow = new WidgetScene();
	_openGLGlobalWindow->getScene()->setFile(_currentFile);
	float T = 60.0f/(float)_speedBox->value();
	_openGLGlobalWindow->getScene()->setTimeBetweenNotes(T);
	_openGLGlobalWindow->resize(screensize.width()*2,screensize.height());
	_openGLGlobalWindow->init();
	_openGLGlobalWindow->show();
}

