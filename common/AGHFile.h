/*
 *  AGHFile.h
 *  RV
 *
 *  Created by Adrien on 18/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#ifndef AGHFILE_H__
#define AGHFILE_H__

#include <QString>
#include <QFile>
#include <stdio.h>
#include <stdlib.h>


class AGHFile : public QFile {
	public :
	AGHFile(const QString & name) : QFile(name) {};
	
	int nextNote();
	void init();
};

#endif //AGHFILE_H__
