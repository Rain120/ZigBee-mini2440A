#define rGPBCON    (*(volatile unsigned *)0x56000010)	//Port B control
#define rGPBDAT    (*(volatile unsigned *)0x56000014)	//Port B data
#define rGPBUP     (*(volatile unsigned *)0x56000018)	//Pull-up control B

#define rTCFG0  (*(volatile unsigned *)0x51000000)	//Timer 0 configuration
#define rTCFG1  (*(volatile unsigned *)0x51000004)	//Timer 1 configuration
#define rTCON   (*(volatile unsigned *)0x51000008)	//Timer control
#define rTCNTB0 (*(volatile unsigned *)0x5100000c)	//Timer count buffer 0
#define rTCMPB0 (*(volatile unsigned *)0x51000010)	//Timer compare buffer 0
#define rTCNTO0 (*(volatile unsigned *)0x51000014)	//Timer count observation 0

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
const unsigned int yinyue[3][7]
={
	262,  294,  330,  349,  392,  440,  494,  //����
	523,  587,  659,  698,  784,  880,  988,  //����
	1046, 1174, 1318, 1396, 1568, 1760, 1975  //����
};

/*
 * �󺣵ļ���, ������ܹ�.....
 */
const unsigned char dahai[] =
{
	0x13,  0x15 , 0x16, 0x16, 0x16, 0x16, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,  //����
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x12, 0x13,
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,
	0x13,  0x15,  0x16, 0x21, 0x21, 0x16, 0x15, 0x15,//ƮԶ
};

/*
 * �󺣵ļ���, ����ÿһ������ʱ�� 1Ϊ��ʱ�� 4Ϊ��ʱ��
 */
const unsigned char time[] =
{
	4,	4,   4,     2,    4 ,   2,    4,     4,    4,    2,    4,    2 ,//����
	4,      4,   4,     2,    4,    2,    2,     1,
	4,      4,   4,     2,    4,    2,    4,     4,     4,   2,    4,    2,
	4,      4,   2,      4,   2,     4,   4,     1  //ƮԶ

};


//������
int Main(void)
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
		for(loop=0; loop<sizeof(dahai)/(sizeof(unsigned char)); loop++)
		{
			//��󺣵ļ��ױ�
			a = dahai[loop]>>4;       		    //������ߵ���
			b = (dahai[loop] & 0x0f )-1;  	    //�����DO RE MI FA SO LA XI
			delay = 0x6500000ul/time[loop];	    //�����ÿ�����Ĳ��ż��ʱ��

			Buzzer_Freq_Set( yinyue[a][b] ); //�����趨Ƶ��
			beep_on();			//����һ����
			asmdelay(delay);  //������ʱ

			beep_off();         //�رշ������� ÿ����������ɺ��м����
			asmdelay(0x20);   //�رշ�����
		}

		asmdelay(1000000);  //������ʱ
	}

}


