
#include "2440addr.h"

/*
tclk = pclk / ((pre + 1) * div)

fout = 1kHz
pclk = 50MHz
pre = 15
div = 8

1/fout = count * (1/tclk)
1/fout = count * ((pre+1) * div) / pclk

count = pclk / (fout*(pre+1)* div)
	  = 50Mhz / (1KHz * 16 * 8)
	  = 390.625  = 391
*/

/*

*/
#define PCLK 50000000

void delay(unsigned int loops);
//led
void ledInit(void);
void ledON(void);
void ledOFF(void);
void ledBlink(void);

//key
void keyInit(void);

//PWM

void PWM_ON(void);
void PWM_OFF(void);

void ledInit(void)            //��ʼ��LED 
{  
	rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8);
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);
}

void ledON(void)
{
	rGPBDAT &= (~(1<<5)) & (~(1<<6)) & (~(1<<7)) & (~(1<<8));  //��
}

void ledOFF(void)
{
	 rGPBDAT |= (1<<5) | (1<<6) | (1<<7) | (1<<8); //��
}
void ledBlink(void)
{
	ledON();
	delay(0x05000000);
	ledOFF(); 
	delay(0x05000000);
}
void keyInit(void)
{
	//I/O��ʼ��,�����ⲿ�ж�
	rGPGCON |= (2<<0);
}
/*
void setFre(int fre)
{
	// ��ʱ������
	rTCFG0 |= (15<<0);   // pre = 15
	rTCFG1 |= (2<<0);    // div = 8
	
	rTCNTB0 =  PCLK / (fre * 16 * 8); 
	       
	rTCMPB0 = rTCNTB0 >> 1;   // ռ�ձ�50%
	
	rTCON |= (1<<3) | (1<<1) | (1<<0);  // �Զ����أ��ֶ����£�������ʱ����
	
	rTCON &= ~(1<<1);   //  �ر��ֶ�����
}	

void toutIoCfg(void)
{
	// ��������
	rGPBCON &= ~(3<<0);
	rGPBCON |= (2<<0);   // ����Ϊtout0����
}
*/
void PWM_ON(void)
{

	// ��������
	rGPBCON &= ~(3<<0);
	rGPBCON |= (2<<0);   // ����Ϊtout0����
	
	// ��ʱ������
	rTCFG0 |= (15<<0);   // pre = 15
	rTCFG1 |= (2<<0);    // div = 8
	
	rTCNTB0 = 391;        
	rTCMPB0 = rTCNTB0 >> 1;   // ռ�ձ�50%
	
	rTCON |= (1<<3) | (1<<1) | (1<<0);  // �Զ����أ��ֶ����£�������ʱ����
	
	rTCON &= ~(1<<1);   //  �ر��ֶ�����
	 
}

void PWM_OFF(void)
{
	rGPBCON &= ~(0x3);      //�ر�GPB0��TOUT0���ܣ�  
    rGPBCON |= 0x01;        //����GPB0Ϊ01�������  
    rGPBDAT &= 0x0;;        //����GPB0���ݼĴ���Ϊ0���Է�������˵1��������������0�����������
	
}

void delay(unsigned int loops)
{
	while(loops-->0);
}

void Main(void)
{
	ledInit();
	keyInit();
	PWM_ON();
	ledON();
	while(1)
	{
		//0-K1 3-K2 5-K3 6-K4
		if(!(rGPGDAT & (1<<3)))
		{
			delay(0x1000);
			if(!(rGPGDAT & (1<<3)))
			{
				PWM_OFF();
				ledOFF();
			}
		}	
	}
}