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

     $Id: ssgLoadXPlaneObj.cxx,v 1.3 2002/11/09 21:38:28 sjbaker Exp $
*/

/*
 * XPlane object loader for PLIB.
 * Written by Bernie Bright <bbright@bigpond.net.au>, Sept 2002.
 *
 * KNOWN ISSUES
 *
 * Only version 1 objects are handled.
 *
 * The .obj XPlane custom object file extension clashes with Wavefront objects.
 * For the moment, XPlane objects must be renamed with a .xpl extension.
 *
 * Point objects are not handled.
 */

#include <stdio.h>
#include <string.h>

#include "ssgLocal.h"

/**
 * XPlane .obj loader.
 */
class ObjLoader
{
public:
    ObjLoader( const ssgLoaderOptions* options );

    ~ObjLoader();

    ssgEntity* load( const char* fname );

private:
    ssgState* get_state( const char* tfname );
    char* fgets( char* s, int size, FILE* fp );
    char* getline( char* line, size_t n, FILE* fp );
    char* getTextureName( const char* p, char* tname, size_t n );
    const char* file_extension( const char* fname );

private:
    ssgLoaderOptions* options;
    ssgBranch* root;
};

ObjLoader::ObjLoader( const ssgLoaderOptions* options_ )
    : options(0)
    , root(0)
{
    ssgSetCurrentOptions( const_cast<ssgLoaderOptions*>(options_) );
    this->options = ssgGetCurrentOptions();
}

ObjLoader::~ObjLoader()
{
}

ssgState*
ObjLoader::get_state( const char* tfname )
{
    if (tfname != 0)
    {
	ssgState* st = options->createState( const_cast<char*>(tfname) );
	if (st != 0)
	    return st;
    }

    ssgSimpleState* st = new ssgSimpleState;
    st->setName( tfname );
    st->setMaterial( GL_SPECULAR, 0.0f, 0.0f, 0.0f );
    st->setMaterial( GL_EMISSION, 0.0f, 0.0f, 0.0f );
    st->setMaterial( GL_AMBIENT, 0.0f, 0.0f, 0.0f );
    st->setMaterial( GL_DIFFUSE, 0.0f, 0.0f, 0.0f );
    st->enable( GL_LIGHTING );
    st->setShadeModel( GL_SMOOTH );

    bool has_alpha = false;
    if (tfname != 0)
    {
	ssgTexture* tex = options->createTexture( const_cast<char*>(tfname) );
	has_alpha = tex->hasAlpha();
	st->setTexture(tex);
	st->enable( GL_TEXTURE_2D );
    }
    else
    {
	st->disable( GL_TEXTURE_2D );
    }

    if (has_alpha)
    {
	st->disable( GL_ALPHA_TEST );
	st->enable( GL_BLEND );
	st->setTranslucent();
    }
    else
    {
	st->disable( GL_ALPHA_TEST );
	st->disable( GL_BLEND );
	st->setOpaque();
    }

    return st;
}

/**
 * XPlane object files have either IBM (\r\n) or Apple (\r) line endings.
 * Handle both here.
 */
char*
ObjLoader::fgets( char* s, int size, FILE* fp )
{
    char* p = s;
    int n = 0;
    int c = 0;

    for (--size; n < size; ++n)
    {
	c = getc(fp);
	if (c == EOF)
	    return 0;

	if (c == '\r')
	    break;
	*p++ = c;
    }

    *p = 0;

    c = getc(fp);
    if (c != '\n')
	ungetc( c, fp );

    return s;
}

/**
 * Read the next non-blank line.
 * Returns pointer to first non-whitespace character or NULL on error or EOF.
 */
char*
ObjLoader::getline( char* line, size_t n, FILE* fp )
{
    for (;;)
    {
	char* p = fgets( line, n, fp );
	if (p == 0)
	    return 0;

	// Skip leading whitespace.
	while (*p != 0 && isspace(*p))
	    ++p;

	if (*p != 0)
	    return p;
    }
}

/**
 * 
 */
