/*
按下k1，点亮LED1
*/

#include "mmu.h"

#define rGPBCON  (*(unsigned int *)0x56000010)
#define rGPBDAT	 (*(unsigned int *)0x56000014)

#define rGPGCON  (*(unsigned int *)0x56000060)
#define rGPGDAT  (*(unsigned int *)0x56000064)


// 中断控制器
#define rSRCPND  (*(unsigned int *)0X4A000000)
#define rINTMSK  (*(unsigned int *)0X4A000008)
#define rINTPND  (*(unsigned int *)0X4A000010)



// 外部中断
#define rEXTINT1 (*(unsigned int *)0x5600008C)
#define rEINTMASK  (*(unsigned int *)0x560000A4)
#define rEINTPEND	 (*(unsigned int *)0x560000A8)



// led初始化
void ledInit(void)
{
	// LED IO设置为输出
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
	// 灭灯
	rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
	
}

// 点亮led
void ledon(void)
{
	rGPBDAT &= (~(1<<5))  & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
}


// 熄灭led
void ledoff(void)
{
	rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
}

// 外部中断初始化
void eintInit(void)
{
	// io设置为外部中断功能
	rGPGCON |= (2<<0);
	
	// 中断控制的初始化
	rINTMSK &= ~(1<<5);   // 使能EINT8_23
	
	// 外部中断的初始化
	// 下降沿触发
	rEXTINT1 |= (2<<0); 
	rEINTMASK &= ~(1<<8);  // 使能eint8
	
	// 清源寄存器
	rSRCPND |= (1<<5); 
	rEINTPEND |= (1<<8);
	
	
}


void Main(void)
{
	// 初始化led	
	ledInit();
	ledon();
	//MMU_Init();

	eintInit();
	
	
	
	while(1)
	{
		;
	}
}