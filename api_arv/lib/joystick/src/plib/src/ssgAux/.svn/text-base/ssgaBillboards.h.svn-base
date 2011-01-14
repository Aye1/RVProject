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

     $Id: ssgaBillboards.h,v 1.2 2004/01/21 16:30:20 stromberg Exp $
*/

/* 
 * ssgaBillboards
 *
 * Each billboard is represented by four values: X, Y, Z and scale.
 * They are drawn as GL_QUADS using the texture defined in the state.
 *
 * Set the basic billboard size (each can be individually scaled):
 *
 *   bb->setWidth(3.0f);
 *   bb->setHeight(8.0f);
 *
 * Set a texture image:
 *
 *   bb->initTexAlpha(texture, 0.3f); // only once per texture, choose a suitable alpha threshold
 *   bb->setTexture(texture);
 *
 * Instantiate some billboards:
 *
 *   for (i = 0; i < 1000; i++)
 *     bb->add( (drand48() - 0.5) * WORLD_SIZE_X,
 *              (drand48() - 0.5) * WORLD_SIZE_Y,
 *              Z, 0.75 + 0.5 * drand48() );
 *
 *
 * The billboards can be faded:
 *
 *   bb->setClipRange(300.0f);  // i.e. far clip
 *   bb->setFadeRange(200.0f);  // starting fade here
 * 
 * or, equivalently:
 *
 *   bb->setFarRange(300.0f, 200.0f);  // clip, fade
 *
 *
 * The number of billboards drawn can be a function of distance:
 *
 *   bb->setNearRange(50.0f);  // draw all at this distance
 *   bb->setFadeNum(100);      // number drawn at the fade distance (or clip distance if unset)
 *
 * Limit the number at the near range (defaults to all):
 *
 *   bb->setDrawNum(500);
 *
 * 
 * Example code: plib/examples/src/ssg/shrubs/shrubs.cxx
 *
 *
 * November 2003, Marten Stromberg
 *
 */


#include "ssg.h"


class ssgaBillboards : public ssgLeaf
{
protected:

    sgVec3 up;              // vector pointing up (default: 0, 0, 1)
    ssgColourArray array;   // position and scale for each billboard
    int draw_num;           // only draw this many (-1 for all)
    float width, height;    // basic size of billboard    
    float clip_dist;        // far clip distance (-1 disables)
    float fade_dist;        // fade-out distance (fade iff fade_dist < d < clip_dist)
    float near_dist;        // near distance for adjusting number of trees
    int fade_num;           // optionally adjust number of trees as a function of distance
    float alpha_ref;        // default 0.1 (initTexAlpha relies on that)

public:
    
    ssgaBillboards();
    ~ssgaBillboards();
    
    void setUp(const sgVec3 v)            { sgNormaliseVec3(up, v); }
    void setUp(float x, float y, float z) { sgVec3 v = { x, y, z }; setUp(v); }
    const float *getUp() const            { return up; }
    
    int getNum()             { return array.getNum(); }  // const
    const float *get(int i)  { return array.get((unsigned) i); }  // const
    
    inline void add(float x, float y, float z, float scale = 1.0f);
    inline void add(const sgVec4 xyzs);
    
    void removeLast()  { array.removeLast(); dirtyBSphere(); }
    void removeAll()   { array.removeAll();  dirtyBSphere(); }
    
    void  setWidth(float w)   { width = w;     }
    float getWidth() const    { return width;  }
    void  setHeight(float h)  { height = h;    }
    float getHeight() const   { return height; }
    
    void  setClipRange(float d) { clip_dist = d;    }
    float getClipRange() const  { return clip_dist; }
    void  setFadeRange(float d) { fade_dist = d;    }
    float getFadeRange() const  { return fade_dist; }
    void  setNearRange(float d) { near_dist = d;    }
    float getNearRange() const  { return near_dist; }

    void setFarRange(float clip, float fade)                  { setFadeRange(fade); setClipRange(clip);       }
    void setFarRange(float clip)                              { setFarRange(clip, 0.75f * clip);              }
    void setNearFarRange(float near_, float clip, float fade) { setNearRange(near_); setFarRange(clip, fade); }
    void setNearFarRange(float near_, float clip)             { setNearRange(near_); setFarRange(clip);       }
    
    void setDrawNum(int n)   { draw_num = n;    }
    int  getDrawNum() const  { return draw_num; }
    void setFadeNum(int n)   { fade_num = n;    }
    int  getFadeNum() const  { return fade_num; }
    
    inline void setTexture(ssgTexture *tex);
    inline ssgTexture *getTexture();  // const
    
    void  setAlphaRef(float aref) { alpha_ref = aref; }  // only when not using initTexAlpha
    float getAlphaRef() const     { return alpha_ref; }
    
    static void initTexAlpha(GLuint handle, float aref = 0.3f); // initialize alpha channel for smooth fading
    static void initTexAlpha(ssgTexture *tex, float aref = 0.3f) { initTexAlpha(tex->getHandle(), aref); }


    /* methods derived from ssgEntity and ssgLeaf */

    virtual void recalcBSphere();

    virtual void cull(sgFrustum *, sgMat4, int);
    virtual void draw();

    void draw(const sgMat4 m);

    virtual void draw_geometry() {}
    virtual void pick(int) {}

    virtual void drawHighlight(sgVec4) {}
    virtual void drawHighlight(sgVec4, int) {}

    virtual void isect_triangles(sgSphere *, sgMat4, int) {}
    virtual void hot_triangles(sgVec3, sgMat4, int) {}
    virtual void los_triangles(sgVec3, sgMat4, int) {}
    
    virtual const char *getTypeName() { return "ssgaBillboards"; }
    
    virtual GLenum getPrimitiveType() { return GL_QUADS; }
    
    virtual int getNumVertices()  { return 4 * getNum(); }
    virtual int getNumTexCoords() { return 4 * getNum(); }
    virtual int getNumNormals()   { return 0; }
    virtual int getNumColours()   { return 0; }
    
    virtual float *getVertex(int i);
    virtual float *getTexCoord(int i);
    virtual float *getNormal(int) { return _ssgNormalUp;    }
    virtual float *getColour(int) { return _ssgColourWhite; }
    
    virtual int  getNumTriangles() { return 2 * getNum(); }
    virtual void getTriangle(int n, short *v1, short *v2, short *v3);
    virtual int  getNumLines() { return 0; }
    virtual void getLine(int, short *v1, short *v2) {}
    
    virtual void transform(const sgMat4 m);


    // incremented each draw, use it as you like
    static int total_drawn;

};



inline void ssgaBillboards::add(float x, float y, float z, float w)
{
    array.add(x, y, z, w);
    dirtyBSphere(); 
}


inline void ssgaBillboards::add(const sgVec4 xyzs)
{ 
    array.add((float *) xyzs); 
    dirtyBSphere(); 
}

    
inline void ssgaBillboards::setTexture(ssgTexture *tex)
{
    ssgSimpleState *ss = (ssgSimpleState *) getState();
    assert( ss && ss->isAKindOf( ssgTypeSimpleState() ));
    ss->setTexture(tex);
}


inline ssgTexture *ssgaBillboards::getTexture()
{
    ssgSimpleState *ss = (ssgSimpleState *) getState();
    assert( ss && ss->isAKindOf( ssgTypeSimpleState() ));
    return ss->getTexture();
}

 
/*
  Local Variables:
  mode: C++
  c-basic-offset: 4
  c-file-offsets: ((substatement-open 0) (case-label 0))
  End:
*/
