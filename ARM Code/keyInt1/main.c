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
	ledoff();
	eintInit();
	MMU_Init();


	
	
	
	while(1);
}


/*

1.如果IRQ信号成功的传递到了arm920t内核，内核认为发生了一个irq异常：
内核检测CPSR的I位，看是否为I=0，如果是，就会处理这个异常。
内核如何处理这个异常呢？
1）cpsr -> spsr_irq
2)修改cpsr的值：
	I = 1
	MOD = 0x12
	lr = pc-4
	T = 0
	PC = 0x18
	
2.软件如果想要检测IRQ中断:
中断发生前：
	初始化arm920t相关寄存器：
	·	1）使能IRQ  I=0
		2) 切换到各个模式下，初始化相应模式的sp
		
	初始化：保证中断请求信号能够成功的传递到arm920t中。
		1）GPIO功能选择寄存器，选择外部中断功能
		2）触发信号初始化（低电平，上升沿，下降沿等)
		3）使能EINTMASK相应的位
		4）使能INTMSK相应的位。	

		
	准备Irq异常处理的代码：
		1）中断向量表0x18的位置写 b	irqHandler
		2) irqHandler标号位置，处理IRQ的业务
		3）处理完后需要异常返回。
		
		
	
*/