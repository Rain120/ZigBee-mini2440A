#include "2440addr.h"
#include "uart0.h"

void adcInit(void)
{
	rADCCON &= ~0xffff;  // 清低16位
	rADCCON  = (1<<1) | (49<<6) | (1<<14);// 设置读启动 使能预分频 预分频值49 
	
	//rADCCON  = (1<<0) |  (49<<6) | (1<<14);   // // 设置手动启动 使能预分频 预分频值49 
}


void delay(int loops)
{
	while(loops-- > 0);
}

void Main(void)
{
	int adcVal = 0;
	
	adcInit();
	uart0_init();
	
	Uart0_Printf("Neuedu#\r\n");
	
	adcVal = rADCDAT0;  // 读启动ADC转换
	
	//rADCCON |= 0x1;       // 设置手动启动	
	//while(rADCCON &0x1);
	
	while(1)
	{	
	
		//rADCCON |= 0x1;       // 设置手动启动
		//while(rADCCON &0x1);
		
		while((rADCCON & (1<<15)) == 0);   // 等待转换完毕
		adcVal = rADCDAT0;   // 读取转换完毕后的数据
		Uart0_Printf("adcVal= %d\n\r", adcVal&0x3ff);
		
		delay(0x1000000);
	}
	


}