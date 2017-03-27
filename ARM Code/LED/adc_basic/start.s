		area Init, code, readonly
		code32
		entry
start

		ldr sp, =0x34000000

		import adcMain
		b	adcMain

stop
		b	stop
		end