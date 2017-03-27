		area Init, code, readonly
		code32
		entry 
start
		ldr sp, =0x34000000
		
		import watchDogMain
		b	watchDogMain

stop
		b stop
		end