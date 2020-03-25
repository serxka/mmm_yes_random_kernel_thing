#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

void tty_init(void);
void tty_put(const char c);
void tty_write(const char *str, const int length);
void tty_writestring(const char *str);

#endif // _KERNEL_TTY_H