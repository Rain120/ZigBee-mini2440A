		area Init, code, readonly
		code32
		entry

		b		start
		nop
		nop
		nop
		nop
		nop
		nop
		nop
start		
		ldr sp, =0x34000000
		
		import Main
		b Main
		
stop
		b stop
		end