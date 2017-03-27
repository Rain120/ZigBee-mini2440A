		area Init,code, readonly
		code32
		entry
start
		ldr sp, =0x34000000
		import Main
		b Main
stop
		b stop

		end