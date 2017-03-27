#include "2440addr.h"
#include "uart0.h"
#include "mmu.h"

void __irq alarm_irq_handler(void);

#define pISR_alarm   (*((unsigned int *) (0x33FFFF98)))

void setRTC(void)
{
	rRTCCON = 1;   // 使能读写
	
	rBCDYEAR = 0x17;   // 年
	rBCDMON = 0x03;
	rBCDDATE = 0x09;
	rBCDDAY = 0x4;
	rBCDHOUR = 0x14;
	rBCDMIN = 0x07;
	rBCDSEC = 0x10;
	
	rRTCCON = 0;   // disable 读写
	
	
}



void displayRTC(void)
{
	unsigned char year, month, date, hour, minute, second, day;
	
	//rRTCCON = 1;
	
	year = rBCDYEAR  ;		//年
	month = rBCDMON  ;		//月
	date = rBCDDATE  ;		//日	
	day = rBCDDAY  ;		//星期
	hour = rBCDHOUR  ;		//小时
	minute = rBCDMIN  ;		//分
	second = rBCDSEC  ;		//秒
		
	//rRTCCON &= ~1 ;
	
	Uart0_Printf("%04x-%02x-%02x %02x:%02x:%02x 星期%x\r\n",\
	 0x2000+year, month, date, hour, minute, second, day);
}


void alarmInit(void)
{
	rRTCALM |= (1<<0) | (1<<6);   // SEC 使能，总 使能
	rALMSEC = rBCDSEC + 5;        // 程序下载5s后闹铃
	
	// 初始化中断
	pISR_alarm = (unsigned int)alarm_irq_handler; // 存入汇编的地址表里
	rINTMSK &= ~(1<<30);   // 使能INT_RTC
	rSRCPND |= (1<<30);
	
	
}


// 热启动

// 中断服务程序
void __irq alarm_irq_handler(void)
{

	displayRTC();
	rSRCPND = (1<<30);
	rINTPND = (1<<30);
	rRTCALM = 0;
}


void Main(void)
{
	MMU_Init();
	uart0_init();
	setRTC();
	alarmInit();
	//displayRTC();
	
	
		
	while(1);
	

}