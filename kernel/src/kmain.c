#include <stdio.h>
#include <tty.h>

void kmain (void)
{
	tty_init();
	printf("Hello kernel!\n");
}