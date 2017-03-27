#include "2440addr.h"

void uart0Init(){
	// 1.��������ΪRXD0 TXD0
	rGPHCON |= (rGPHCON &(~(0xf<<4))) | (2<<4) | (2<<6);
	
	// uart������
	rULCON0 = (3<<0);    // ��ͨģʽ������żУ�飬8λ���ݣ�1λֹͣλ
	rUCON0 = 0;
	
	rUBRDIV0 = 26;  // pclk = 50M, (pclk / (115200*16)-1)��������
	
	rUCON0 |= (1<<0) | (1<<2);   // ��ѯ���жϷ�ʽ�������ڷ��ͽ��ա� 
	

}

void sendChar(char ch){
	while((rUTRSTAT0 & (1<<1)) == 0);
	rUTXH0 = ch;
	return;
}


char recvChar(void){
	while((rUTRSTAT0 & (1<<0)) == 0);
	return rURXH0;
}

void sendString(char *str){
	while(*str != '\0')
	{
		sendChar(*str);
		str++;
	}
	return;
}

// led��ʼ��
void ledInit(void)
{
	// LED IO����Ϊ���
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
	// ���
	rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
	
}

// ����led
void ledon(void)
{
	rGPBDAT &= (~(1<<5))  & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
}


// Ϩ��led
void ledoff(void)
{
	rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
}

void uartMain(){
	char ch;
	ledInit();
	ledoff();
	uart0Init();
	
	sendString("Neusoft#");
	ledon();

	while(1)
	{
		ch = recvChar();
		sendChar(ch);
	}
}