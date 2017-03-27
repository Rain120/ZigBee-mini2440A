#include "2440addr.h"

void uart0Init(){
	// 1.引脚配置为RXD0 TXD0
	rGPHCON |= (rGPHCON &(~(0xf<<4))) | (2<<4) | (2<<6);
	
	// uart控制器
	rULCON0 = (3<<0);    // 普通模式，无奇偶校验，8位数据，1位停止位
	rUCON0 = 0;
	
	rUBRDIV0 = 26;  // pclk = 50M, (pclk / (115200*16)-1)四舍五入
	
	rUCON0 |= (1<<0) | (1<<2);   // 查询或中断方式启动串口发送接收。 
	

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