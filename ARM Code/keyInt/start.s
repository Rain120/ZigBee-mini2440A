		area Init, code, readonly
		code32
		entry
		EXPORT	__ENTRY
__ENTRY
start
		b	resetHandler   ;0
		nop				   ;4
		nop
		nop
		nop
		nop
		b	irqHandler
		nop                ;0x1c
resetHandler
		; 初始化堆栈
		
		ldr r0, =0x34000000
		mov sp, r0
		sub r0, r0, #0x300
		
		
		
		; 切换到irq模式
		mrs r1, cpsr
		bic r1, r1, #0x1f
		orr r1, r1, #0x12
		msr cpsr_c, r1
		mov sp, r0
		sub r0, r0, #0x800 
		
		ldr r0, =0x56000010
		ldr r1, =0x15400
		str r1, [r0]
		
		ldr r0, =0x56000014
		ldr r1, =0
		str r1, [r0]
		
		;切换到用户模式，并且使能中断
		mrs r1, cpsr
		bic r1, r1, #0x1f
		orr r1, r1, #0x10
		bic r1, r1, #0x80
		msr cpsr_c, r1
		mov sp, r0
		
		import Main
		b  Main
		
stop
		b stop
		
		
		
		
irqHandler
		;现场保护
		stmfd sp!, {r0-r12, lr}
		
		; 执行中断服务
		ldr r0, =0x56000010
		ldr r1, =0x15400
		str r1, [r0]
		
		ldr r0, =0x56000014
		ldr r1, =0
		str r1, [r0]
		
		;现场恢复
		ldmfd sp!, {r0-r12, lr}
		subs pc, lr, #4   ; 中断返回
				
		end