	AREA Init,CODE,READONLY
	ENTRY
	CODE32
	;IMPORT	Eint_Irq
	import	Main
	EXPORT	__ENTRY
__ENTRY
start
	b		reset_handler
	nop
	b		swi_handler
	nop
	nop
	nop
	b		irq_handler
	nop
reset_handler
	
		;ldr    r0 , =0x53000000
    	;mov    r1 , #0x0
   		;str    r1 , [r0]
   		
		ldr r0,=0x34000000
		mov	sp, r0
		sub r0, r0, #0x500
	
		mrs		r1, cpsr
		bic		r1, r1, #0x1f
		orr		r1, r1, #0x12
		msr		cpsr_c, r1
	
		mov sp, r0
		sub r0, r0, #0x500
	
		mrs		r1, cpsr
		bic 	r1, r1, #0x1f
		bic 	r1, r1, #0x80
		orr 	r1, r1, #0x1f
		msr		cpsr_c, r1
			
		mov		sp, r0
	
	    
    	b		Main
stop	
		b		stop
		
	
irq_handler
		sub		lr, lr, #4
		stmfd	sp!, {r0-r12, lr}
	
		ldr		r1, =0x56000014
		ldr		r0, =0
   		str		r0, [r1]
	
		;bl	Eint_Irq
			
		ldmfd sp!, {r0-r12, pc}^
		
		;import c_swi_handler
swi_handler
		stmfd	sp!, {r0-r12, lr}
		ldr		r0, [lr, #-4]
		bic		r0, r0, #0xff000000
		;bl		c_swi_handler
		ldmfd	sp!, {r0-r12, pc}^
		
	
	export	asmdelay	
asmdelay
		;mov		r7, #0x1000000
		mov		r7, r0
del_loop
		sub		r7, r7, #1
		cmp		r7, #0
		bne		del_loop
		mov		pc, lr
		
	END

