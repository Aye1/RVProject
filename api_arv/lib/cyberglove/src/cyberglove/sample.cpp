/************************************************************
*
*	Copyright(C) Satoshi Konno 1995
*
*	sample.cpp
*
************************************************************/

#include <stdio.h>
#include "cyberglove.h"

int main(int argc, char **argv)
{
    CCyberGlove cglove("/dev/ttyS1", B115200);

	while(1) {
		cglove.UpdateData();
		
		for (int i=0; i<cglove.GetNSensors(); i++)
			printf("%d ", cglove.GetData(i));
		printf("\n");
	}
	return 0;
}
