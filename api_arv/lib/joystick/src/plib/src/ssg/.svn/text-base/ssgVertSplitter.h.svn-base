////////////////////////////////////////////////////////////////////////
//
// Copyright 2003, Andrew Ross
//
// Walk through an indexed triangle list, splitting vertices that
// share "sharp" edges, and calculate normals for the resulting mesh.
//
// This code should really live inside the OptVertexList class in
// ssgOptimizer.cxx, whose data model is almost 1:1 compatible with
// the indexed triangle representation here.  It is a separate file
// because it comes from separate code that Andy Ross wrote for a
// different project, and this was the cleanest way to do the port.
//
////////////////////////////////////////////////////////////////////////

#ifndef _SSGA_VERTSPLITTER_H
#define _SSGA_VERTSPLITTER_H

class ssgVertSplitter {
public:
    ssgVertSplitter(int nVerts, int nTris);
    virtual ~ssgVertSplitter();
    void setSharpAngle(float deg);

    float* vert(int i) { return _verts + 3*i; }
    float* norm(int i) { return _norms + 3*i; }

    // Initialize a triangle.  The vertex indices might change.
    void setTri(int tidx, int va, int vb, int vc);

    // Grab the new/different indices post-split
    int* getTri(int t);

    // Number of new vertices (indices start after the original list)
    int newVerts() { return _nextNewVert; }

    // Original vertex associated with a given "new" vertex
    int origVert(int i);

    void splitAndCalcNormals();

private:
    struct Tri {
        int verts[3];
        int oVerts[3];
        bool degenerate;
    };

    void condenseGeometry();
    void expandDuplicates();

    int findTriWithVert(int fidx, int vidx, int* tris, int ntris);
    int nextTri(int fidx, int vidx, int* tris, int ntris);
    int prevTri(int fidx, int vidx, int* tris, int ntris);
    static void fixVidx(Tri* t, int oldIdx, int newIdx);

    float _threshold;

    int _origVerts;
    int _vertsAlloced;

    int _nVerts;
    float* _verts;
    float* _norms;

    int _nTris;
    Tri* _tris;
    float* _triNorms;

    int* _newVertMap;
    int _nextNewVert;

    int* _geomVerts;

    void DUMP();
};

#endif // _SSGA_VERTSPLITTER_H
