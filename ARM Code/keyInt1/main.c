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
	ledoff();
	eintInit();
	MMU_Init();


	
	
	
	while(1);
}


/*

1.���IRQ�źųɹ��Ĵ��ݵ���arm920t�ںˣ��ں���Ϊ������һ��irq�쳣��
�ں˼��CPSR��Iλ�����Ƿ�ΪI=0������ǣ��ͻᴦ������쳣��
�ں���δ�������쳣�أ�
1��cpsr -> spsr_irq
2)�޸�cpsr��ֵ��
	I = 1
	MOD = 0x12
	lr = pc-4
	T = 0
	PC = 0x18
	
2.��������Ҫ���IRQ�ж�:
�жϷ���ǰ��
	��ʼ��arm920t��ؼĴ�����
	��	1��ʹ��IRQ  I=0
		2) �л�������ģʽ�£���ʼ����Ӧģʽ��sp
		
	��ʼ������֤�ж������ź��ܹ��ɹ��Ĵ��ݵ�arm920t�С�
		1��GPIO����ѡ��Ĵ�����ѡ���ⲿ�жϹ���
		2�������źų�ʼ�����͵�ƽ�������أ��½��ص�)
		3��ʹ��EINTMASK��Ӧ��λ
		4��ʹ��INTMSK��Ӧ��λ��	

		
	׼��Irq�쳣����Ĵ��룺
		1���ж�������0x18��λ��д b	irqHandler
		2) irqHandler���λ�ã�����IRQ��ҵ��
		3�����������Ҫ�쳣���ء�
		
		
	
*/