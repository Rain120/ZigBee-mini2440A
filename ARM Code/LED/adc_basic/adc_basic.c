#include "2440addr.h"
#include "uart0.h"

void adcInit(){
	rADCCON &= ~0xffff;
	rADCCON |= (1<<1) | (49<<6) | (1<<14);

}

int delay(int loops){
	while (loops-- > 0);
}

void adcMain(){
	
	int adcVal = 0;
	
	adcInit();
	uart0_init();
	
	adcVal = rADCDAT0;
	
	while(1){
		while(rADCCON & (1<<15) == 0);
		adcVal = rADCDAT0;
		Uart0_Printf("adcVal = %d\t",adcVal & 0x3ff);
		delay(0x10000000);
	}
	
}