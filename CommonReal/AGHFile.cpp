/*
 *  AGHFile.cpp
 *  RV
 *
 *  Created by Adrien on 18/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include "AGHFile.h"
#include <stdlib.h>
int AGHFile::nextNote() {
	if (this->atEnd()) {
		return -1;
	}
	QByteArray line = this->readLine();
	std::cout << "taille : " << line.size() << endl;
	char number = line.at(0);
	return atoi(&number);
}

void AGHFile::init() {
	this->open(QIODevice::ReadOnly | QIODevice::Text);
}

