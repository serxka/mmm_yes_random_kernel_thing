#include <stdio.h>
#include <kernel/tty.h>

int puts(const char* __restrict string)
{
	tty_writestring(string);
	tty_put('\n');
	return 0;
}