		area	resetsec, code, readonly
		code32
		entry
		import	Main
		
		b		start
		nop
		nop
		nop
		nop
		nop
		nop
		nop

start	
		ldr		sp, =0x34000000

		ldr		r2, =0x015550
		ldr		r1, =0x56000010
		str		r2, [r1]
    
		ldr		r2, =0x7ff
		ldr		r1, =0x56000018
		str		r2, [r1]
    
		ldr		r1, =0x56000014

    
led_loop    
		ldr		r2, =0x1e0
	    str		r2, [r1]
	    
	    bl		asmdelay
	    
	    ldr		r2, =0
	    str		r2, [r1]
	    
	    bl		asmdelay

	 	;b		led_loop
	 	
	 	b		Main
	 	
	 	export	asmdelay
asmdelay
		ldr		r7, =0x1000000
del_loop
		sub		r7, r7, #1
		cmp		r7, #0
		bne		del_loop
		mov		pc, lr
	
		end