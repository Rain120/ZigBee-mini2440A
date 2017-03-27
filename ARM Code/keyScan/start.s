		area Init, code, readonly
		code32
		entry
		EXPORT	__ENTRY
__ENTRY
start
		b	resetHandler
		nop
		nop
		nop
		nop
		nop
		b	IRQHandler
		nop
		
		
resetHandler
		ldr r0, =0x34000000
		mov sp,  r0
		sub r0, r0, #0x300
		
		;切换IRQ模式
		mrs r1, cpsr
		bic r1, r1, #0x1f
		orr r1, r1, #0x12
		msr cpsr_c, r1
		
		mov sp, r0
		sub r0, r0, #0x800
				
		;切换模式,并使能中断
		mrs r1, cpsr
		bic r1, r1, #0x1f
		bic r1, r1, #0x80
		orr r1, r1, #0x1f
		msr cpsr_c, r1
		mov sp, r0
		
		
		import keyMain
		b keyMain
		

stop
		b stop
		
IRQHandler
		stmfd sp!, {r0-r12,lr}
		
		; 执行中断服务
		
		import ledMain
		b	ledMain
		
		;ldr r0, =0x56000010
		;ldr r1, =0x15400
		;str r1, [r0]
		
		;ldr r0, =0x56000014
		;ldr r1, =0
		;str r1, [r0]
		
		ldmfd sp!, {r0-r12,lr}
		subs pc, lr, #4
		
		 
		end