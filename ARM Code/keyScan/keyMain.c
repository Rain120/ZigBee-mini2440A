#include "mmu.h"

//LED
#define rGPBCON (*(unsigned int *)0x56000010)
#define rGPBDAT (*(unsigned int *)0x56000014)
//Key
#define rGPGCON (*(unsigned int *)0x56000060)
#define rGPGDAT (*(unsigned int *)0x56000064)

//Interrupt control
#define rSRCPND (*(unsigned int *)0x4A000000)
#define rINTMSK (*(unsigned int *)0x4A000008)
#define rINTPND (*(unsigned int *)0x4A000010)

//externer Interrupt
#define rEXTINT1 (*(unsigned int *)0x5600008C)
#define rEINTMASK (*(unsigned int *)0x560000A4)
#define rEINTPEND (*(unsigned int *)0x560000A8)

void ledInit(void);
void ledOn(void);
void ledOff(void);
void keyInit(void);
void eintInit(void);
void keyMain(void);


void ledInit(){
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
	rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
}

void ledOn(){
	rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
}

void ledOff(){
	rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
}

void keyInit(){
	//I/O初始化,设置外部中断
	rGPGCON |= (2<<0);
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

void delay(int loops){
	while(loops-- > 0);
}

void keyMain(){

	//int flag = 0;
	MMU_Init();
	ledInit();
	eintInit();
	while(1);
	/*
	while(1){
		if( (rGPGDAT & (1<<0)) == 0 ){
			if (flag % 2 == 0){
				ledOn();
			}
			else{
				ledOff();
			}
			flag++;
			delay(0x01000000);
		}
	}
*/
}
