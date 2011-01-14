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

#include "ssgVertSplitter.h"

#include <math.h>
#include "sg.h"

//
////////////////////////////////////////////////////////////////////////

int ssgVertSplitter::origVert(int i)
{
    return _newVertMap[i];
}

ssgVertSplitter::ssgVertSplitter(int nVerts, int nTris)
{
    setSharpAngle(46);
    _nVerts = _origVerts = _vertsAlloced = nVerts;
    _verts = new float[3*_nVerts];
    _norms = new float[3*_nVerts];
    _nTris = nTris;
    _tris = new Tri[3*_nTris];
    _triNorms = new float[3*_nTris];
    _geomVerts = new int[_nVerts];
}

ssgVertSplitter::~ssgVertSplitter()
{
    delete[] _verts;
    delete[] _norms;
    delete[] _tris;
    delete[] _triNorms;
}

void ssgVertSplitter::setTri(int tidx, int va, int vb, int vc)
{
    _tris[tidx].verts[0] = _tris[tidx].oVerts[0] = va;
    _tris[tidx].verts[1] = _tris[tidx].oVerts[1] = vb;
    _tris[tidx].verts[2] = _tris[tidx].oVerts[2] = vc;
    _tris[tidx].degenerate = false;
}

int* ssgVertSplitter::getTri(int t)
{
    return &(_tris[t].verts[0]);
}

void ssgVertSplitter::setSharpAngle(float deg)
{ 
    _threshold = cos(deg*SG_DEGREES_TO_RADIANS);
}

int ssgVertSplitter::findTriWithVert(int fidx, int vidx, int* tris, int ntris)
{
    // check each triangle in the list for a matching edge
    for(int t=0; t<ntris; t++) {
        if(t == fidx) continue;     // not a neighbor to ourselves
        if(tris[t] == -1) continue; // no longer available
        for(int i=0; i<3; i++)
            if(_tris[tris[t]].verts[i] == vidx)
                return t;
    }
    return -1;
}

int ssgVertSplitter::nextTri(int fidx, int vidx, int* tris, int ntris)
{
    if(tris[fidx] == -1) return -1;

    // Adjoining triangles share an edge
    int* verts = _tris[tris[fidx]].verts, vertnum;
    for(vertnum=0; vertnum<3; vertnum++) if(verts[vertnum] == vidx) break;
    int nextvert = _tris[tris[fidx]].verts[vertnum==2 ? 0 : vertnum+1];
    return findTriWithVert(fidx, nextvert, tris, ntris);
}

int ssgVertSplitter::prevTri(int fidx, int vidx, int* tris, int ntris)
{
    if(tris[fidx] == -1) return -1;
    int* verts = _tris[tris[fidx]].verts, vertnum;
    for(vertnum=0; vertnum<3; vertnum++) if(verts[vertnum] == vidx) break;
    int prevvert = _tris[tris[fidx]].verts[vertnum==0 ? 2 : vertnum-1];
    return findTriWithVert(fidx, prevvert, tris, ntris);
}

void ssgVertSplitter::fixVidx(Tri* t, int oldIdx, int newIdx)
{
    for(int i=0; i<3; i++) {
        if(t->verts[i] == oldIdx) {
            t->verts[i] = newIdx;
            return;
        }
    }
}

// Find geometrically identical vertices and fixup triangle indices to
// point to the first one.
void ssgVertSplitter::condenseGeometry()
{
    for(int i=0; i<_nVerts; i++) {
        _geomVerts[i] = i;
        for(int j=0; j<i; j++) {
            if(sgEqualVec3(vert(i), vert(j))) {
                _geomVerts[i] = j;
                for(int t=0; t<_nTris; t++) {
                    if(_tris[t].verts[0] == i) _tris[t].verts[0] = j;
                    if(_tris[t].verts[1] == i) _tris[t].verts[1] = j;
                    if(_tris[t].verts[2] == i) _tris[t].verts[2] = j;
                }
                break;
            }
        }
    }
}

