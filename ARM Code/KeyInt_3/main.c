#include "2440addr.h"
#include "led.h"
#include "mmu.h"

#define _ISR_STARTADDRESS 	(0x33ffff00+0x20)
#define pISR_Eint0	(*(unsigned *)(_ISR_STARTADDRESS + 20))
void __irq EINT8_23_isq(void);

/*功能：中断初始化
参数：
返回值：
*/
void irq_init(void){
	rEXTINT1 = 0x3;
	rEINTMASK = 0x0;
	rINTMSK &= ~0x20;
	
	rGPGCON |= (0x1 << 1);
	rEINTPEND = 0x1 << 8;
	
	rSRCPND = 0x1 << 5;
	rINTPND = 0x1 << 5;
	pISR_Eint0 = (unsigned int)EINT8_23_isq;
}

/*功能：关闭警报
参数：
返回值：
*/
unsigned int uiFlag = 0;
void __irq EINT8_23_isq(void){
	rEINTPEND = rEINTPEND;
	rSRCPND = rSRCPND;
	rINTPND = rINTPND;
	if (0 == uiFlag)
	{
		led_close();
		uiFlag = 1;
	}
	else
	{
		led_open();
		uiFlag = 0;
	}
	
}
/*
tclk = pclk / ((pre + 1) * div)

fout = 1kHz
pclk = 50MHz
pre = 15
div = 8

1/fout = count * (1/tclk)
1/fout = count * ((pre+1) * div) / pclk

count = pclk / (fout*(pre+1)* div)
	  = 50Mhz / (1KHz * 16 * 8)
	  = 390.625  = 391
*/

/*

*/
#define PCLK 50000000
/*	
void setFre(int fre)
{
	// 定时器配置
	rTCFG0 |= (15<<0);   // pre = 15
	rTCFG1 |= (2<<0);    // div = 8
	
	rTCNTB0 =  PCLK / (fre * 16 * 8); 
	       
	rTCMPB0 = rTCNTB0 >> 1;   // 占空比50%
	
	rTCON |= (1<<3) | (1<<1) | (1<<0);  // 自动加载，手动更新，启动定时器。
	
	rTCON &= ~(1<<1);   //  关闭手动加载
}	
*/

void toutIoCfg(void)
{
	// 引脚配置
	rGPBCON &= ~(3<<0);
	rGPBCON |= (2<<0);   // 配置为tout0功能
}
	
void tout0Init(void)
{

	// 引脚配置
	rGPBCON &= ~(3<<0);
	rGPBCON |= (2<<0);   // 配置为tout0功能
	
	// 定时器配置
	rTCFG0 |= (15<<0);   // pre = 15
	rTCFG1 |= (2<<0);    // div = 8
	
	rTCNTB0 = 391;        
	rTCMPB0 = rTCNTB0 >> 1;   // 占空比50%
	
	rTCON |= (1<<3) | (1<<1) | (1<<0);  // 自动加载，手动更新，启动定时器。
	
	rTCON &= ~(1<<1);   //  关闭手动加载
	 
}

void delay(unsigned int loops)
{
	while(loops-->0);
}

int Main(void)
{
	//toutIoCfg();
	MMU_Init();
	led_init();
	led_open();
	irq_init();
	
	
	while(1);
	
	return 0;
}