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
	//QMdiArea * area = new QMdiArea();
	
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
	
	QPushButton * placeholder = new QPushButton("Placeholder");
	
	QWidget * mainWidget = new QWidget;
	QGridLayout * mainLayout = new QGridLayout();
	
	mainWidget->setLayout(mainLayout);
	
	//QMdiSubWindow * parametersWindow = new QMdiSubWindow(paramDock);
	
	setCentralWidget(placeholder);
}

AGHMainWindow::~AGHMainWindow() {
}

void AGHMainWindow::openFile() {
	printf("opening");
	QString filename = QFileDialog::getOpenFileName(this, "Open Image", ".", "LEJEA files (*.lejea)");
	_currentFile = new QFile(filename);
}

void AGHMainWindow::closeFile() {
	_currentFile->close();
}