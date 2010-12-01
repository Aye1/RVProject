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
	
	QMenu * menuFichier = menuBar()->addMenu("&Fichier");
	
	QAction * actionOuvrir = new QAction("&Ouvrir", this);
	menuFichier->addAction(actionOuvrir);
	connect(actionOuvrir, SIGNAL(triggered()), this, SLOT(openFile()));
	
	QAction * actionQuitter = new QAction("&Quitter", this);
	menuFichier->addAction(actionQuitter);
	connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
	
	QDockWidget * paramDock = new QDockWidget("Parametres", this);
	addDockWidget(Qt::LeftDockWidgetArea, paramDock);
	
	QWidget * paramDockContent = new QWidget;
	paramDock->setWidget(paramDockContent);
	
	QLabel * speedLabel = new QLabel("Vitesse");
	QLineEdit * speedField = new QLineEdit;
	
	QVBoxLayout * paramLayout = new QVBoxLayout;
	paramLayout->addWidget(speedLabel);
	paramLayout->addWidget(speedField);
	
	paramDockContent->setLayout(paramLayout);
		
	QWidget * mainWidget = new QWidget;
	mainWidget->resize(800,600);
	QGridLayout * mainLayout = new QGridLayout();
	
	_fileLabel = new QLabel("Pas de fichier ouvert");
	mainLayout->addWidget(_fileLabel, 0, 0, Qt::AlignLeft | Qt::AlignTop);
	
	_playPauseButton = new QPushButton("Play");
	_playPauseButton->setEnabled(false);
	mainLayout->addWidget(_playPauseButton, 1, 0, Qt::AlignLeft);
	
	mainWidget->setLayout(mainLayout);
		
	setCentralWidget(mainWidget);
}

AGHMainWindow::~AGHMainWindow() {
}

void AGHMainWindow::openFile() {
	printf("opening");
	QString filename = QFileDialog::getOpenFileName(this, "Open Image", ".", "LEJEA files (*.lejea)");
	_currentFile = new QFile(filename);
	_fileLabel->setText("Fichier ouvert : " + filename);
}

void AGHMainWindow::closeFile() {
	_currentFile->close();
}