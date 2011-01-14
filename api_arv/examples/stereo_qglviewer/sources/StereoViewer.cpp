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

#include "StereoViewer.h"
#include <QApplication>
#include <math.h>

using namespace std;

void Viewer::init() {
    //restoreStateFromFile();

    // Activate the stereo display. Press 'S' to toggle.
    setStereoDisplay(true);

    setFullScreen(true);
    //help();
    }

QString Viewer::helpString() const {
    QString text("<h2>S t e r e o V i e w e r</h2>");
    text += "You can display in stereo with no change to your application, provided that your hardware supports stereo display.<br><br>";

    text += "If you get a <b>Stereo not supported on this display</b> error message, check that ";
    text += "your machine supports stereo (search for quad-buffer in <i>glxinfo</i> and find stereo glasses !).<br><br>";

    text += "You can then toggle the stereo display by pressing <b>S</b> in any application.";
    return text;
    }

void Viewer::draw() {
    // Draws a spiral
    const float nbSteps = 200.0;
    glBegin(GL_QUAD_STRIP);
    for (float i = 0; i < nbSteps; ++i) {
        float ratio = i / nbSteps;
        float angle = 21.0 * ratio;
        float c = cos(angle);
        float s = sin(angle);
        float r1 = 1.0 - 0.8 * ratio;
        float r2 = 0.8 - 0.8 * ratio;
        float alt = ratio - 0.5;
        const float nor = 0.5;
        const float up = sqrt(1.0 - nor * nor);
        glColor3f(1 - ratio, 0.2 , ratio);
        glNormal3f(nor*c, up, nor*s);
        glVertex3f(r1*c, alt, r1*s);
        glVertex3f(r2*c, alt + 0.05, r2*s);
        }
    glEnd();
    }

