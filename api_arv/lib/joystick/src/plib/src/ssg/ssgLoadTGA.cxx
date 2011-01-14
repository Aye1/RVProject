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

     $Id: ssgLoadTGA.cxx,v 1.10 2003/01/30 00:51:59 stromberg Exp $
*/


#include "ssgLocal.h"

#ifdef SSG_LOAD_TGA_SUPPORTED

//#include <sys/stat.h>

/*
 * Submitted by Sam Stickland : sam@spacething.org
 * Targe loading code based on code written Dave Gay : f00Dave@bigfoot.com, http://personal.nbnet.nb.ca/daveg/
 * 
 * Jan 29, 2003:
 *   Added support for other image types, as well as run-length encoding.
 *   /marten (stromberg@users.sf.net)
 */


inline int get16u(const GLubyte *ptr) // little endian
{
    return (ptr[0] | (ptr[1] << 8));
}


bool ssgLoadTGA ( const char *fname, ssgTextureInfo* info )
{

#if 1 // new loader:

    
    /* TGA file layout:
     *
     * =============================
     * Header:
     *  Ofs  Sz  Desc
     * ----------------
     *   0   1   Image ID Length
     *   1   1   Colormap Type
     *   2   1   Image Type
     *   3   2   Colormap Origin
     *   5   2   Colormap Length
     *   7   1   Colormap Entry Size
     *   8   2   X Origin of Image
     *  10   2   Y Origin of Image
     *  12   2   Image Width
     *  14   2   Image Height
     *  16   1   Image Pixel Size
     *  17   1   Image Descriptor
     * =============================
     *  Image Identification
     * =============================
     *  Colormap Data
     * =============================
     *  Image Data
     * =============================
     */


    GLubyte header[18];

    FILE *f = fopen(fname, "rb");
    
    if ( f == NULL ) 
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: Failed to open '%s' for reading.", fname );
	return false;
    }

    if ( fread(header, 18, 1, f) != 1 ) 
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: Failed to read header of '%s'.", fname );
	fclose(f);
	return false;
    }

    // colormap info
    int cm_type = header[1];
    int cm_first = get16u(header + 3);
    int cm_count = get16u(header + 5);
    int cm_bits  = header[7];

    // image info
    int type = header[2];
    int xsize = get16u(header + 12);
    int ysize = get16u(header + 14);
    int bits  = header[16];

    /* image types:
     *
     *  0 - no image
     *  1 - colormap
     *  2 - RGB
     *  3 - grayscale
     *  9 - colormap, RLE
     * 10 - RGB, RLE
     * 11 - grayscale, RLE
     *
     */

    if ( cm_type > 1 || (type & ~(8+3)) != 0 || (type & 3) == 0 || ((type & 3) == 1 && cm_type == 0) ) 
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: '%s' is not a TGA image.", fname );
	fclose(f);
	return false;
    }

    if ( (type & 3) == 1 && cm_bits != 8 && cm_bits != 16 && cm_bits != 24 && cm_bits != 32 )
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: Unsupported colormap depth %d.", cm_bits );
	fclose(f);
	return false;
    }

    if ( ((type & 3) != 2 && bits != 8) || // colormap and grayscale must be 8-bit
	 ((type & 3) == 2 && bits != 16 && bits != 24 && bits != 32) ) // RGB may not be 8-bit
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: Unsupported depth %d for image type %d.", bits, type );
	fclose(f);
	return false;
    }

    ulSetError( UL_DEBUG, "ssgLoadTGA: Loading '%s', %s %dx%d-%d%s.",
		fname, 
		(type & 3) == 1 ? "colormap" : (type & 3) == 2 ? "RGB" : "grayscale",
		xsize, ysize, bits, 
		(type & 8) != 0 ? " RLE" : "");


    bool eof = false;


    // skip image identification
    
    if (fseek(f, header[0], SEEK_CUR) != 0)
	eof = true;


    // read colormap

    GLubyte *cm_data = 0;

    if (cm_type != 0) 
    {
	cm_data = new GLubyte [ (cm_bits / 8) * cm_count ];
	if (fread(cm_data, (cm_bits / 8) * cm_count, 1, f) != 1)
	    eof = true;
    }


    // read image data

    GLubyte *image = new GLubyte [ (bits / 8) * xsize * ysize ];

    if ((type & 8) != 0) 
    {
	// unpack RLE data

	int comp = (bits / 8);
	int current = 0;

	for (;;) 
	{
	    int code = getc(f);

	    if (code == EOF) {
		eof = true;
		break;
	    }

	    int length = (code & 0x7f) + 1;

	    if (current + length > xsize * ysize) 
	    {
		ulSetError( UL_WARNING, "ssgLoadTGA: RLE unpack problems." );
		fclose(f);
		delete [] cm_data;
		delete [] image;
		return false;
	    }

	    if ((code & 0x80) == 0) 
	    {
		// raw packet
		if (fread(image + current * comp, length * comp, 1, f) != 1) {
		    eof = true;
		    break;
		}
		current += length;
	    }
	    else
	    {
		// run-length packet
		GLubyte data[4];
		if (fread(data, comp, 1, f) != 1) {
		    eof = true;
		    break;
		}
		for (int i = 0; i < length; i++)
		    memcpy(image + (current++) * comp, data, comp);
	    }

	    if (current == xsize * ysize)
		break;
	}
    }
    else
    {
	if (fread(image, (bits / 8) * xsize * ysize, 1, f) != 1)
	    eof = true;
    }

    if (eof || ferror(f)) 
    {
	ulSetError( UL_WARNING, "ssgLoadTGA: %s.", 
		    ferror(f) ? "Read error" : "Unexpected end of file" );
	fclose(f);
	delete [] cm_data;
	delete [] image;
	return false;
    }

    fclose(f);


    // apply colormap

    if ((type & 3) == 1)
    {
	int comp = (cm_bits / 8);

	GLubyte *source = image;
	GLubyte *target = new GLubyte [ comp * xsize * ysize ];

	for (int i = 0; i < xsize * ysize; i++) 
	{
	    int index = source[i] - cm_first;
	    if (index < 0 || index > cm_count)
		memset(target + i * comp, 0, comp);
	    else
		memcpy(target + i * comp, cm_data + index * comp, comp);
	}

	delete [] source;

	image = target;
	bits = cm_bits;
    }

    delete [] cm_data;


    // convert image to plain GL_LUMINANCE, GL_RGB or GL_RGBA

    int zsize = (bits == 8) ? 1 : (bits == 24) ? 3 : 4;

    GLubyte *pixels = new GLubyte [ xsize * ysize * zsize ];
    
    for (int i = 0; i < xsize * ysize; i++)
    {
	switch (bits) {

	    case 8:
		pixels[i] = image[i];
		break;

	    case 16:
	    {
		int temp = get16u(image + 2*i); // ARRR RRGG GGGB BBBB
		pixels[4*i + 0] = (temp & 0x7c00) >> 7;
		pixels[4*i + 1] = (temp & 0x03e0) >> 2;
		pixels[4*i + 2] = (temp & 0x001f) << 3;
		pixels[4*i + 3] = (temp & 0x8000) ? 255 : 0;
		break;
	    }

	    case 24:
		pixels[3*i + 0] = image[3*i + 2];
		pixels[3*i + 1] = image[3*i + 1];
		pixels[3*i + 2] = image[3*i + 0];
		break;

	    case 32:
		pixels[4*i + 0] = image[4*i + 2];
		pixels[4*i + 1] = image[4*i + 1];
		pixels[4*i + 2] = image[4*i + 0];
		pixels[4*i + 3] = image[4*i + 3];
		break;
	}
    }

    delete [] image;

