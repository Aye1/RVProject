/************************************************************
*
*	Copyright(C) Satoshi Konno 1995
*
*	serial.h
*
************************************************************/
#ifndef _SERIAL_H_
#define _SERIAL_H_

//mega HACK here !!
#define LINUX

#ifdef LINUX

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERIAL1	"/dev/ttyd1"
#define SERIAL2	"/dev/ttyd2"

typedef int FileHandle;

#elif WIN32

#include <windows.h>

#define SERIAL1 "COM1"
#define SERIAL2	"COM2"

typedef HANDLE	FileHandle;

#define B9600	9600
#define B19200	19200
#define B38400	38400
#define B57600	57600
#define B115200	115200

#endif

class CSerial {
    FileHandle	mFD;
public:
    CSerial(char *device, int speed);
    ~CSerial(void);
    int Read(char *bp, int len);
    int Write(char *bp, int len);
    int Write(char *bp);
    int Write(char c);
};

#endif
