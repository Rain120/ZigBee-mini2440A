#include "2440addr.h"



void Main(void)
{
	// ¿ª¿´ÃÅ¹·
	rWTCON &= ~0xffff;
	rWTCON |= (124<<8) | (1<<5) | (1<<3) | (1<<0);
	
	rWTDAT = 0x30d4;
	rWTCNT = 0x30d4;
	
	while(1)
	{
		

		rWTDAT = 0x30d4;
		rWTCNT = 0x30d4;		
	}
	
}