#if 0
    if (zsize == 3)
    {
	printf("writing 'out.ppm'...\n");
	FILE *f = fopen("out.ppm", "wb");
	fprintf(f, "P6\n%d %d\n255\n", xsize, ysize);
	fwrite(pixels, 3 * xsize * ysize, 1, f);
	fclose(f);
    }
#endif

    if ( info != NULL )
    {
	info -> width  = xsize;
	info -> height = ysize;
	info -> depth  = zsize;
	info -> alpha  = (zsize == 4);
    }

    return ssgMakeMipMaps ( pixels, xsize, ysize, zsize );


#else // old loader:


#define DEF_targaHeaderLength  12
#define DEF_targaHeaderContent "\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00"
  
  struct stat fileinfo;
  int bytesRead, width, height, maxLen;
  char *pData = NULL;
  
  if ( stat(fname, &fileinfo) == -1 ) {
    ulSetError ( UL_WARNING, "ssgLoadTexture: Failed to load '%s'.", fname);
    return false ;
  }
  
  FILE *tfile;
  if( (tfile = fopen(fname, "rb")) == NULL) {
    ulSetError ( UL_WARNING, "ssgLoadTexture: Failed to load '%s'.", fname);
    return false ;
  }
  
  maxLen = fileinfo.st_size;
  pData  = new char [maxLen];
  fread (pData, maxLen, 1, tfile);
  fclose (tfile);
  pData[0] = 0x00;
  
  if( memcmp( pData, DEF_targaHeaderContent, DEF_targaHeaderLength ) != 0 ) {
    ulSetError ( UL_WARNING, "ssgLoadTexture: Failed to load '%s'. Not a targa (apparently).", fname);
    delete[] pData;
    return false ;
  }
  
  unsigned char smallArray[ 2 ];
  
  memcpy( smallArray, pData + DEF_targaHeaderLength + 0, 2 );
  width = smallArray[ 0 ] + smallArray[ 1 ] * 0x0100;
  
  memcpy( smallArray, pData + DEF_targaHeaderLength + 2, 2 );
  height = smallArray[ 0 ] + smallArray[ 1 ] * 0x0100;
  
  memcpy( smallArray, pData + DEF_targaHeaderLength + 4, 2 );
  int depth = smallArray[ 0 ];
  // + smallArray[ 1 ] * 0x0100;
  
  if( ( width <= 0 ) || ( height <= 0 ) )
  {
    ulSetError ( UL_WARNING, "ssgLoadTexture: Failed to load '%s'. Width and height < 0.", fname);
    delete[] pData;
    return false ;
  }
  
  // Only allow 24-bit and 32-bit!
  bool is24Bit( depth == 24 );
  bool is32Bit( depth == 32 );
  
  if( !( is24Bit || is32Bit ) )
  {
    ulSetError ( UL_WARNING, "ssgLoadTexture: Failed to load '%s'. Not 24 or 32 bit.", fname);
    delete[] pData;
    return false ;
  }
  
  // Make it a BGRA array for now.
  int bodySize( width * height * 4 );
  unsigned char * texels = new unsigned char[ bodySize ];
  if( is32Bit )
  {
    // Texture is 32 bit
    // Easy, just copy it.
    memcpy( texels, pData + DEF_targaHeaderLength + 6, bodySize );
  }
  else if( is24Bit )
  {
    // Texture is 24 bit
    bytesRead = DEF_targaHeaderLength + 6;
    for( int loop = 0; loop < bodySize; loop += 4, bytesRead += 3 )
    {
      memcpy( texels + loop, pData + bytesRead, 3 );
      texels[ loop + 3 ] = 255;                      // Force alpha to max.
    }
  }
  
  // Swap R & B (convert to RGBA).
  for( int loop = 0; loop < bodySize; loop += 4 )
  {
    unsigned char tempC = texels[ loop + 0 ];
    texels[ loop + 0 ] = texels[ loop + 2 ];
    texels[ loop + 2 ] = tempC;
  }
  
  delete[] pData;
  
  if ( info != NULL )
  {
    info -> width = width ;
    info -> height = height ;
    info -> depth = 4 ;
    info -> alpha = is32Bit? 1: 0 ;
  }
  return ssgMakeMipMaps ( texels, width, height, 4) ;


#endif
}

#else

bool ssgLoadTGA ( const char *fname, ssgTextureInfo* info )
{
  ulSetError ( UL_WARNING,
    "ssgLoadTexture: '%s' - TGA support not configured", fname ) ;
  return false ;
}

#endif
