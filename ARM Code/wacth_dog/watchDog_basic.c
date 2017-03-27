#include "2440addr.h"
void watchDogMain(){
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);

	rWTCON &= ~0xffff;
	rWTCON |= (124<<8) | (1<<3) | (1<<0);
	rWTDAT = 0x30d4;
	rWTCNT = 0x30d4;
	
	while(1)
	{
		rGPBDAT &= (~(1<<5))  & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
		rWTDAT = 0x30d4;
		rWTCNT = 0x30d4;
		rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
	}
}