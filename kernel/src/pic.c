#include "io.h"
#include "pic.h"
#include <string.h>
#include <stddef.h>

struct pit_timer
{
	uint32_t timeleft;
	uint8_t complete;
};

static struct pit_timer timers[10];

static struct pit_timer* get_timer ()
{
	for (int i = 0; i < 10; ++i)
	{
		if (timers[i].timeleft == 0 && timers[i].complete == 0)
			return timers+i;
	}
	return NULL;
}

static void pit_init(void)
{
	memset(timers, 0, sizeof(struct pit_timer) * 10);
	// 1000hz
	uint16_t divisor = 1193182 / 1193;
	// set the config
	outb(0x43, 0x36);
	// set the divisor
	outb(0x40, divisor & 0xFF);
	outb(0x40, divisor >> 8);
	// enable pit
	pic_clearmask(0);
}

void isr32_handler(void)
{
	pic_eoi;
	for (int i = 0; i < 10; ++i)
	{
		if (timers[i].timeleft > 0 && --timers[i].timeleft == 0)
			timers[i].complete = 1;
	}
}

int32_t ksleep (uint32_t ms)
{
	struct pit_timer *t = get_timer();
	if (t == NULL)
		return -1;

	t->timeleft = ms;

	while(!t->complete) asm volatile ("");

	t->timeleft = 0;
	t->complete = 0;

	return ms;
}

void pic_init(void)
{
	// remap the PIC
	outb(PIC1_CMD, 0x11);
	outb(PIC2_CMD, 0x11);
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);
	outb(PIC1_DATA, 0x04);
	outb(PIC2_DATA, 0x02);
	outb(PIC1_DATA, 0x01);
	outb(PIC2_DATA, 0x01);
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);

	// init the PIT
	pit_init();
}

void pic_setmask (uint8_t irqline)
{
	uint16_t port;
	uint8_t mask;
	
	if (irqline < 8)
		port = PIC1_DATA;
	else {
		port = PIC2_DATA;
		irqline -= 8; }

	mask = inb(port) | (1 << irqline);
	outb(port, mask);
}

void pic_clearmask (uint8_t irqline)
{
	uint16_t port;
	uint8_t mask;
	
	if (irqline < 8)
		port = PIC1_DATA;
	else {
		port = PIC2_DATA;
		irqline -= 8; }

	mask = inb(port) & ~(1 << irqline);
	outb(port, mask);
}