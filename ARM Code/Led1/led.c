void delay(int loops);
#define rGPBCON  (*(unsigned int *)0x56000010)
#define rGPBDAT	 (*(unsigned int *)0x56000014)

void ledMain(void)
{
	//rGPBCON = 0x400;
	//rGPBDAT = 0xffffffdf;
	
	// 读 - 该 - 写
	//rGPBCON = rGPBCON  & (~(3<<5)) | (1<< 10);
	//rGPBCON = rGPBCON | (1<<10);
	// GPB5 GPB6 GPB7 GPB8 都配置为输出
	rGPBCON |= (1<<10) | (1<<12) | (1<<14) | (1<<16);  
	//rGPBDAT &= (~(1<<5))  & (~(1<<6)) & (~(1<<7)) & (~(1<<8)); 
	//rGPBDAT &= (~(0xf <<5));
	while(1)
	{
		// 亮
		rGPBDAT &= (~(1<<5))  & (~(1<<6)) & (~(1<<7)) & (~(1<<8));
		// 延时
		delay(0x5000000);
		// 灭
		rGPBDAT |= (1<<5) | (1<<6) |(1<<7) | (1<<8);
		// 延时
		delay(0x5000000);
	}
}

void delay(int loops)
{
	while(loops-->0);
}

/*
作业：流水灯（跑马灯）
*/
