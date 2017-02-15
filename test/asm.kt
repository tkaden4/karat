__start:
	load #$5F, $0
	sout #$FF, L1
L1:
	sout r1, __start
	sout $1, #0
