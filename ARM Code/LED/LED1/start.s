		area Init, code, readonly
		code32
		entry
		
start
		b		resetHandler
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		


resetHandler

		ldr sp, =0x34000000
		
		import ledMain
		b	ledMain
		
		ldr r0, =0x56000010    ;GPBCON
		ldr r1, =0x400
		str r1, [r0]
		
		ldr r0, =0x56000014    ;GPBDAT
		ldr r1, =0xffffffdf
		str r1, [r0]
		

stop
		b stop
		end