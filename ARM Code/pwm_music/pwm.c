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
 * 调用该函数， 可以控制蜂鸣器响起， 原理是使能了PWM的TOU1功能为动重装模式
 */
void beep_on( void )
{
	rTCON = (1<<0) | (1<<3);//clear manual update bit
}

/*
 * 调用该函数， 可以控制蜂鸣器响起， 原理是停止了PWM的TOU1功能
 */
void beep_off( void )
{
	rTCON &= ~1;
}


//音阶频率表
const unsigned int yinyue[3][7]
={
	262,  294,  330,  349,  392,  440,  494,  //低音
	523,  587,  659,  698,  784,  880,  988,  //中音
	1046, 1174, 1318, 1396, 1568, 1760, 1975  //高音
};

/*
 * 大海的简谱, 如果大海能够.....
 */
const unsigned char dahai[] =
{
	0x13,  0x15 , 0x16, 0x16, 0x16, 0x16, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,  //哀愁
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x12, 0x13,
	0x13,  0x12 , 0x11, 0x11, 0x11, 0x11, 0x21, 0x16, 0x15, 0x15, 0x16, 0x15,
	0x13,  0x15,  0x16, 0x21, 0x21, 0x16, 0x15, 0x15,//飘远
};

/*
 * 大海的简谱, 控制每一个音的时间 1为长时间 4为短时间
 */
const unsigned char time[] =
{
	4,	4,   4,     2,    4 ,   2,    4,     4,    4,    2,    4,    2 ,//哀愁
	4,      4,   4,     2,    4,    2,    2,     1,
	4,      4,   4,     2,    4,    2,    4,     4,     4,   2,    4,    2,
	4,      4,   2,      4,   2,     4,   4,     1  //飘远

};


//主函数
int Main(void)
{
	unsigned int loop = 0;   //循环控制变量
	unsigned int a = 0;      //计算高中低音使用的变量
	unsigned int b = 0;		 //计算do re mi ....使用的变量
	unsigned int delay = 0;  //计算每一个音延时使用的变量

	//GPH0.GPH0CON = (GPH0.GPH0CON) & ~(0x3f<<1); //初始化key1-key6的引脚功能为输入

	Buzzer_init();   //初始化PWM硬件， 此刻没有启动PWM
	//while( 	( GPH0.GPH0DAT & (0x3ful<<1) )  == (0x3ful<<1) );//等待有任意一个按键被按下,程序才会向下

	while(1)
	{
		//循环播放 音乐
		for(loop=0; loop<sizeof(dahai)/(sizeof(unsigned char)); loop++)
		{
			//查大海的简谱表
			a = dahai[loop]>>4;       		    //计算出高低音
			b = (dahai[loop] & 0x0f )-1;  	    //计算出DO RE MI FA SO LA XI
			delay = 0x6500000ul/time[loop];	    //计算出每个音的播放间隔时间

			Buzzer_Freq_Set( yinyue[a][b] ); //重新设定频率
			beep_on();			//发出一个音
			asmdelay(delay);  //播放延时

			beep_off();         //关闭蜂鸣器， 每个音播放完成后有间隔感
			asmdelay(0x20);   //关闭蜂鸣器
		}

		asmdelay(1000000);  //播放延时
	}

}


