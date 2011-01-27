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
	int number = 0;
	for(int i=0; i<line.size()-1; i++) {	
	    char tmp = line.at(i);
	    number = number * 10 + atoi(&tmp);
        }
	return number;
}

void AGHFile::init() {
	this->open(QIODevice::ReadOnly | QIODevice::Text);
}

