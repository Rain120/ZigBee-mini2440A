#include "led.h"

/*功能：led灯初始化
参数：
返回值：
*/
void led_init(void){
	rGPBCON |= 0x15400;
	rGPBDAT |= 0x1e0;
	rGPBDAT &= ~0x1;
	return ;
}

/*功能：打开led灯
参数：
返回值：
*/
void led_open(void){
	rGPBDAT &= ~0x1e0;
	return ;
}

/*功能：关闭led灯
参数：
返回值：
*/
void led_close(void){
	rGPBDAT |= 0x1e0;
	return ;
}