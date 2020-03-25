#include <stdio.h>
#include <tty.h>
#include "dts.h"

void kmain (void)
{
	tty_init();
	dts_init();
	printf("Hello kernel!\n");
	asm volatile ("int $0x80");
	while (1)
		asm volatile ("");
}