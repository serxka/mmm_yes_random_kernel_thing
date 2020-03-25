#ifndef _LIBC_STDIO_H
#define _LIBC_STDIO_H

#define EOF (-1)

int putchar(int);
int puts(const char* __restrict);
int printf(const char* __restrict, ...);

#endif // _LIBC_STDIO_H