void ssgVertSplitter::expandDuplicates()
{
    int i, j, k;

    // "split" vertex, original vertex, final vertex
    struct vrec { int sv; int ov; int fv; };
    vrec* vrecs = new vrec[_vertsAlloced];
    int nvrecs = 0;

    // Tick off each vertex index as it gets finalized to a
    // input/post-split vertex tuple.
    bool *used = new bool[_vertsAlloced];
    for(i=0; i<_vertsAlloced; i++) used[i] = false;

    for(i=0; i<_nTris; i++) {
        for(j=0; j<3; j++) {
            int sv = _tris[i].verts[j];
            int ov = _tris[i].oVerts[j];
            int fv = -1;
            for(k=0; k<nvrecs; k++)
                if(vrecs[k].sv == sv && vrecs[k].ov == ov)
                    fv = _tris[i].verts[j] = vrecs[k].fv;
            if(fv >= 0) continue;

            // No existing record, make one.
            if(!used[sv] && sv >= _origVerts) fv = sv;
            else if(!used[ov]) fv = ov;
            else { fv = _nVerts++; _nextNewVert++; }

            int r = nvrecs++;
            vrecs[r].sv = sv;
            vrecs[r].ov = ov;
            vrecs[r].fv = fv;
            // __builtin_printf("s %d o %d f %d\n", sv, ov, fv);

            sgCopyVec3(vert(fv), vert(sv));
            sgCopyVec3(norm(fv), norm(sv));
            if(fv >= _origVerts)
                _newVertMap[fv - _origVerts] = ov;
            used[fv] = true;
            _tris[i].verts[j] = fv;
        }
    }
    delete[] vrecs;
    delete[] used;
}

