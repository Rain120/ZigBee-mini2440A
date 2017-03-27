#include "2440addr.h"
#define PCLK	50000000

void Buzzer_init(void)
{
	rGPBCON &= ~3;			//set GPB0 as tout0, pwm output
	rGPBCON |= 2;
		
	rTCFG0 &= ~0xff;
	rTCFG0 |= 15;			//prescaler = 15+1
	rTCFG1 &= ~0xf;
	rTCFG1 |= 2;			//mux = 1/8
	rTCNTB0 = (PCLK>>7)/1000;
	rTCMPB0 = rTCNTB0>>1;	// 50%
	rTCON &= ~0x1f;
	rTCON |= 0xb;			//disable deadzone, auto-reload, inv-off, update TCNTB0&TCMPB0, start timer 0
	rTCON &= ~2;			//clear manual update bit
}
void Buzzer_Freq_Set( unsigned int freq )
{
	
	rTCNTB0 = (PCLK>>7)/freq;
	rTCMPB0 = rTCNTB0>>1;	// 50%
}

void Buzzer_Stop( void )
{
	rGPBCON &= ~3;			//set GPB0 as output
	rGPBCON |= 1;
	rGPBDAT &= ~1;
}

/*
 * ���øú����� ���Կ��Ʒ��������� ԭ����ʹ����PWM��TOU1����Ϊ����װģʽ
 */
void beep_on( void )
{
	rTCON = (1<<0) | (1<<3);//clear manual update bit
}

/*
 * ���øú����� ���Կ��Ʒ��������� ԭ����ֹͣ��PWM��TOU1����
 */
void beep_off( void )
{
	rTCON &= ~1;
}


//����Ƶ�ʱ�
const unsigned int musicalScore[4][7]={
	262,  294,  330,  349,  392,  440,  494,  //����
	523,  587,  659,  698,  784,  880,  988,  //����
	1046, 1175, 1318, 1397, 1568, 1760, 1967,  //����
	50000									  //0
};

const unsigned char ambulance[]={
	0x16,0x13,
	0x16,0x13,
	0x16,0x13,
	0x16,0x13
};

const unsigned char policeWagon[]={
	0x11,0x15,
	0x11,0x15,
	0x11,0x15,
	0x11,0x15
};

const unsigned char time[] ={
	2,2,
	2,2,
	2,2,
	2,2		
};
//ambulanceSounds
void ambulanceSounds()
{
	unsigned int loop = 0;   //ѭ�����Ʊ���
	unsigned int a = 0;      //������е���ʹ�õı���
	unsigned int b = 0;		 //����do re mi ....ʹ�õı���
	unsigned int delay = 0;  //����ÿһ������ʱʹ�õı���

	//GPH0.GPH0CON = (GPH0.GPH0CON) & ~(0x3f<<1); //��ʼ��key1-key6�����Ź���Ϊ����

	Buzzer_init();   //��ʼ��PWMӲ���� �˿�û������PWM
	//while( 	( GPH0.GPH0DAT & (0x3ful<<1) )  == (0x3ful<<1) );//�ȴ�������һ������������,����Ż�����

	while(1)
	{
		//ѭ������ ����
		for(loop=0; loop<sizeof(ambulance)/(sizeof(unsigned char)); loop++)
		{
			//����ױ�
			a = ambulance[loop]>>4;       		    //������ߵ���
			b = (ambulance[loop] & 0x0f )-1;  	    //�����DO RE MI FA SO LA XI
			delay = 0x6500000ul/time[loop];	    //�����ÿ�����Ĳ��ż��ʱ��

			Buzzer_Freq_Set( musicalScore[a][b] ); //�����趨Ƶ��
			beep_on();			//����һ����
			asmdelay(delay);  //������ʱ

			beep_off();         //�رշ������� ÿ����������ɺ��м����
			asmdelay(0x20);   //�رշ�����
		}

		asmdelay(10000);  //������ʱ
	}

}
//policeWagon
void policeWagonSounds()
{
	unsigned int loop = 0;   //ѭ�����Ʊ���
	unsigned int a = 0;      //������е���ʹ�õı���
	unsigned int b = 0;		 //����do re mi ....ʹ�õı���
	unsigned int delay = 0;  //����ÿһ������ʱʹ�õı���

	//GPH0.GPH0CON = (GPH0.GPH0CON) & ~(0x3f<<1); //��ʼ��key1-key6�����Ź���Ϊ����

	Buzzer_init();   //��ʼ��PWMӲ���� �˿�û������PWM
	//while( 	( GPH0.GPH0DAT & (0x3ful<<1) )  == (0x3ful<<1) );//�ȴ�������һ������������,����Ż�����

	while(1)
	{
		//ѭ������ ����
		for(loop=0; loop<sizeof(policeWagon)/(sizeof(unsigned char)); loop++)
		{
			//����ױ�
			a = policeWagon[loop]>>4;       		    //������ߵ���
			b = (policeWagon[loop] & 0x0f )-1;  	    //�����DO RE MI FA SO LA XI
			delay = 0x6500000ul/time[loop];	    //�����ÿ�����Ĳ��ż��ʱ��

			Buzzer_Freq_Set( musicalScore[a][b] ); //�����趨Ƶ��
			beep_on();			//����һ����
			asmdelay(delay);  //������ʱ

			beep_off();         //�رշ������� ÿ����������ɺ��м����
			asmdelay(0x20);   //�رշ�����
		}

		asmdelay(10000);  //������ʱ
	}

}
//������
int Main()
{
	//ambulanceSounds();
	policeWagonSounds();
	return 0;
}
