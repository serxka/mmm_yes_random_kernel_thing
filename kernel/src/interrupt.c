#include <stdio.h>

void isr8_handler(void)
{
	printf("double fault\n");
}

void isr32_handler(void)
{
	printf("timer irq\n");
}

void isr128_handler(void)
{
	printf("syscall\n");
}