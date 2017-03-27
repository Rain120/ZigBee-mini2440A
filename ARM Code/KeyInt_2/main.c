#include "2440addr.h"
#include "led.h"
#include "mmu.h"

#define _ISR_STARTADDRESS 	(0x33ffff00+0x20)
#define pISR_TICK2	(*(unsigned *)(_ISR_STARTADDRESS + 20))
void __irq Eint0_isq(void);

/*���ܣ��жϳ�ʼ��
������
����ֵ��
*/
void irq_init(void){
	rEXTINT1 = 0x3;
	rEINTMASK = 0x0;
	rINTMSK &= ~0x20;
	
	rGPGCON |= (0x1 << 1);
	rEINTPEND = 0x1 << 8;
	
	rSRCPND = 0x1 << 5;
	rINTPND = 0x1 << 5;
	pISR_TICK2 = (unsigned int)Eint0_isq;
}




/*���ܣ��رվ���
������
����ֵ��
__irq:����ʱ����ָ�cpsr��ֵ��
*/

unsigned int uiFlag = 0;
void __irq Eint0_isq(void){
	rEINTPEND = rEINTPEND;
	rSRCPND = rSRCPND;
	rINTPND = rINTPND;
	if (uiFlag == 0)
	{
		led_close();
		uiFlag = 1;
	}
	else 
	{
		led_open();
		uiFlag = 0;
	}

}

int main(void)
{

	MMU_Init();
	led_init();
	led_open();
	irq_init();
	
	
	while(1);
	
	return 0;
}