#include <stdint.h>
#include <stddef.h>
#include "io.h"
#include <tty.h>

enum vga_colour {
	VGA_COLOUR_BLACK = 0,
	VGA_COLOUR_BLUE = 1,
	VGA_COLOUR_GREEN = 2,
	VGA_COLOUR_CYAN = 3,
	VGA_COLOUR_RED = 4,
	VGA_COLOUR_MAGENTA = 5,
	VGA_COLOUR_BROWN = 6,
	VGA_COLOUR_LIGHT_GREY = 7,
	VGA_COLOUR_DARK_GREY = 8,
	VGA_COLOUR_LIGHT_BLUE = 9,
	VGA_COLOUR_LIGHT_GREEN = 10,
	VGA_COLOUR_LIGHT_CYAN = 11,
	VGA_COLOUR_LIGHT_RED = 12,
	VGA_COLOUR_PINK = 13,
	VGA_COLOUR_YELLOW = 14,
	VGA_COLOUR_WHITE = 15,
};

static inline uint8_t vga_entry_colour(enum vga_colour fg, enum vga_colour bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t colour) {
	return (uint16_t) uc | (uint16_t) colour << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t vga_row = 0;
static size_t vga_column = 0;
static uint8_t vga_color = 4;
static uint16_t* vga_buffer = (uint16_t*) 0xB8000;

static inline void vga_putat(uint16_t entry, size_t x, size_t y)
{ vga_buffer[y * VGA_WIDTH + x] = entry; }

static inline size_t vga_readat(size_t x, size_t y)
{ return vga_buffer[y * VGA_WIDTH + x]; }

static inline void vga_clearline(size_t row)
{
	const uint16_t entry = vga_entry(' ', vga_color);
	for (size_t x = 0; x < VGA_WIDTH; ++x)
		vga_putat(entry, x, row);
}

static inline void vga_setcursor(size_t x, size_t y)
{
	const size_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static inline void vga_scroll(void)
{
	for (size_t y = 1; y < VGA_HEIGHT; ++y)
	{
		for (size_t x = 0; x < VGA_WIDTH; ++x)
		{
			const uint16_t entry = vga_readat(x, y);
			vga_putat(entry, x, y-1);
		}
	}
	vga_clearline(VGA_HEIGHT-1);
	--vga_row;
	vga_setcursor(vga_column, vga_row);
}

void tty_init(void)
{
	const uint16_t entry = vga_entry(' ', vga_color);
	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; ++i)
		vga_buffer[i] = entry;
}

void tty_put(const char c)
{
	switch (c)
	{
		case '\n':
			vga_column = 0;
			if (++vga_row == VGA_HEIGHT)
				vga_scroll();
			break;

		case '\t':
			tty_writestring("    ");
			break;

		case '\r':
			vga_column = 0;
			break;

		case '\b':
			vga_putat(vga_entry(' ', vga_color), vga_column, vga_row);
			if (vga_column == 0)
			{
				vga_column = VGA_WIDTH-1;
				if (vga_row != 0)
					--vga_row;
			} else
				--vga_column;
			break;
			
		default:
			vga_putat(vga_entry(c, vga_color), vga_column, vga_row);
			if (++vga_column == VGA_WIDTH)
			{
				vga_column = 0;
				if (++vga_row == VGA_HEIGHT)
					vga_scroll();
			}
			break;
	}
}

void tty_updatecursor(void)
{ vga_setcursor(vga_column, vga_row); }

void tty_write(const char *str, const int length)
{
	for (size_t i = 0; i < length; ++i)
		tty_put(str[i]);
	vga_setcursor(vga_column, vga_row);
}

void tty_writestring(const char *str)
{
	size_t i = 0;
	while (str[i])
		tty_put(str[i++]);
	vga_setcursor(vga_column, vga_row);
}