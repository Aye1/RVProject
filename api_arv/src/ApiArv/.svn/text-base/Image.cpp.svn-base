/******************************************
 *  creation :
 *      2008 : Lionel Baboud
 */

#include "Image.h"
#include <GL/gl.h>
#include <QImage>
#include <QString>
#include <iostream>

#define ERROR(msg) std::cerr << "- error in arv::Image : " << msg << "." << std::endl;

namespace arv {

Image::Image(int w, int h, const char *data, Time t) : w(w), h(h), data(const_cast<char*>(data)), t(t) {
    }


Image::Image(const char* fileName) {
    QImage image = QImage(QString(fileName));
    if (image.isNull()) {
        ERROR("problem while loading '" << fileName << "'.")
        return;
        }
    
    w = image.width();
    h = image.height();
    data = new char[3*w*h];
    if (data == NULL) {
        ERROR("cannot allocate memory");
        w = h = 0;
        return;
        }
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        QRgb pix = image.pixel(i, h-1-j);
        data[3*(i + j*w) + 0] = qBlue(pix);
        data[3*(i + j*w) + 1] = qGreen(pix);
        data[3*(i + j*w) + 2] = qRed(pix);
        }
    
    /*
    // The following code loads the image with GL_RGBA format.
    // This allows to correctly load non-power-of-two textures
    // but it is not compatible with output format by cameras (GL_BGR).
    w = image.width();
    h = image.height();
    data = new char[4*w*h];
    if (data == NULL) {
        ERROR("cannot allocate memory");
        w = h = 0;
        return;
        }
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        QRgb pix = image.pixel(i, h-1-j);
        data[4*(i + j*w) + 0] = qRed(pix);
        data[4*(i + j*w) + 1] = qGreen(pix);
        data[4*(i + j*w) + 2] = qBlue(pix);
        data[4*(i + j*w) + 3] = qAlpha(pix);
        }
    */
    }


//void Image::draw(int x, int y, bool background) const {}


void Image::draw(float zoom) const {
    int sx = 0;
    int sy = h - 1;

    // save and set GL state :
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-0.5f, w - 0.5f, -0.5f, h - 0.5f, -1.0, 1.0);   //@@ a voir.. : glOrtho(x, x+w, y, y+h, -1, 1); avec x=y=0
    glViewport(0, 0, w, h);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    // draw pixels :
    glPixelZoom(zoom, -zoom);
    glRasterPos3i(sx, sy, 0);
    glDrawPixels(w, h, GL_BGR, GL_UNSIGNED_BYTE, data);
    //glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // restore GL state :
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    }

void Image::loadTexture() const {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }


}

