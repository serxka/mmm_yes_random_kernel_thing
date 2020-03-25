#include <stdio.h>

void isr8_handler(void)
{
	printf("double fault\n");
}

void isr128_handler(void)
{
	printf("syscall\n");
}