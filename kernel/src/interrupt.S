.macro ISR num
	.global isr\num
	.global isr\num\()_handler
	isr\num\():
		pusha
		call isr\num\()_handler
		popa
		iret
.endm

.section .text
ISR 8
ISR 32
ISR 33
ISR 128