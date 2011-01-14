/************************************************************
*
*	Copyright(C) Satoshi Konno 1995
*
*	cyberglove.h
*
************************************************************/
#ifndef _CYBERGLOVE_H_
#define _CYBERGLOVE_H_

#include <stdio.h>
#include <math.h>
#include "serial.h"

#define TRUE	1
#define FALSE	0

#define CG_GetAsciiRecode	"g"
#define CG_GetBinaryRecode	"G"
#define CG_SetNoTimeStamp	"D0"
#define CG_SetNoGloveStatus	"U0"
#define CG_GetRightHand		"?R"
#define CG_GetSensorNum		"?n"

class CCyberGlove : public CSerial
{
    int		nSensors;
    char	buffer[256];
	int		value[22];
public:
    CCyberGlove(char *device, int speed);			
    ~CCyberGlove();					   
    int	    GetNSensors(void)	    		{return nSensors;}
    int	    GetMessage(int n);
    void    UpdateData();     		    
    int 	GetData(int i)					{return value[i];}
	void	Wait(double t);
};

#endif
