#include "2440addr.h"

void keyInit(){
	//I/O��ʼ��,�����ⲿ�ж�
	rGPGCON |= (2<<0);
}

// ����led
void ledon1(void)
{
	rGPBDAT &= (~(1<<5));
}

void eintInit(){
	keyInit();
	//�жϿ�����
	rINTMSK &= (~(1<<5));
	//�ⲿ�жϳ�ʼ��
	//�½��ش���
	rEXTINT1 |= (2<<0);
	//ʹ��eint8
	rEINTMASK &= ~(1<<8);
	
	//��Դ�Ĵ���
	rSRCPND |= (1<<5);
	rEINTPEND |= (1<<8);
}

void keyMain(){ 
	ledInit();
	ledon1();
	eintInit();
	//while(1);
}