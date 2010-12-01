/*
 *  main.cpp
 *  RV
 *
 *  Created by Adrien on 18/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include <iostream>
#include "AGHFile.h"
#include "AGHMainWindow.h"
#include <QTimer>
#include <QObject>
#include <QApplication>

using namespace std;

void playNote() {
	cout << "note" << endl;
}

int main(int argc, char** argv) {
	
	/* Arguments management */
	if (argc < 1) {
		cout << "Argument error, expecting : " << endl;
		return -1;
	}
	
	QApplication app(argc, argv);
	AGHMainWindow window;
	
	AGHFile file(argv[1]);
	file.init();
	
	/*QTimer * timer = new QTimer();
	QObject::connect(timer, SIGNAL(timeout()), qApp, SLOT(playNote()));
	timer->start(1000);*/
	
	cout << "Hello RV !" << endl;
	window.show();
	return app.exec();
}