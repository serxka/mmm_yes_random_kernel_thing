#ifndef _KERNEL_PIC_H
#define _KERNEL_PIC_H

#include <stdint.h>

#define PIC1_CMD 0x20
#define PIC2_CMD 0xA0
#define PIC1_DATA 0x21
#define PIC2_DATA 0xA1

#define pic_eoi outb(PIC1_CMD, 0x20)

void pic_init(void);

void pic_setmask(uint8_t irqline);
void pic_clearmask(uint8_t irqline);

int32_t ksleep(uint32_t ms);

#endif // _KERNEL_PIC_H