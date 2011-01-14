/*
     PLIB - A Suite of Portable Game Libraries
     Copyright (C) 1998,2002  Steve Baker
 
     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Library General Public
     License as published by the Free Software Foundation; either
     version 2 of the License, or (at your option) any later version.
 
     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Library General Public License for more details.
 
     You should have received a copy of the GNU Library General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 
     For further information visit http://plib.sourceforge.net

     $Id: ssgaBillboards.cxx,v 1.4 2004/02/25 15:39:34 sjbaker Exp $
*/


#include <math.h>
#include "ssgaBillboards.h"


int ssgaBillboards::total_drawn = 0;


inline float max3f(const float v[3])
{
    float tmp = v[0] >= v[1] ? v[0] : v[1];
    return tmp >= v[2] ? tmp : v[2];
}


inline double random_value()
{
#ifdef UL_WIN32
    return (double) rand() / RAND_MAX;
#else
    return drand48();
#endif
}



ssgaBillboards::ssgaBillboards()
    : array(16)
{
    sgSetVec3(up, 0, 0, 1);

    width     = 1;
    height    = 1;
    clip_dist = -1;
    fade_dist = -1;
    near_dist = -1;
    draw_num  = -1;
    fade_num  = -1;
    alpha_ref = 0.1f;

    ssgSimpleState *ss = new ssgSimpleState;
    ss->disable(GL_LIGHTING);
    ss->disable(GL_BLEND);
    ss->enable(GL_ALPHA_TEST);
    ss->enable(GL_TEXTURE_2D);
    setState(ss);
}



ssgaBillboards::~ssgaBillboards()
{
}



void ssgaBillboards::recalcBSphere()
{
    int i;
    sgBox box;
    box.empty();
    for (i = 0; i < getNum(); i++)
	box.extend(get(i));
    sgAddVec3(bsphere.center, box.min, box.max);
    sgScaleVec3(bsphere.center, 0.5f);
    sgSubVec3(box.max, box.min);
    bsphere.radius = 0.5f * max3f(box.max);
    for (i = 0; i < getNum(); i++) {
	sgSphere sph;
	sgCopyVec3(sph.center, get(i));
	float s = get(i)[3];
	sph.radius = 0.5f * s * width;
	bsphere.extend(&sph);
	sgAddScaledVec3(sph.center, up, s * height);
	bsphere.extend(&sph);
    }
    bsphere_is_invalid = 0;
}


// static
void ssgaBillboards::initTexAlpha(GLuint texture, float athres)
{    
    const GLenum format[] = { GL_NONE, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glBindTexture(GL_TEXTURE_2D, texture);

    for (int level = 0;; level++) {
	GLint width = 0, height = 0, red = 0, alpha = 0;
	int icomp, ocomp;
	
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_WIDTH,          &width );
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_HEIGHT,         &height);       
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_RED_SIZE,       &red   );
	glGetTexLevelParameteriv(GL_TEXTURE_2D, level, GL_TEXTURE_ALPHA_SIZE,     &alpha );

	if (width <= 0 || height <= 0)
	    break;

	if (red > 0) {
	    icomp = 3 + (alpha > 0);
	    ocomp = 4;
	} else {
	    icomp = 1 + (alpha > 0);
	    ocomp = 2;
	}
	
	GLubyte *pixels = new GLubyte [ width * height * ocomp ];

	glGetTexImage(GL_TEXTURE_2D, level, format[icomp], GL_UNSIGNED_BYTE, pixels);
    
	for (int y = height; y--;) {
	    for (int x = width; x--;) {
		float af = icomp % 2 ? 1.0f : (1.0f / 256.0f) * pixels[(y * width + x + 1) * icomp - 1];
		// default alpha_ref is 0.1, so match that:
		af = af > athres ? 0.1f + 0.9f * random_value() : 0;
		int a = (int) (256.0f * af);
		pixels[(y * width + x + 1) * ocomp - 1] = a >= 255 ? 255 : a;
		if (ocomp > icomp)
		    for (int k = 0; k < icomp; k++)
			pixels[(y * width + x) * ocomp + k] = pixels[(y * width + x) * icomp + k];
	    }
	}

	glTexImage2D(GL_TEXTURE_2D, level, format[ocomp], width, height, 0, 
		     format[ocomp], GL_UNSIGNED_BYTE, pixels);
	
	delete [] pixels;

	if (width == 1 && height == 1)
	    break;
    }
}



void ssgaBillboards::cull(sgFrustum *f, sgMat4 m, int test_needed)
{
    int cull_result = cull_test(f, m, test_needed);

    if (cull_result == SSG_OUTSIDE)
	return;

    draw(m);
}


void ssgaBillboards::draw()
{
    sgMat4 m;

    glGetFloatv(GL_MODELVIEW_MATRIX, &m[0][0]);

    draw(m);
}


