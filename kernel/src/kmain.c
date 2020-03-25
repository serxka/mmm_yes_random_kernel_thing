#include <stdio.h>
#include <tty.h>
#include "dts.h"
#include "pic.h"

void kmain (void)
{
	tty_init();
	dts_init();
	printf("Hello kernel!\n");
	asm volatile ("int $0x80");

	int timeslept = ksleep(1337);
	printf("slept for %dms\n", timeslept);

	while (1)
		asm volatile ("");
}