#include "2440addr.h"
#include "uart0.h"

void setRTC(void)
{
	/*rRTCCON = 1;   // 使能读写
	
	rBCDYEAR = 0x17;   // 年
	rBCDMON = 0x03;
	rBCDDATE = 0x09;
//	rBCDDAY = 0x4;
	rBCDHOUR = 0x14;
	rBCDMIN = 0x07;
	rBCDSEC = 0x10;
	
	rRTCCON = 0;   // disable 读写
	*/
	rRTCCON = 1 ;		//RTC read and write enable

	rBCDYEAR = 0x17 ;		//年
    rBCDMON  = 0x03 ;		//月
    rBCDDATE = 0x09 ;		//日	
	rBCDDAY  = 0x04 ;		//星期
	rBCDHOUR = 0x19 ;		//小时
    rBCDMIN  = 0x20 ;		//分
    rBCDSEC  = 0x00 ;		//秒
	
	rRTCCON &= ~1 ;		//RTC read and write disable
}


void Main(void)
{
	unsigned char year, month, date, hour, minute, second, day;
	uart0_init();
	setRTC();
	
	rRTCCON = 1;
	year = rBCDYEAR  ;		//年
	month = rBCDMON  ;		//月
	date = rBCDDATE  ;		//日	
	day = rBCDDAY  ;		//星期
	hour = rBCDHOUR  ;		//小时
	minute = rBCDMIN  ;		//分
	second = rBCDSEC  ;		//秒
		
	rRTCCON &= ~1 ;
	Uart0_Printf("%2x-%2x-%2x %2x:%2x:%2x 星期%x", year, month, date, hour, minute, second, day);
		
	while(1);
	

}