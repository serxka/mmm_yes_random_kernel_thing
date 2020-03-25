#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <kernel/tty.h>

int printf(const char* __restrict format, ...)
{
	va_list args;
	va_start(args, format);

	size_t written = 0;

	while(*format != '\0')
	{
		if (format[0] != '%' || format[1] == '%')
		{
			if(format[0] == '%')
				++format;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			tty_write(format, amount);
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		switch (*format)
		{
			case 'x': {
				++format;
				int num = (int) va_arg(args, int);
				char buffer[33];
				size_t len = strlen(itoa (num, buffer, 16));
				tty_write(buffer, len);
				written += len;
				break; }
			case 'd': {
				++format;
				int num = (int) va_arg(args, int);
				char buffer[33];
				size_t len = strlen(itoa (num, buffer, 10));
				tty_write(buffer, len);
				written += len;
				break; }
			case 'c': {
				++format;
				char c = (char) va_arg(args, int);
				tty_put(c);
				++written;
				break; }
			case 's': {
				++format;
				const char* str = va_arg(args, const char*);
				size_t len = strlen(str);
				tty_write(str, len);
				written += len;
				break; }
			default: {
				format = format_begun_at;
				size_t len = strlen(format);
				tty_write(format, len);
				written += len;
				format += len;
				break; }
		}
	}
	tty_updatecursor();

	va_end(args);
	return written;
}