/*
����k1������LED1
*/

#include "mmu.h"

#define rGPBCON  (*(unsigned int *)0x56000010)
#define rGPBDAT	 (*(unsigned int *)0x56000014)

#define rGPGCON  (*(unsigned int *)0x56000060)
#define rGPGDAT  (*(unsigned int *)0x56000064)


// �жϿ�����
#define rSRCPND  (*(unsigned int *)0X4A000000)
#define rINTMSK  (*(unsigned int *)0X4A000008)
#define rINTPND  (*(unsigned int *)0X4A000010)



// �ⲿ�ж�
#define rEXTINT1 (*(unsigned int *)0x5600008C)
#define rEINTMASK  (*(unsigned int *)0x560000A4)
#define rEINTPEND	 (*(unsigned int *)0x560000A8)



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

// �ⲿ�жϳ�ʼ��
void eintInit(void)
{
	// io����Ϊ�ⲿ�жϹ���
	rGPGCON |= (2<<0);
	
	// �жϿ��Ƶĳ�ʼ��
	rINTMSK &= ~(1<<5);   // ʹ��EINT8_23
	
	// �ⲿ�жϵĳ�ʼ��
	// �½��ش���
	rEXTINT1 |= (2<<0); 
	rEINTMASK &= ~(1<<8);  // ʹ��eint8
	
	// ��Դ�Ĵ���
	rSRCPND |= (1<<5); 
	rEINTPEND |= (1<<8);
	
	
}


void Main(void)
{
	// ��ʼ��led	
	ledInit();
	ledon();
	//MMU_Init();

	eintInit();
	
	
	
	while(1)
	{
		;
	}
}