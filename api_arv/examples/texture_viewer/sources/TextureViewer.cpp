/****************************************************************************

Copyright (C) 2002-2007 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
Version 2.2.6-3, released on August 28, 2007.

http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

libQGLViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

libQGLViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libQGLViewer; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include "TextureViewer.h"

#include <QImage>
#include <QFileDialog>
#include <ApiArv/Image.h>

using namespace qglviewer;
using namespace std;
using namespace arv;

void Viewer::init() {
    //restoreStateFromFile();

    // Enable GL textures
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);

    setKeyDescription(Qt::Key_L, "Loads a new image");

    loadImage();
    help();
    }

void Viewer::draw() {
    // Display the quad
    glNormal3f(0.0, 0.0, 1.0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2f(-1,-1);
        glTexCoord2f(0, 0); glVertex2f(-1, 1);
        glTexCoord2f(1, 0); glVertex2f( 1, 1);
        glTexCoord2f(1, 1); glVertex2f( 1,-1);
    glEnd();
    }


void Viewer::loadImage() {
    Image image("image.png");
    image.loadTexture();
    }


void Viewer::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_L :
            loadImage();
            break;
        default:
            QGLViewer::keyPressEvent(e);
        }
    }

QString Viewer::helpString() const {
    QString text("<h2>T e x t u r e V i e w e r</h2>");
    text += "This pedagogical example illustrates how to texture map a polygon.<br><br>";
    text += "The Qt <i>QImage</i> class and its <i>convertToGLFormat()</i> function are used ";
    text += "to load an image in any format. Feel free to cut and paste.<br><br>";
    text += "Press <b>L</b>(oad) to load a new image.";
    return text;
    }
