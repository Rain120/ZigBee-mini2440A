#include "2440addr.h"
#include "uart0.h"

void setRTC(void)
{
	/*rRTCCON = 1;   // ʹ�ܶ�д
	
	rBCDYEAR = 0x17;   // ��
	rBCDMON = 0x03;
	rBCDDATE = 0x09;
//	rBCDDAY = 0x4;
	rBCDHOUR = 0x14;
	rBCDMIN = 0x07;
	rBCDSEC = 0x10;
	
	rRTCCON = 0;   // disable ��д
	*/
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x17 ;		//��
    rBCDMON  = 0x03 ;		//��
    rBCDDATE = 0x09 ;		//��	
	rBCDDAY  = 0x04 ;		//����
	rBCDHOUR = 0x19 ;		//Сʱ
    rBCDMIN  = 0x20 ;		//��
    rBCDSEC  = 0x00 ;		//��
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}


void Main(void)
{
	unsigned char year, month, date, hour, minute, second, day;
	uart0_init();
	setRTC();
	
	rRTCCON = 1;
	year = rBCDYEAR  ;		//��
	month = rBCDMON  ;		//��
	date = rBCDDATE  ;		//��	
	day = rBCDDAY  ;		//����
	hour = rBCDHOUR  ;		//Сʱ
	minute = rBCDMIN  ;		//��
	second = rBCDSEC  ;		//��
		
	rRTCCON &= ~1 ;
	Uart0_Printf("%2x-%2x-%2x %2x:%2x:%2x ����%x", year, month, date, hour, minute, second, day);
		
	while(1);
	

}