#include "2440addr.h"
#include "uart0.h"

void adcInit(void)
{
	rADCCON &= ~0xffff;  // ���16λ
	rADCCON  = (1<<1) | (49<<6) | (1<<14);// ���ö����� ʹ��Ԥ��Ƶ Ԥ��Ƶֵ49 
	
	//rADCCON  = (1<<0) |  (49<<6) | (1<<14);   // // �����ֶ����� ʹ��Ԥ��Ƶ Ԥ��Ƶֵ49 
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
	
	adcVal = rADCDAT0;  // ������ADCת��
	
	//rADCCON |= 0x1;       // �����ֶ�����	
	//while(rADCCON &0x1);
	
	while(1)
	{	
	
		//rADCCON |= 0x1;       // �����ֶ�����
		//while(rADCCON &0x1);
		
		while((rADCCON & (1<<15)) == 0);   // �ȴ�ת�����
		adcVal = rADCDAT0;   // ��ȡת����Ϻ������
		Uart0_Printf("adcVal= %d\n\r", adcVal&0x3ff);
		
		delay(0x1000000);
	}
	


}