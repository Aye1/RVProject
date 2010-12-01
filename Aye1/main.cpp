/*
 *  main.cpp
 *  RV
 *
 *  Created by Adrien on 18/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include <iostream>
#include "AGHMainWindow.h"
#include <QTimer>
#include <QObject>
#include <QApplication>

using namespace std;

int main(int argc, char** argv) {
	
	/* Arguments management */
	if (argc < 1) {
		cout << "Argument error, expecting : " << endl;
		return -1;
	}
	
	QApplication app(argc, argv);
	AGHMainWindow window;
	window.resize(1024, 768);
	
	AGHFile file(argv[1]);
	file.init();
	
	window.show();
	return app.exec();
}