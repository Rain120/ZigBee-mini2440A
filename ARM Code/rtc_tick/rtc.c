#include "2440addr.h"
#include "uart0.h"
#include "mmu.h"

void __irq alarm_irq_handler(void);
void __irq tick_irq_handler(void);

#define pISR_alarm   (*((unsigned int *) (0x33FFFF98)))
#define pISR_tick    (*((unsigned int *) (0x33FFFF20 + 32)))

void setRTC(void)
{
	rRTCCON = 1;   // ʹ�ܶ�д
	
	rBCDYEAR = 0x17;   // ��
	rBCDMON = 0x03;
	rBCDDATE = 0x09;
	rBCDDAY = 0x4;
	rBCDHOUR = 0x16;
	rBCDMIN = 0x3;
	rBCDSEC = 0x0;
	
	rRTCCON = 0;   // disable ��д
	
	
}



void displayRTC(void)
{
	unsigned char year, month, date, hour, minute, second, day;
	
	//rRTCCON = 1;
	
	year = rBCDYEAR  ;		//��
	month = rBCDMON  ;		//��
	date = rBCDDATE  ;		//��	
	day = rBCDDAY  ;		//����
	hour = rBCDHOUR  ;		//Сʱ
	minute = rBCDMIN  ;		//��
	second = rBCDSEC  ;		//��
		
	//rRTCCON &= ~1 ;
	
	Uart0_Printf("%04x-%02x-%02x %02x:%02x:%02x ����%x\r\n",\
	 0x2000+year, month, date, hour, minute, second, day);
}


void alarmInit(void)
{
	rRTCALM |= (1<<0) | (1<<6);   // SEC ʹ�ܣ��� ʹ��
	rALMSEC = rBCDSEC + 5;        // ��������5s������
	
	// ��ʼ���ж�
	pISR_alarm = (unsigned int)alarm_irq_handler; // ������ĵ�ַ����
	rINTMSK &= ~(1<<30);   // ʹ��INT_RTC
	rSRCPND |= (1<<30);
	
	
}


void tickInit(void)
{

	rTICNT = (1<<7) | 127;   // ʹ��tick, ����ֵ127
	
	// �жϿ�����
	rINTMSK &= ~(1<<8);
	
	rSRCPND |= (1<<8);
	rINTPND |= (1<<8);
	
	pISR_tick = (unsigned int)tick_irq_handler;
	
}



void __irq tick_irq_handler(void)
{
	displayRTC();
	
	rSRCPND = rSRCPND;
	rINTPND = rINTPND;
	
	
}

// ������ : ��ʱ5s

// �жϷ������
void __irq alarm_irq_handler(void)
{

	tickInit();
	//rSRCPND = (1<<30);
	//rINTPND = (1<<30);
	//rSRCPND |= (1<<30);
	//rINTPND |= (1<<30);
	rSRCPND = rSRCPND;
	rINTPND = rINTPND;
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