void ssgVertSplitter::splitAndCalcNormals()
{
    int i, j;
    float zero[] = {0,0,0};
    for(i=0; i<_nVerts; i++) sgCopyVec3(norm(i), zero);

    // First, find duplicate vertices and coalesce the geometry
    condenseGeometry();

    // Calculate a normal for each face, and count the number of
    // references to each vertex.
    float* faceNorms = _triNorms;
    int* vCounts = new int[_nVerts];
    for(i=0; i<_nVerts; i++)
        vCounts[i] = 0;
    for(i=0; i<_nTris; i++) {
        sgVec3 a, b;
        float *fnorm = faceNorms + 3*i;
        int* verts = _tris[i].verts ;
        sgCopyVec3(a, vert(verts[1]));
        sgSubVec3(a, vert(verts[0]));
        sgCopyVec3(b, vert(verts[2]));
        sgSubVec3(b, vert(verts[0]));
        sgVectorProductVec3(fnorm, a, b);
        sgNormaliseVec3(fnorm);
        vCounts[verts[0]]++;
        vCounts[verts[1]]++;
        vCounts[verts[2]]++;

        if(verts[0]==verts[1] || verts[1]==verts[2] || verts[2]==verts[0])
        {
            _tris[i].degenerate = true;
            fnorm[0] = fnorm[1] = 0;
            fnorm[2] = 1;
        }
    }

    // Build a table mapping vertices to arrays of triangle indices
    // vTris[v] is a pointer to a list of vCounts[v] triangle indices.
    int* vtspace = new int[3*_nTris];
    int** vTris = new int*[_nVerts];
    int* vtptr = vtspace;
    for(i=0; i<3*_nTris; i++) vtspace[i] = -1;
    for(i=0; i<_nVerts; i++) {
        vTris[i] = vtptr;
        vtptr += vCounts[i];
    }
    int *vctmp = new int[_nVerts];
    for(i=0; i<_nVerts; i++) vctmp[i] = 0;
    for(i=0; i<_nTris; i++) {
        if(_tris[i].degenerate) continue;
        for(int v=0; v<3; v++)
            vTris[_tris[i].verts[v]][vctmp[_tris[i].verts[v]]++] = i;
    }
    delete[] vctmp;

    // Make some temp space for new vertices and their normals
    int maxNewVerts = 8*_nVerts;
    _newVertMap = new int[maxNewVerts];
    float* newNorms = new float[3*maxNewVerts];
    _nextNewVert = 0;

    // Now the hard part.  For each vertex, pick a face.  Walk forward
    // and backwards from that face until you hit a sharp edge or
    // already-done face, summing normals along the way and marking
    // off the face as "done".  If there are faces left, split off a
    // *new* vertex for the remaining ones, pick a remaining face, and
    // repeat.  Repeat until we run out of faces.
    for(int vidx=0; vidx<_nVerts; vidx++) {
        while(1) {
            // Pick a face
            int fidx = -1;
            bool someDone = false;
            for(int face=0; face<vCounts[vidx]; face++)
                if(vTris[vidx][face] != -1) fidx = face;
                else someDone = true;
            if(fidx == -1) break;

            float* fnorm = norm(vidx);

            // Oops, some faces already stole the previous vertex index.
            // Need to make a *new* vertex and replace the index on the
            // remaining triangles.
            int realvert = vidx;
            if(someDone) {
                realvert = _nVerts + _nextNewVert;
                fnorm = newNorms + 3*_nextNewVert;
                _newVertMap[_nextNewVert++] = vidx;
            }

            for(i=0; i<3; i++) fnorm[i] = 0;
            int nextidx = nextTri(fidx, vidx, vTris[vidx], vCounts[vidx]);
            int previdx = prevTri(fidx, vidx, vTris[vidx], vCounts[vidx]);
            sgAddVec3(fnorm, faceNorms + 3*vTris[vidx][fidx]);
            float* lastNorm0 = faceNorms + 3*vTris[vidx][fidx];
            float* lastNorm = lastNorm0;
            if(someDone)
                fixVidx(_tris + vTris[vidx][fidx], vidx, realvert);
            vTris[vidx][fidx] = -1;
            while(nextidx >= 0) {
                float* n = faceNorms + 3*vTris[vidx][nextidx];
                if(sgScalarProductVec3(n, lastNorm) < _threshold) break;
                lastNorm = n;
                sgAddVec3(fnorm, n);
                int oldnext = nextidx;
                nextidx = nextTri(nextidx, vidx, vTris[vidx], vCounts[vidx]);
                if(someDone)
                    fixVidx(_tris + vTris[vidx][oldnext], vidx, realvert);
                vTris[vidx][oldnext] = -1;
            }
            lastNorm = lastNorm0;
            while(previdx >= 0) {
                if(vTris[vidx][previdx] < 0) break;
                float* n = faceNorms + 3*vTris[vidx][previdx];
                if(sgScalarProductVec3(n, lastNorm) < _threshold) break;
                lastNorm = n;
                sgAddVec3(fnorm, faceNorms + 3*vTris[vidx][previdx]);
                int oldprev = previdx;
                previdx = prevTri(previdx, vidx, vTris[vidx], vCounts[vidx]);
                if(someDone)
                    fixVidx(_tris + vTris[vidx][oldprev], vidx, realvert);
                vTris[vidx][oldprev] = -1;
            }
        }
    };

    // Reallocate the new vertex/normal tables.
    _vertsAlloced = _nVerts + maxNewVerts;
    float* newVerts = new float[3*_vertsAlloced];
    for(i=0; i<3*_nVerts; i++)
        newVerts[i] = _verts[i];
    for(i=0; i<_nextNewVert; i++)
        for(j=0; j<3; j++)
            newVerts[3*(_nVerts+i)+j] = _verts[3*_newVertMap[i]+j];
    delete[] _verts;
    _verts = newVerts;

    float* tmpnorms = new float[3*_vertsAlloced];
    for(i=0; i<3*_nVerts; i++)
        tmpnorms[i] = _norms[i];
    for(i=0; i<3*_nextNewVert; i++)
        tmpnorms[3*_nVerts+i] = newNorms[i];
    delete[] _norms;
    _norms = tmpnorms;

    _nVerts += _nextNewVert;

    // Finally, renormalize everything
    for(i=0; i<_nVerts; i++)
        if(i >= _origVerts || _geomVerts[i] == i) sgNormaliseVec3(norm(i));
    for(i=0; i<_origVerts; i++)
        if(_geomVerts[i] != i) sgCopyVec3(norm(i), norm(_geomVerts[i]));

    // Now, if necessary, duplicate the vertices that got "condensed"
    // earlier by creating a new vertex and cloning the normal.
    expandDuplicates();

#if 0
    static int allOrig = 0;
    static int allFinal = 0;
    allOrig += _origVerts;
    allFinal += _nVerts;
    fprintf(stderr, "%d -> %d (%d/%d)\n", _origVerts, _nVerts,
            allOrig, allFinal);
#endif
    
    delete[] vCounts;
    delete[] vtspace;
    delete[] vTris;
    delete[] newNorms;
}
