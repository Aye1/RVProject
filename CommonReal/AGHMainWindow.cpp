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
		
	QWidget * mainWidget = new QWidget;
	mainWidget->resize(800,600);
	QGridLayout * mainLayout = new QGridLayout();

	/*
	_vG = new ViewerG();
	_vD = new ViewerD();
*/

	//_viewer->resize(2000,2000);
	//mainLayout->addWidget(_vG, 0, 1, Qt::AlignLeft);
	//mainLayout->addWidget(_vD, 0, 2, Qt::AlignLeft);
	
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
	QAction* actionOuvrirOpenGL = new QAction("ouvrir la fenetre de &jeu", this);
	menuFichier->addAction(actionOuvrirOpenGL);
	actionOuvrirOpenGL->setShortcut(QKeySequence("Ctrl+J"));
	connect(actionOuvrirOpenGL, SIGNAL(triggered()),this, SLOT(openWindow()) );
	
}

void AGHMainWindow::initSounds() {
	_do = new QSound("do.wav");
	_re = new QSound("re.wav");
	_mi = new QSound("mi.wav");
	_fa = new QSound("fa.wav");
	_sol = new QSound("sol.wav");
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
	if (note == 1) {
		soundToPlay = _do;
	} else if (note == 2) {
		soundToPlay = _re;
	} else if (note == 3) {
		soundToPlay = _mi;
	} else if (note == 4) {
		soundToPlay = _fa;
	} else {
		soundToPlay = _sol;
	}
	if (soundToPlay != NULL) {
		soundToPlay->play();
	} else {
		printf("note nulle \n");
	}
}

void AGHMainWindow::openWindow(){
	printf("open window\n");
	QDesktopWidget desktop;
	QRect screensize = desktop.screenGeometry(desktop.primaryScreen());

	_openGLGlobalWindow = new WidgetScene();
	_openGLGlobalWindow->resize(screensize.width()*100,screensize.height());
	_openGLGlobalWindow->init();
	_openGLGlobalWindow->show();
}

