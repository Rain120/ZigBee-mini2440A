INTOFFSET			EQU  0x4a000014    ;Interruot request source offset
_ISR_STARTADDRESS	EQU  0x33ffff00
WTCON				EQU	 0x53000000

	area Init, code, readonly
	code32
	entry
	EXPORT __ENTRY
__ENTRY
	
	b start
	nop
	nop
	nop
	nop
	nop
	b irq_handler
	nop
	
start 
	;超级管理模式
	LDR		R0, =WTCON
  	MOV		R1, #0
  	STR		R1, [R0]
	
	ldr r0, =0x34000000   ;初始化堆栈
	mov sp, r0
	sub r0, r0, #0x500    ;栈大小为0x500
	
	;切换到IRQ模式
	mrs r1, cpsr
	bic r1, r1, #0x1f
	orr r1, r1, #0x12
	msr cpsr_c, r1
	
	mov sp, r0			 ;初始化sp_irq
	sub r0, r0, #0x1000   ;栈空间为0x500
	
	;切换到用户模式并且开中断
	mrs r1, cpsr
	bic r1, r1, #0x1f
	bic r1, r1, #0x80    ;开irq中断
	orr r1, r1, #0x1f    
	msr cpsr_c, r1
	
	mov sp, r0           ;初始化用户模式堆栈
	
	import main
	b main
	
	; 查一级表：可以作为所有IRQ入口
irq_handler
	sub sp, sp, #4
	stmfd sp!, {r8-r9}
	
	ldr		r9,=INTOFFSET
	ldr		r9,[r9]				;5
	ldr		r8,=HandleEINT0		;0x33FF_FF20
	add		r8,r8,r9,lsl #2		;R8 = r8+R9*4 = 0x33FF_FF20 + 0x14
	ldr		r8,[r8]				;Eint_Irq
	str		r8,[sp,#8]
	
	
	ldmfd sp!, {r8-r9, pc}
	
	
	
	
	AREA RamData, DATA, READWRITE

	^   _ISR_STARTADDRESS+0x20		; _ISR_STARTADDRESS=0x33FF_FF00

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
	
	end
