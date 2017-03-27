#include "uart0.h"
#include "2440addr.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void uart0_init(void)
{

/*	//��ʼ�����ţ�ѡ�񴮿�0����
	rGPHCON = (rGPHCON &(~(0xf << 4))) | (0x2 << 4) | (0x2 << 6);
	rULCON0 = (0x3 << 0);
	
	rUFCON0 = 0;
	rUBRDIV0 = 26;
	
	rUCON0 = 0;
	rUCON0 = (1<<2) | (1<<0);*/
	
		// 1.��������ΪRXD0 TXD0
		
	rGPHCON |= (2<<4) | (2<<6);
	
	// uart������
	rULCON0 = (3<<0);    // ��ͨģʽ������żУ�飬8λ���ݣ�1λֹͣλ
	rUCON0 = 0;
	
	rUBRDIV0 = 26;  // pclk = 50M, (pclk / (115200*16)-1)��������
	
	rUCON0 |= (1<<0) | (1<<2);   // ��ѯ���жϷ�ʽ�������ڷ��ͽ��ա� 
	
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

