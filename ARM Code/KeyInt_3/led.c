#include "led.h"

/*���ܣ�led�Ƴ�ʼ��
������
����ֵ��
*/
void led_init(void){
	rGPBCON |= 0x15400;
	rGPBDAT |= 0x1e0;
	rGPBDAT &= ~0x1;
	return ;
}

/*���ܣ���led��
������
����ֵ��
*/
void led_open(void){
	rGPBDAT &= ~0x1e0;
	return ;
}

/*���ܣ��ر�led��
������
����ֵ��
*/
void led_close(void){
	rGPBDAT |= 0x1e0;
	return ;
}