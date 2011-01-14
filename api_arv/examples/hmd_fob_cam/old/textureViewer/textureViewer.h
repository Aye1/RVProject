/****************************************************************************

 This file is part of the QGLViewer library.
 Copyright (C) 2002, 2003, 2004, 2005 Gilles Debunne (Gilles.Debunne@imag.fr)
 Version 1.3.9-5, released on March 11, 2005.

 http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

 libQGLViewer is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 libQGLViewer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with libQGLViewer; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer
{
protected :
  virtual void init();
  virtual void draw();
  virtual void keyPressEvent(QKeyEvent *e);
  virtual QString helpString() const;

  void loadImage();

private :
  float ratio, u_max, v_max;
};
