__start:
	load #$5F, $0
	sout #$FF, $0
	sout r1, __start
	sout #$1, #0
