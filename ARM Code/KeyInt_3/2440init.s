	GET 2440addr.inc

_STACK_BASEADDRESS	EQU 0x33ff8000
_ISR_STARTADDRESS	EQU 0x33ffff00

;预定义常量
USERMODE    EQU 	0x10
FIQMODE     EQU 	0x11
IRQMODE     EQU 	0x12
SVCMODE     EQU 	0x13
ABORTMODE   EQU 	0x17
UNDEFMODE   EQU 	0x1b
MODEMASK    EQU 	0x1f
NOINT       EQU 	0xc0


;堆栈位置
UserStack	EQU	(_STACK_BASEADDRESS-0x3800)	;0x33ff4800 ~
SVCStack	EQU	(_STACK_BASEADDRESS-0x2800)	;0x33ff5800 ~
UndefStack	EQU	(_STACK_BASEADDRESS-0x2400)	;0x33ff5c00 ~
AbortStack	EQU	(_STACK_BASEADDRESS-0x2000)	;0x33ff6000 ~
IRQStack	EQU	(_STACK_BASEADDRESS-0x1000)	;0x33ff7000 ~
FIQStack	EQU	(_STACK_BASEADDRESS-0x0)	;0x33ff8000 ~


 		MACRO
$HandlerLabel HANDLER $HandleLabel

$HandlerLabel
	sub		sp,sp,#4			;decrement sp(to store jump address)
	stmfd	sp!,{r0}			;PUSH the work register to stack(lr does not push because it return to original address)
	ldr     r0,=$HandleLabel	;load the address of HandleXXX to r0
	ldr     r0,[r0]				;load the contents(service routine start address) of HandleXXX
	str     r0,[sp,#4]      	;store the contents(ISR) of HandleXXX to stack
	ldmfd   sp!,{r0,pc}     	;POP the work register and pc(jump to ISR)
	MEND
	
	


	IMPORT  Main    ; The main entry of mon program

	AREA    Init,CODE,READONLY

	ENTRY
	
	EXPORT	__ENTRY
__ENTRY
ResetEntry
	;1)The code, which converts to Big-endian, should be in little endian code.
	;2)The following little endian code will be compiled in Big-Endian mode.
	;  The code byte order should be changed as the memory bus width.
	;3)The pseudo instruction,DCD can not be used here because the linker generates error.
	
 	b	ResetHandler
	b	HandlerUndef	;handler for Undefined mode
	b	HandlerSWI		;handler for SWI interrupt
	b	HandlerPabort	;handler for PAbort
	b	HandlerDabort	;handler for DAbort
	b	.				;reserved
	b	HandlerIRQ		;handler for IRQ interrupt
	b	HandlerFIQ		;handler for FIQ interrupt

;@0x20
;	b	EnterPWDN	; Must be @0x20.

	
HandlerFIQ      HANDLER HandleFIQ
HandlerIRQ      HANDLER HandleIRQ
HandlerUndef    HANDLER HandleUndef
HandlerSWI      HANDLER HandleSWI
HandlerDabort   HANDLER HandleDabort
HandlerPabort   HANDLER HandlePabort

IsrIRQ
	sub		sp,sp,#4       		;reserved for PC
	stmfd	sp!,{r8-r9}

	ldr		r9,=INTOFFSET
	ldr		r9,[r9]
	ldr		r8,=HandleEINT0
	add		r8,r8,r9,lsl #2
	ldr		r8,[r8]
	str		r8,[sp,#8]
	ldmfd	sp!,{r8-r9,pc}


	LTORG

;=======
; ENTRY
;=======
ResetHandler
	ldr		r0,=WTCON       ;watch dog disable
	ldr		r1,=0x0
	str		r1,[r0]

	ldr		r0,=INTMSK
	ldr		r1,=0xffffffff  ;all interrupt disable
	str		r1,[r0]

	ldr		r0,=INTSUBMSK
	ldr		r1,=0x7fff		;all sub interrupt disable
	str		r1,[r0]


	bl		InitStacks
	
	

;===========================================================
  	; Setup IRQ handler
	ldr		r0,=HandleIRQ       ;This routine is needed
	ldr		r1,=IsrIRQ	  		;if there is not 'subs pc,lr,#4' at 0x18, 0x1c
	str		r1,[r0]



 	bl		Main	;Do not use main() because ......
 	b	.


;初始化堆栈函数
InitStacks
	;Do not use DRAM,such as stmfd,ldmfd......
	;SVCstack is initialized before
	;Under toolkit ver 2.5, 'msr cpsr,r1' can be used instead of 'msr cpsr_cxsf,r1'
	mrs		r0,cpsr
	bic		r0,r0,#MODEMASK
	orr		r1,r0,#UNDEFMODE|NOINT
	msr		cpsr_cxsf,r1		;UndefMode
	ldr		sp,=UndefStack		; UndefStack=0x33FF_5C00

	orr		r1,r0,#ABORTMODE|NOINT
	msr		cpsr_cxsf,r1		;AbortMode
	ldr		sp,=AbortStack		; AbortStack=0x33FF_6000

	orr		r1,r0,#IRQMODE|NOINT
	msr		cpsr_cxsf,r1		;IRQMode
	ldr		sp,=IRQStack		; IRQStack=0x33FF_7000

	orr		r1,r0,#FIQMODE|NOINT
	msr		cpsr_cxsf,r1		;FIQMode
	ldr		sp,=FIQStack		; FIQStack=0x33FF_8000

	bic		r0,r0,#MODEMASK|NOINT
	orr		r1,r0,#SVCMODE
	msr		cpsr_cxsf,r1		;SVCMode
	ldr		sp,=SVCStack		; SVCStack=0x33FF_5800

	;USER mode has not be initialized.

	mov	pc,lr
	;The LR register will not be valid if the current mode is not SVC mode.
	
;===========================================================

	ALIGN

	AREA RamData, DATA, READWRITE

	^   _ISR_STARTADDRESS		; _ISR_STARTADDRESS=0x33FF_FF00
HandleReset 	#   4
HandleUndef 	#   4
HandleSWI		#   4
HandlePabort    #   4
HandleDabort    #   4
HandleReserved  #   4
HandleIRQ		#   4
HandleFIQ		#   4

;Do not use the label 'IntVectorTable',
;The value of IntVectorTable is different with the address you think it may be.
;IntVectorTable
;@0x33FF_FF20
HandleEINT0		#   4
HandleEINT1		#   4
HandleEINT2		#   4
HandleEINT3		#   4
HandleEINT4_7	#   4
HandleEINT8_23	#   4
HandleCAM		#   4		; Added for 2440.
HandleBATFLT	#   4
HandleTICK		#   4
HandleWDT		#   4
HandleTIMER0 	#   4
HandleTIMER1 	#   4
HandleTIMER2 	#   4
HandleTIMER3 	#   4
HandleTIMER4 	#   4
HandleUART2  	#   4
;@0x33FF_FF60
HandleLCD 		#   4
HandleDMA0		#   4
HandleDMA1		#   4
HandleDMA2		#   4
HandleDMA3		#   4
HandleMMC		#   4
HandleSPI0		#   4
HandleUART1		#   4
HandleNFCON		#   4		; Added for 2440.
HandleUSBD		#   4
HandleUSBH		#   4
HandleIIC		#   4
HandleUART0 	#   4
HandleSPI1 		#   4
HandleRTC 		#   4
HandleADC 		#   4
;@0x33FF_FFA0
	END
