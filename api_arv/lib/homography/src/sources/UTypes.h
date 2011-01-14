//
//       File : UTypes.h
//    Creator : Francois BERARD
//       Date : 07/95
//       Goal : Global custom type definitions
//
//    Main modifs : N.Oliver (7/96) new functionality in TRect
//

#ifndef __UTypes_H_
#define __UTypes_H_



// project includes
/*#include "UArch.h"*/

// ANSI includes
extern "C" {
#include <stdlib.h>    // for the definition of size_t
#include <stdio.h>
#include <string.h>
}

#define TEMPLLIST(x) <x>


// Byte order: the VL_BYTEORDER is defined later and set to one
//    of these constants:

#define VL_LITTLEENDIAN   0123
#define VL_BIGENDIAN      3210



// plateform specific includes and definitions

#ifdef __MacOS__

  #define VL_BYTEORDER  VL_BIGENDIAN
#endif

#ifdef __Darwin__

  #define __UNIX
  #define VL_BYTEORDER  VL_BIGENDIAN

#endif

#ifdef __IRIX__

  #define __UNIX

  #define VL_BYTEORDER  VL_BIGENDIAN

  #undef TEMPLLIST
  #define TEMPLLIST(x)

  #ifdef __EDG_ABI_COMPATIBILITY_VERSION
    #define __MIPS_PRO
  #endif
#endif



#ifdef __LINUX__
  #define __UNIX

  #undef TEMPLLIST
  #define TEMPLLIST(x) <x>

  #include <endian.h>

  #if __BYTE_ORDER == __BIG_ENDIAN
    #define VL_BYTEORDER VL_BIGENDIAN
  #else
    #define VL_BYTEORDER VL_LITTLEENDIAN
  #endif

#endif



#ifdef __WIN32__
    #define VL_BYTEORDER VL_LITTLEENDIAN
#endif


#ifdef __SunOS__
  #define __UNIX
#endif

#ifdef __SOLARIS__
  #define __UNIX
#endif

#ifdef __UNIX
  #include <sys/types.h>
#endif

/// return true if the underlying system does Most Significant Byte first.
inline bool MSB()
{
 int i = 1;
 return (*((char *) &i) == 0);
}

////////// Custom type definitions

typedef unsigned char   UInt8;      // 1 byte integer
typedef unsigned short  UInt16;     // 2 bytes integer
typedef unsigned long   UInt32;     // 4 bytes integer

typedef signed char     SInt8;      // 1 byte signed integer
typedef signed short    SInt16;     // 2 bytes signed integer
typedef signed long     SInt32;     // 4 bytes signed integer

typedef SInt16      ErrorNumber;

typedef float       Float32;    // 4 bytes float

typedef size_t      BlockSize;
typedef void*       Pointer;

#ifdef __MacOS__
typedef SInt32      ProcessID;
#else
  #ifdef __UNIX
    typedef pid_t   ProcessID;
  #endif
#endif

typedef void        _Callback (ErrorNumber err, Pointer param);
typedef _Callback*  Callback;


#ifndef NULL
#define NULL (Pointer)0
#endif

#ifdef __UNIX

  typedef int       SysSemID;

#endif



class TRect
{
  public :

    SInt32 left;
    SInt32 top;
    SInt32 right;
    SInt32 bottom;

    void    Set (SInt32 l, SInt32 t, SInt32 r, SInt32 b)
              { left = l; top = t; right = r; bottom = b; }
    void    Set (TRect* r)
              { left = r->left; top = r->top;
                right = r->right; bottom = r->bottom; }
    UInt32  Width ()
              { return (UInt32)(right-left); }
    UInt32  Height ()
              { return (UInt32)(bottom-top); }
    void    Offset (SInt32 dx = 0, SInt32 dy = 0)
              { left += dx; right += dx;
                top += dy; bottom += dy; }
};




#endif






