/*
 *  AGHFile.h
 *  RV
 *
 *  Created by Adrien on 18/11/10.
 *  Copyright 2010 Ensimag. All rights reserved.
 *
 */

#include <QString>
#include <QFile>

class AGHFile : public QFile {
	public :
	AGHFile(const QString & name) : QFile(name) {};
	
	int nextNote();
	void init();
};

