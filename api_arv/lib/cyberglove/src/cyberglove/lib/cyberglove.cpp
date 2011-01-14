/************************************************************
*
*	Copyright(C) Satoshi Konno 1995
*
*	cyberglove.cpp
*
************************************************************/
#include <stdlib.h>
#include <time.h>
#include "cyberglove.h"
#include "serial.h"

void CCyberGlove::Wait(double t)	/* Wait (msec order) */
{
	float fStart = (double)clock();
	while (t > ((double)clock() - fStart) / (double)CLOCKS_PER_SEC );
}

int CCyberGlove::GetMessage(int nMessage)
{
    int n = 0;

    while (n < nMessage) {
		n += Read(buffer + n, nMessage - n);
    	buffer[n] = '\0';
	}
#ifdef DEBUG
    printf("CyberGlove : Received data(%d) is '%s'\n", n, buffer);
#endif
    return TRUE; 
}

void CCyberGlove::UpdateData(void)
{
	Write(CG_GetBinaryRecode);
	GetMessage(1+GetNSensors()+1);
	for (int i=0; i<GetNSensors(); i++) {
		value[i] = (int)((unsigned char)buffer[i+1]);
	}
}

CCyberGlove::CCyberGlove(char *device, int speed) : CSerial(device, speed)
{
	/* Reset CGIU */
/*
	char comm[2];
	comm[0] = 0x12;
	comm[1] = '\0'; 
	Write(comm);Wait(1.0);
	Read(buffer, 4);
*/
#ifndef DEBUG
    printf("CyberGlove : Reset %s\n", buffer);
#endif

	/* Get Sensor Number */
	Write(CG_GetSensorNum);Wait(.5);
	Read(buffer, 9);
	nSensors = atoi(&buffer[3]);
#ifndef DEBUG
    printf("CyberGlove : Sensor Number (%d) %s\n", nSensors, buffer);
#endif
}

CCyberGlove::~CCyberGlove()
{
}
