//
//           File : UErrors.h
//        Creator : Francois BERARD
//           Date : 07/95
//           Goal : Definition of miscelanous error numbers and 
//                      other units error number ranges
//
//    Main modifs :
//

#ifndef __Errors__
#define __Errors__

#include "UTypes.h"



//
//    StringError
//
// "StringError" returns a pointer to a string that explains the error
// code <err>.

char* ErrorString (ErrorNumber err);
ErrorNumber RegisterErrorString (ErrorNumber err, char* str);



// Miscellanous       errors ranges from    0 to  949
// I/O                errors ranges from  950 to  999

// Sound              errors ranges from 1000 to 1019
// SoundHardware      errors ranges from 1020 to 1039
// TBitmapSpec        errors ranges from 1040 to 1049
// TBitmap            errors ranges from 1050 to 1059
// SysCalls           errors ranges from 1060 to 1079
// Grabber            errors ranges from 1910 to 1929
// TSemaphore         errors ranges from 1930 to 1939
// TMovie             errors ranges from 1940 to 1949
// TMovieRecorder     errors ranges from 1950 to 1959
// TBitmapSource      errors ranges from 1960 to 1969

// TSocket            errors ranges from 2000 to 2019
// TConnectSocket     errors ranges from 2020 to 2039
// TSerialSocket      errors ranges from 2040 to 2049

// TCamControl        errors ranges from 2060 to 2069
// TEVID30            errors ranges from 2070 to 2079

// TColorModel        errors ranges from 2100 to 2109
// TProjective        errors ranges from 2110 to 2119

// TVLGrabber         errors ranges from 10910 to 10929
// TITManager         errors ranges from 10940 to 10949

// TMacOpenTransport  errors ranges from 10800 to 10809




// Miscellanous           errors ranges from 0 to 1000
const ErrorNumber  kErrNoErr          = 0;  // No error
const ErrorNumber  kErrUnknown        = 1;  // Unknown error
const ErrorNumber  kErrMemFull        = 2;  // Memory full error
const ErrorNumber  kErrBadParams      = 3;  // Bad parameters for the call
const ErrorNumber  kErrBadIndex       = 4;  // Bad parameters for the call
const ErrorNumber  kErrFileErr        = 5;  // FileSystem error
const ErrorNumber  kErrNoMoreProcess  = 6;  // Could not fork
const ErrorNumber  kErrIOErr          = 7;  // IO device error
const ErrorNumber  kErrSingularity    = 8;  // Singularity in computation
const ErrorNumber  kErrNoImpl         = 9;  // Not Implemented
const ErrorNumber  kErrRegErr         = 10; // Cannot register new error




// I/O                errors ranges from  950 to  999
const ErrorNumber  kErrIOOpenR        = 950;  // can't open device for reading
const ErrorNumber  kErrIOOpenW        = 951;  // can't open device for writing
const ErrorNumber  kErrIOOpenRW       = 952;  // can't open device for r / w
const ErrorNumber  kErrIOOpenA        = 953;  // can't open device for appending
const ErrorNumber  kErrIOOutput       = 954;  // can't output data
const ErrorNumber  kErrIOInput        = 955;  // can't input data


#endif