char*
ObjLoader::getTextureName( const char* p, char* buf, size_t bufsize )
{
    if (p == 0 || buf == 0)
	return 0;

    while (*p != 0 && isspace(*p))
	++p;

    size_t n = 0;
    char* bp = buf;
    for ( ; *p != 0 && n < bufsize && !isspace(*p); ++n, ++bp, ++p)
    {
	// Replace ':' path separator with '/'.
	*bp = *p == ':' ? '/' : *p;
    }

    if (n < bufsize)
	*bp = 0;

    return const_cast<char*>(p);
}

const char*
ObjLoader::file_extension( const char *fname )
{
    const char* p = fname + strlen(fname);

    while (p != fname && *p != '/' && *p != '.' )
	--p;

    return p;
}

ssgEntity*
ObjLoader::load( const char* fname )
{
    char mname[1024];
    options->makeModelPath( mname, fname );

    FILE* fp = fopen( fname, "r" );
    if (fp == 0)
    {
	ulSetError( UL_WARNING,
		    "ssgLoadXPlaneObj: Failed to open '%s' for reading.",
		    fname );
	return 0;
    }

    char line[ 1024 ];

    // Read obj header.
    char* p = getline( line, sizeof(line), fp );
    if (p == 0)
    {
	ulSetError( UL_WARNING, "ssgLoadXPlaneObj: expected header." );
	fclose(fp);
	return 0;
    }

    if (*p != 'I' && *p != 'A')
    {
	ulSetError( UL_WARNING,
		    "ssgLoadXPlaneObj: invalid header, expected 'I' or 'A'" );
	fclose(fp);
	return 0;
    }
    
    p = getline( line, sizeof(line), fp );
    if (p == 0)
    {
	ulSetError( UL_WARNING,
		    "ssgLoadXPlaneObj: unexpected EOF while reading header." );
	fclose(fp);
	return 0;
    }

    root = new ssgTransform();
    root->setName( fname );

    if (*p == '2')
    {
	// Version 2 obj file.
	ulSetError( UL_WARNING,
		    "ssgLoadXPlaneObj: Version 2 not yet implemented" );

	fclose( fp );
	delete root;
	return 0;

	// TODO: implement V2.
    }
    else
    {
	// Version 1 obj file.

	// Version number is optional.
	if (*p == '1')
	{
	    // get TextureDef
	    p = getline( line, sizeof(line), fp );
	    if (p == 0)
	    {
		ulSetError( UL_WARNING,
			    "ssgLoadXPlaneObj: unexpected EOF "
			    "while reading data." );
		fclose(fp);
		return 0;
	    }
	}

	int type = 0;
	char* endp;
	do
	{
 	    type = strtol( p, &endp, 10 );
	    switch (type)
	    {
	    case 4:
		{
		    // Get texture filename.
		    // Allow room for extension and terminator.
		    char tname[ 251 + 4 + 1 ];
		    endp = getTextureName( endp, tname, 251 );
		    if (endp == 0)
		    {
			ulSetError( UL_WARNING,
				    "ssgLoadXPlaneObj: Error reading TextureDef" );
			delete root;
			fclose( fp );
			return 0;
		    }

		    strcat( tname, ".bmp" );

		    // TODO: check for comment following texture name.

		    sgVec3* vc = new sgVec3[4];
		    sgVec2* tc = new sgVec2[4];
		    sgVec4* cc = new sgVec4[1];

		    for (int i = 0; i < 4; ++i)
		    {
			p = getline( line, sizeof(line), fp );
			// Read vertices, swapping y and z.
			sscanf( p, "%f%f%f", &vc[i][0], &vc[i][2], &vc[i][1] );
		    }

		    sgSetVec2( tc[0], 1.0f, 1.0f );
		    sgSetVec2( tc[1], 1.0f, 0.0f );
		    sgSetVec2( tc[2], 0.0f, 0.0f );
		    sgSetVec2( tc[3], 0.0f, 1.0f );

		    sgSetVec4( cc[0], 1.0f, 1.0f, 1.0f, 1.0f );

		    ssgVertexArray* vl   = new ssgVertexArray( 4, vc );
		    ssgTexCoordArray* tl = new ssgTexCoordArray( 4, tc );
		    ssgNormalArray* nl   = new ssgNormalArray();
		    ssgColourArray* cl   = new ssgColourArray( 1, cc );

		    ssgVtxTable* vtab = new ssgVtxTable( GL_POLYGON,
							 vl, nl, tl, cl );
		    vtab->setState( get_state( tname ) );
		    root->addKid( vtab );
		}
		break;

	    case 3:
		{
		    // Get texture filename.
		    // Allow room for extension and terminator.
		    char tname[ 251 + 4 + 1 ];
		    endp = getTextureName( endp, tname, 251 );
		    strcat( tname, ".bmp" );

		    sgVec3* vc = new sgVec3[3];
		    sgVec2* tc = new sgVec2[3];
		    sgVec4* cc = new sgVec4[1];

		    for (int i = 0; i < 3; ++i)
		    {
			p = getline( line, sizeof(line), fp );
			// Read vertices, swapping y and z.
			sscanf( p, "%f%f%f", &vc[i][0], &vc[i][2], &vc[i][1] );
		    }

		    sgSetVec2( tc[0], 1.0f, 1.0f );
		    sgSetVec2( tc[1], 1.0f, 0.0f );
		    sgSetVec2( tc[2], 0.0f, 0.0f );

		    sgSetVec4( cc[0], 1.0f, 1.0f, 1.0f, 1.0f );

		    ssgVertexArray* vl   = new ssgVertexArray( 3, vc );
		    ssgTexCoordArray* tl = new ssgTexCoordArray( 3, tc );
		    ssgNormalArray* nl   = new ssgNormalArray();
		    ssgColourArray* cl   = new ssgColourArray( 1, cc );

		    ssgVtxTable* vtab = new ssgVtxTable( GL_POLYGON,
							 vl, nl, tl, cl );
		    vtab->setState( get_state( tname ) );
		    root->addKid( vtab );
		}
		break;

	    case 2:
		{
		    float r, g, b;
		    sscanf( endp, "%f%f%f", &r, &g, &b );

		    sgVec3* vc = new sgVec3[2];
		    sgVec4* cc = new sgVec4[1];
		    for (int i = 0; i < 2; ++i)
		    {
			p = getline( line, sizeof(line), fp );
			// Read vertices, swapping y and z.
			sscanf( p, "%f%f%f", &vc[i][0], &vc[i][2], &vc[i][1] );
		    }

		    // Scale rgb values to range 0..1
		    sgSetVec4( cc[0], r/10.f, g/10.f, b/10.f, 1.0f );

		    ssgVertexArray* vl   = new ssgVertexArray( 2, vc );
		    ssgTexCoordArray* tl = new ssgTexCoordArray();
		    ssgNormalArray* nl   = new ssgNormalArray();
		    ssgColourArray* cl   = new ssgColourArray( 1, cc );

		    ssgVtxTable* vtab = new ssgVtxTable( GL_LINES,
							 vl, nl, tl, cl );
		    ssgSimpleState* st = new ssgSimpleState;
		    st->setMaterial( GL_SPECULAR, 0.0f, 0.0f, 0.0f );
		    st->setMaterial( GL_EMISSION, 0.0f, 0.0f, 0.0f );
		    st->setMaterial( GL_AMBIENT, 0.0f, 0.0f, 0.0f );
		    st->setMaterial( GL_DIFFUSE, 0.0f, 0.0f, 0.0f );
		    st->enable( GL_LIGHTING );
		    st->setShadeModel( GL_SMOOTH );
		    st->disable( GL_TEXTURE_2D );
		    st->disable( GL_ALPHA_TEST );
		    st->disable( GL_BLEND );
		    st->setOpaque();
		    vtab->setState( st );
		    root->addKid( vtab );
		}
		    break;

	    case 1:
		{
		    int r, g, b;
		    sscanf( endp, "%d%d%d", &r, &g, &b );
		    p = getline( line, sizeof(line), fp );
		    float x, y, z;
		    sscanf( p, "%f%f%f", &x, &z, &y );

		    // TODO: create appropriate ssgEntity.
		}
		break;

	    case 99:
		break;

	    default:
		ulSetError( UL_WARNING,
			    "ssgLoadXPlaneObj: invalid line, '%s'", line );
		fclose(fp);
		delete root;
		return 0;
	    }
	}
	while ((p = getline( line, sizeof(line), fp )) != 0 && type != 99);
    }

    fclose( fp );
    return root;
}

ssgEntity*
ssgLoadXPlaneOBJ( const char* fname, const ssgLoaderOptions* options )
{
    ObjLoader loader( options );
    return loader.load( fname );
}

