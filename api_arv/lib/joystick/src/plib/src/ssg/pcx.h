// Loading of the PCX texture file format
//
// This file is used for both PLIB and for BoB.
//
// Written by Wolfram Kuss in december 2003
//
// This file is published as public domain


struct pcxHeaderType		
   {
   char manufacturer;		// always 10
   char version;        // usually 5
   char encoding;		    // 1 = RLE
   char bitsPerPixel;		// number of bits to represent a pixel
   short x, y;			    // upper left coordinates
   short xmax, ymax;		
   short hDPI, vDPI;	  // DPI
   char egaPalette[48];	// obsolete
   char reserved;       // unused, always 0
   char nColorPlanes;		// number of color planes. not implemented yet?
   short bytesPerLine;	// bytes per scan line. not implemented yet? todo kludge
   short paletteInfo;		// colored or grey scale. not implemented yet?
	 short hScreenSize, cScreenSize;
   char padding[54];		// reserved, always 0
   };


#define READ_BYTE (buffer[0]); buffer++;
#define READ_SHORT ((((unsigned long)buffer[0])<<8) + buffer[1]); buffer += 2;
#define READ_LONG ((((unsigned long)buffer[0])<<24) + (((unsigned long)buffer[1])<<16) + (((unsigned long)buffer[2])<<8) + buffer[3]); buffer += 4;


// It is bad style to have a function inside a header file, but this way there is only one file
// I need to "synchronise" between PLIB and BoB.

static int ReadPCXBody(UByte*& buffer, pcxHeaderType *ppcxHeader, UByte * pBody)
// returns TRUE on success.
// reads from buffer (which will normally just hold the file contents, but in memory,
// writes to pBody, which must have been allocated
{
	unsigned short x,y;

  if ((ppcxHeader->bitsPerPixel != 8)	// unsupported pcx
		|| (ppcxHeader->encoding>1) // invalid PCX
   //   || (pcxHeader.nColorPlanes != 1)  	// unsupported pcx
      || (ppcxHeader->manufacturer != 10))    // invalid pcx
		return (FALSE);

	unsigned short width = ppcxHeader->xmax-ppcxHeader->x+1;

	x=0; y=0;				// upper left coordinates
	for(;;)
	{
		UByte color = READ_BYTE;
		if ((color &0xC0) != 0xC0)			// uncompressed?
		{
			*pBody++ = color;
			x++;
			if (x >= width) 
			{ x=0; y++; 	// next line
				if(y > ppcxHeader->ymax-ppcxHeader->y)
					break;
			}
		}
		else				
		{
			UByte counter, length = color & 0x3F;		// number of times next byte repeated
			color = READ_BYTE;
			for (counter=0; counter<length; counter++)
			{
				*pBody++ = color;
				x++;
				if (x >= width) 
				{ x=0; y++; // next line
					counter=length;
					if(y > ppcxHeader->ymax-ppcxHeader->y)
						break; // breaks for (counter=0; counte...
				}
			}
			if(y > ppcxHeader->ymax-ppcxHeader->y)
				break; // breaks for(;;)
		}
	}
	return TRUE;
}
