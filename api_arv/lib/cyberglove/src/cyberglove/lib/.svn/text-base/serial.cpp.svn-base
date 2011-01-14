/************************************************************
*
*	Copyright(C) Satoshi Konno 1995
*
*	serial.cpp
*
************************************************************/
#include <stdio.h>
#include "serial.h"

CSerial::CSerial(char *device, int speed)
{
#ifdef SGI
    struct termios tmio;

    mFD = open(device, O_RDWR);
    if (mFD < 0) {
	fprintf(stderr, "SERIAL : Could't open %s\n", device);
	delete this;
	return;
    } 

    tcgetattr(mFD,&tmio);

    tmio.c_iflag &= ~(IMAXBEL | IXON | BRKINT);
    tmio.c_oflag &= ~OPOST;
    tmio.c_cflag &= ~(CBAUD | CIBAUD | CSIZE | CSTOPB | PARENB /*| CRTSCTS */);
    tmio.c_cflag |= CS8;
    tmio.c_lflag &= ~(ICANON | ECHO | ISIG);
    tmio.c_cc[VMIN] = 0;
    tmio.c_cc[VTIME] = 1;

    cfsetispeed(&tmio, speed);
    cfsetospeed(&tmio, speed);
	
    tcsetattr(mFD, TCSANOW, &tmio); //associe la structure
#elif WIN32
	int err;
	mFD=CreateFile(device, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
	if (!mFD)
		fprintf(stderr, "SERIAL : Could't open %s\n", device);
	else {
		DCB dcb;

		GetCommState(mFD, &dcb ) ;

		dcb.BaudRate	= speed;
		dcb.ByteSize	= 8;
		dcb.Parity		= NOPARITY;
		dcb.StopBits	= ONESTOPBIT;

		dcb.fOutxDsrFlow = 0;
		dcb.fDtrControl = DTR_CONTROL_ENABLE ;
		dcb.fOutxCtsFlow = 0;
		dcb.fRtsControl = RTS_CONTROL_ENABLE ;
		
		dcb.fInX = dcb.fOutX = 0;
		dcb.XonLim = 100;
		dcb.XoffLim = 100;

		dcb.fBinary = TRUE;
		dcb.fParity = TRUE;

		if (!SetCommState(mFD, &dcb)) {
			fprintf(stdout, "SERIAL : Could't set com state (%d)\n", speed);
			err = GetLastError();
		}
	}
#endif
}

CSerial::~CSerial(void)
{
#ifdef SGI
    if (mFD > 0)
	close(mFD);
#elif WIN32
	if (mFD)
		CloseHandle(mFD);
#endif
}

int CSerial::Read(char *bp, int len)
{
#ifdef SGI
    return read(mFD, bp, len);
#elif WIN32
	DWORD	nRead;
	DCB		dcb;
	if (!ReadFile(mFD, bp, len, &nRead, NULL))
		GetCommState(mFD, &dcb); 
	return nRead;
#endif
}

int CSerial::Write(char *bp, int len)
{
#ifdef DEBUG
    bp[len] = '\0';
    printf("SERIAL : Send data '%s'\n", bp);
#endif
#ifdef SGI
    return write(mFD, bp, len);
#elif WIN32
	DWORD		nWrite;
	DCB			dcb;
	DWORD		err;

	while (!WriteFile(mFD, bp, len, &nWrite, NULL)) {
		err = GetLastError();
		GetCommState(mFD, &dcb); 
	}
	return nWrite;
#endif
}

int CSerial::Write(char *bp)
{
    return Write(bp, strlen(bp));
}

int CSerial::Write(char c)
{
    return Write(&c, 1);
}
