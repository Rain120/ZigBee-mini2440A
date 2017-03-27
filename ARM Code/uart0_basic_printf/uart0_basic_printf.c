#include "uart0.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void uart0_init(void)
{
	//初始化引脚，选择串口0功能
	rGPHCON = (rGPHCON &(~(0xf << 4))) | (0x2 << 4) | (0x2 << 6);
	rULCON0 = (0x3 << 0);
	
	rUFCON0 = 0;
	rUBRDIV0 = 26;
	
	rUCON0 = 0;
	rUCON0 = (1<<2) | (1<<0);
	
	return;	
}

void uart0_sendchar(unsigned char ch)
{
	while( (rUTRSTAT0 & (1<<1)) == 0)
	{
		;
	}
	rUTXH0 = ch;
	return;
}

void uart0_sendstring(unsigned char *str)
{
	while(*str != '\0')
	{
		uart0_sendchar(*str);
		str++;
	}
	return;
} 

unsigned char uart0_recv(void)
{
	while((rUTRSTAT0 & (1 << 0)) == 0)
	{
		;
	}

	return rURXH0;
}


void Uart0_Printf(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    uart0_sendstring((unsigned char*)string);
    va_end(ap);
} 

