
#include "2440addr.h"

#define rrUTXH0 (*(volatile unsigned char *)0x50000020)	//UART 0 Transmission Hold
#define rrURXH0 (*(volatile unsigned char *)0x50000024)


void uart0Init(void)
{
	// 1.引脚配置为RXD0 TXD0
	rGPHCON |= (2<<4) | (2<<6);
	
	// uart控制器
	rULCON0 = (3<<0);    // 普通模式，无奇偶校验，8位数据，1位停止位
	rUCON0 = 0;
	
	rUBRDIV0 = 26;  // pclk = 50M, (pclk / (115200*16)-1)四舍五入
	
	rUCON0 |= (1<<0) | (1<<2);   // 查询或中断方式启动串口发送接收。 
}

void sendChar(char ch)
{
	while((rUTRSTAT0 & (1<<1)) == 0);
	rrUTXH0 = ch;
	return;
}


char recvChar(void)
{
	while((rUTRSTAT0 & (1<<0)) == 0);
	return rrURXH0;
}

void sendString(char *str)
{
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

void Main(void)
{
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