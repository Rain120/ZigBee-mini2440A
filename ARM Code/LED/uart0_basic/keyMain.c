#include "2440addr.h"

void keyInit(){
	//I/O初始化,设置外部中断
	rGPGCON |= (2<<0);
}

// 点亮led
void ledon1(void)
{
	rGPBDAT &= (~(1<<5));
}

void eintInit(){
	keyInit();
	//中断控制器
	rINTMSK &= (~(1<<5));
	//外部中断初始化
	//下降沿触发
	rEXTINT1 |= (2<<0);
	//使能eint8
	rEINTMASK &= ~(1<<8);
	
	//清源寄存器
	rSRCPND |= (1<<5);
	rEINTPEND |= (1<<8);
}

void keyMain(){ 
	ledInit();
	ledon1();
	eintInit();
	//while(1);
}