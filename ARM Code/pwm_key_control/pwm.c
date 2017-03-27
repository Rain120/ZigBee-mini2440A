
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

void Main(void)
{

	int i=0;
	toutIoCfg();

	while(1);
}