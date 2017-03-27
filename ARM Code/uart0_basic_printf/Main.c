#include "uart0.h"


int Main(void)
{
	unsigned char buf[] = "hello neusoft#\n";
	int aa = 120;
	
	rGPBCON = 0x015550;
	rGPBDAT = 0x1e0;
	
	uart0_init();
	
	
	Uart0_Printf("aa = %d\n", aa);
	
	uart0_sendstring(buf);
	
	while(1)
	{
		//uart0_sendstring(buf);
		uart0_sendchar(uart0_recv());
	}

	/*
	//exchangeab();
	while(1)
	{
		rGPBDAT = 0x1e0;
		asmdelay();
		rGPBDAT = 0;
		asmdelay();
	}*/
	return 0;
}