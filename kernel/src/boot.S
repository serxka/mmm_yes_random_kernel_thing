# Declare constants for the multiboot header.
.set FLAGS,    0x0              # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# Reserve a section for the stack
.section .bss
.align 16
.skip 8192 # 8 KiB
stack_top:

.section .text
.global _start
.type _start, @function

_start:
	cli                   # block interupts
	mov $stack_top, %esp  # create the stack
	call kmain            # call the main kernel
	jmp loop              # if the main kernel exits then just loop

loop:
	hlt
	jmp loop
