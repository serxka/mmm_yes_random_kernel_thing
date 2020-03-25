#include "dts.h"
#include "pic.h"
#include <string.h>

struct dt_ptr
{
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

static inline void gdt_set_entry(uint32_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
{
	gdt_entries[index].base_low = (base & 0xFFFF);
	gdt_entries[index].base_mid = (base >> 16) & 0xFF;
	gdt_entries[index].base_high = (base >> 24) & 0xFF;

	gdt_entries[index].limit_low = (limit & 0xFFFF);
	gdt_entries[index].granularity = (limit >> 16) & 0x0F;

	gdt_entries[index].granularity |= granularity & 0xF0;
	gdt_entries[index].access = access;
}

static inline void idt_set_entry(uint8_t index, uint32_t offset, uint16_t selector, uint8_t flags)
{
	idt_entries[index].base_low = offset & 0xFFFF;
	idt_entries[index].base_high = (offset & 0xFFFF0000) >> 16;
	idt_entries[index].selector = selector;
	idt_entries[index].flags = flags;
	idt_entries[index].zero = 0;
}

static void set_idt(void)
{
	struct dt_ptr ptr;
	ptr.limit = (sizeof(idt_entry_t) * 255) - 1;
	ptr.base = (uint32_t)&idt_entries;

	memset(idt_entries, 0, sizeof(idt_entry_t) * 255);
	
	pic_init();

	idt_set_entry(8, (uint32_t)isr8, 0x08, 0x8E);
	idt_set_entry(32, (uint32_t)isr32, 0x08, 0x8E);
	idt_set_entry(128, (uint32_t)isr128, 0x08, 0x8E);

	asm volatile ("lidt %0" : : "m"(ptr));
	asm volatile ("sti");
}

static void set_gdt(void)
{
	struct dt_ptr ptr;
	ptr.limit = (sizeof(gdt_entry_t) * 5) - 1;
	ptr.base = (uint32_t)&gdt_entries;

	gdt_set_entry(0, 0, 0, 0, 0);
	gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

	asm volatile ("lgdt %0" : : "m"(ptr));	
}

void dts_init(void)
{
	set_gdt();
	set_idt();
}