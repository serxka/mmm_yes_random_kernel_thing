#include <stdio.h>
#include <kernel/tty.h>

int putchar(int c)
{
	unsigned char ch = (unsigned char)c;
	tty_put(ch);
	tty_updatecursor();
	return ch;
}