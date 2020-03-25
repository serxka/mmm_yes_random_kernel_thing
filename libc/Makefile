BINARY=libc.a
OBJS=\
stdio/printf.c.o \
stdio/putchar.c.o \
stdio/puts.c.o \
stdlib/itoa.c.o \
string/strlen.o \

CFLAGS?=-g -O0
CPPFLAGS?=

DESTDIR?=
SRCDIR=src

CFLAGS:=$(CFLAGS) -MD -c -ffreestanding -nostdlib -nostartfiles -mno-red-zone -fpic -fno-stack-protector
CPPFLAGS:=$(CPPFLAGS) -Iinclude

OBJS:=$(addprefix src/,$(OBJS))

.PHONY: all clean install install-headers install-lib

all: $(BINARY)

%.c.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(BINARY): $(OBJS)
	$(AR) rcs $@ $(OBJS)

clean:
	rm -f $(BINARY)
	rm -f $(OBJS)
	rm -f $(OBJS:.o=.d)

install: install-headers install-lib
	@echo -e "\033[0;34mBuilt $(BINARY)\033[0m"

install-headers: 
	@mkdir -p $(DESTDIR)/usr/include
	@cp -R include/* $(DESTDIR)/usr/include

install-lib: $(BINARY)
	@mkdir -p $(DESTDIR)/usr/lib
	@cp $(BINARY) $(DESTDIR)/usr/lib