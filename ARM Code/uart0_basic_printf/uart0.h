#ifndef _UART0_H_
#define	_UART0_H_

#define		rGPBCON		*((volatile unsigned int*)0x56000010)
#define		rGPBDAT		*((volatile unsigned int*)0x56000014)

#define		rULCON0		*((volatile unsigned int*)0x50000000)
#define		rUCON0		*((volatile unsigned int*)0x50000004)
#define		rUFCON0		*((volatile unsigned int*)0x50000008)

#define		rUTRSTAT0	*((volatile unsigned int*)0x50000010)
#define		rUTXH0		*((volatile unsigned int*)0x50000020)
#define		rURXH0		*((volatile unsigned int*)0x50000024)

#define		rUBRDIV0	*((volatile unsigned int*)0x50000028)

#define		rGPHCON		*((volatile unsigned int*)0x56000070)


void uart0_init(void);
void uart0_sendchar(unsigned char ch);
void uart0_sendstring(unsigned char *str);
unsigned char uart0_recv(void);
void Uart0_Printf(char *fmt,...);


#endif