void ssgaBillboards::draw(const sgMat4 m)
{
    if (!draw_num)
	return;

    sgVec4 z_axis = { m[0][2], m[1][2], m[2][2], m[3][2] };
    float dist = -1;
    
    if (clip_dist >= 0) {
	const sgSphere *s = getBSphere();
	
	float z = sgScalarProductVec3(s->getCenter(), z_axis) + z_axis[3];
	dist = -z - s->getRadius();
	
	if (dist >= clip_dist)
	    return;
    }
    
    if (preDrawCB != NULL && !(*preDrawCB)(this))
	return;


    float aref = alpha_ref;

    if (fade_dist >= 0 && fade_dist < clip_dist && dist > fade_dist)
	aref += (1.0f - alpha_ref) * (dist - fade_dist) / (clip_dist - fade_dist);
    
    
    _ssgCurrentContext->setCullface(0);
    getState()->apply();
    
    glPushAttrib(GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glAlphaFunc(GL_GEQUAL, aref);

    glBegin(GL_QUADS);


    int n = getNum();
    const float *p = get(0);

    if (draw_num > 0 && draw_num < n)
	n = draw_num;

    if (fade_num >= 0 && fade_num < n && clip_dist > near_dist && near_dist > 1e-6f) {
	float far_dist = fade_dist >= 0 && fade_dist < clip_dist ? fade_dist : clip_dist;
	if (dist > near_dist) {
	    if (dist < far_dist) {
		float fn = 1.0f / near_dist;
		float ff = 1.0f / far_dist;
		float fd = 1.0f / dist;
		float s = (fd - fn) / (ff - fn);
		n -= (int) ((n - fade_num) * s + 0.5f);
	    } else {
		n = fade_num;
	    }
	}
    }

    if (up[2] == 1) {

	// fast path when Z is up
	
	float wx, wy, heightz, len;

	wx =  z_axis[1];
	wy = -z_axis[0];
	len = sgHypot(wx, wy);
	
	if (len > 1e-6f) {
	    
	    wx *= 0.5f * width / len;
	    wy *= 0.5f * width / len;
	    heightz = height;

	    for (int i = 0; i < n; i++) {
		float x, y, z, s, dx, dy;

		x = *p++;
		y = *p++;
		z = *p++;
		s = *p++;

		dx = s * wx;
		dy = s * wy;

		glTexCoord2f(0, 0); 
		glVertex3f(x - dx, y - dy, z);

		glTexCoord2f(1, 0);
		glVertex3f(x + dx, y + dy, z);

		z += s * heightz;

		glTexCoord2f(1, 1);
		glVertex3f(x + dx, y + dy, z);

		glTexCoord2f(0, 1);
		glVertex3f(x - dx, y - dy, z);	
	    }

	    total_drawn += n;
	}

    } else {
    
	sgVec3 xy;
	sgVectorProductVec3(xy, z_axis, up);
	float len = sgLengthVec3(xy);

	if (len > 1e-6f) {
	    float wx, wy, wz, heightx, heighty, heightz;

	    wx = 0.5f * width * xy[0] / len;
	    wy = 0.5f * width * xy[1] / len;
	    wz = 0.5f * width * xy[2] / len;

	    heightx = height * up[0];
	    heighty = height * up[1];
	    heightz = height * up[2];
	    
	    for (int i = 0; i < n; i++) {
		float x, y, z, s, dx, dy, dz;

		x = *p++;
		y = *p++;
		z = *p++;
		s = *p++;

		dx = s * wx;
		dy = s * wy;
		dz = s * wz;

		glTexCoord2f(0, 0); 
		glVertex3f(x - dx, y - dy, z - dz);

		glTexCoord2f(1, 0);
		glVertex3f(x + dx, y + dy, z + dz);

		x += s * heightx;
		y += s * heighty;
		z += s * heightz;

		glTexCoord2f(1, 1);
		glVertex3f(x + dx, y + dy, z + dz);

		glTexCoord2f(0, 1);
		glVertex3f(x - dx, y - dy, z - dz);	
	    }

	    total_drawn += n;
	}
    }


    glEnd();
    
    glPopAttrib();

    if (postDrawCB != NULL)
	(*postDrawCB)(this);
}



float *ssgaBillboards::getVertex(int i)
{
    if (i < 0 || i >= 4 * getNum())
	return _ssgVertex000;

    // pick any orientation
    sgVec3 xy, axis = { 0, 0, 0 };
    axis[ sgAbs(up[0]) > sgAbs(up[1]) ] = 1;
    sgVectorProductVec3(xy, up, axis);
    sgNormaliseVec3(xy);

    const sgVec3 tab[4] = { {-0.5f,0.0f}, {0.5f,0.0f}, {0.5f,1.0f}, {-0.5f,1.0f} };
    const float *p = array.get(i / 4);
    const float *d = tab[i % 4];
    static sgVec3 v;
    for (i = 0; i < 3; i++)
	v[0] = p[i] + p[3] * (width * xy[i] * d[0] + height * up[i] * d[1]);
    return v;
}


float *ssgaBillboards::getTexCoord(int i)
{
    static sgVec2 t[4] = { {0,0}, {1,0}, {1,1}, {0,1} };
    return t[i & 3];
}


void ssgaBillboards::getTriangle(int n, short *v1, short *v2, short *v3)
{
    *v1 = 2*n + 0;
    *v2 = 2*n + 1;
    *v3 = 2*n + 2 - 4*(n&1);
}


void ssgaBillboards::transform(const sgMat4 m)
{
    float scale = sgLengthVec3(m[0]);
    for (int i = 0; i < getNum(); i++)
	sgXformPnt3(array.get(i), array.get(i), m);
    sgXformVec3(up, up, m);
    sgNormaliseVec3(up);
    width *= scale;
    height *= scale;
}



/*
  Local Variables:
  mode: C++
  c-basic-offset: 4
  c-file-offsets: ((substatement-open 0) (case-label 0))
  End:
*/
