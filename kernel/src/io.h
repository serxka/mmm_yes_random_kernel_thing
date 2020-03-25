#include <stdint.h>

static inline uint8_t inb (uint16_t p) {
	uint8_t d;
	asm volatile ("inb %1, %0" : "=a"(d) : "Nd"(p) );
	return d;
}

static inline void outb (uint16_t p, uint8_t d) {
  asm volatile ( "outb %0, %1" : : "a"(d), "Nd"(p) );
}

static inline void iowait  () {
	asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}