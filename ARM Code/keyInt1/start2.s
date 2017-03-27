	AREA Init,CODE,READONLY
	ENTRY
	CODE32
	
	import	Main
	EXPORT	__ENTRY
__ENTRY
start
	b		reset_handler
	nop
	nop
	nop
	nop
	nop
	b		irq_handler
	nop
reset_handler	
		;svc
		ldr r0,=0x34000000
		mov	sp, r0
		sub r0, r0, #0x1000
	
		;irq
		mrs		r1, cpsr
		bic		r1, r1, #0x1f		
		orr		r1, r1, #0x12
		msr		cpsr_c, r1
	
		mov sp, r0
		sub r0, r0, #0x500
	
		mrs		r1, cpsr
		bic 	r1, r1, #0x1f		
		orr 	r1, r1, #0x1f
		bic 	r1, r1, #0x80   ; I=0
		msr		cpsr_c, r1
    	mov 	sp, r0
 
    	;mrs		r1, cpsr
		;bic		r1, r1, #0x1f	
		;bic     r1, r1, #0x80
		;orr		r1, r1, #0x13
		;msr		cpsr_c, r1
		
		
		
    	b		Main
stop	
		b		stop
		
	
irq_handler
		sub	lr, 	lr, #4
		stmfd 		sp!, {r0-r12, lr}
	
		;  中断业务功能 简单实现
		ldr			r1, =0x56000014
		ldr			r0, =0  
   		str			r0, [r1]
	
		ldmfd sp!, {r0-r12, pc}^
	
delay
		mov		r7, #0x1000000
del_loop
		sub		r7, r7, #1
		cmp		r7, #0
		bne		del_loop
		mov		pc, lr
		
	END