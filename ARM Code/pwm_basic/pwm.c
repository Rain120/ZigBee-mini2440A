
#include "2440addr.h"

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

void delay(unsigned int loops);
//led
void ledInit(void);
void ledON(void);
void ledOFF(void);
void ledBlink(void);

//key
void keyInit(void);

//PWM

void PWM_ON(void);
void PWM_OFF(void);

void ledInit(void)            //初始化LED 
{  
	rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
}

void ledON(void)
{
	rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));  //亮
}

void ledOFF(void)
{
	 rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8); //暗
}
void ledBlink(void)
{
	ledON();
	delay(0x05000000);
	ledOFF(); 
	delay(0x05000000);
}
void keyInit(void)
{
	//I/O初始化,设置外部中断
	rGPGCON |= (2<<0);
}
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

void toutIoCfg(void)
{
	// 引脚配置
	rGPBCON &= ~(3<<0);
	rGPBCON |= (2<<0);   // 配置为tout0功能
}
*/
void PWM_ON(void)
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

void PWM_OFF(void)
{
	rGPBCON &= ~(0x3);      //关闭GPB0的TOUT0功能，  
    rGPBCON |= 0x01;        //设置GPB0为01输出功能  
    rGPBDAT &= 0x0;;        //设置GPB0数据寄存器为0，对蜂鸣器来说1代表发出声音，而0则代表不出声音
	
}

void delay(unsigned int loops)
{
	while(loops-->0);
}

void Main(void)
{
	ledInit();
	keyInit();
	PWM_ON();
	ledON();
	while(1)
	{
		//0-K1 3-K2 5-K3 6-K4
		if(!(rGPGDAT & (1<<3)))
		{
			delay(0x1000);
			if(!(rGPGDAT & (1<<3)))
			{
				PWM_OFF();
				ledOFF();
			}
		}	
	}
}