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

int main(int argc, char** argv) {
    QApplication application(argc, argv);

    // Set the default OpenGL format with stereo capabilities.
    // An alternative is to pass this parameter to the QGLWidget constructor.
    QGLFormat format;
    format.setStereo(true);
    QGLFormat::setDefaultFormat(format);

    Viewer viewer;
    viewer.setWindowTitle("stereoViewer");
    viewer.show();

    return application.exec();
    }
