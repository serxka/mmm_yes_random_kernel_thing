KERNEL_BIN?=kernel.elf
OBJS=\
kmain.c.o \
boot.S.o \
vga.c.o \
dts.c.o \
interrupt.S.o \
interrupt.c.o 

CFLAGS?=-g -O0
CPPFLAGS?=
LDFLAGS?=

DESTDIR?=
SRCDIR=src

CFLAGS:=$(CFLAGS) -MD -c -ffreestanding -nostdlib -nostartfiles -mno-red-zone -fpic -fno-stack-protector
CPPFLAGS:=$(CPPFLAGS) -Iinclude
LDFLAGS:=$(LDFLAGS) -T link.ld
LIBS:=$(DESTDIR)/usr/lib/libc.a

OBJS:=$(addprefix src/,$(OBJS))

.PHONY: all clean install install-headers install-kernel

all: $(KERNEL_BIN)

%.c.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

%.S.o: %.S
	$(AS) -o $@ $<

$(KERNEL_BIN): $(OBJS)
	$(LD) $(LDFLAGS) -o $(KERNEL_BIN) $(OBJS) $(LIBS)

clean:
	rm -f $(KERNEL_BIN)
	rm -f $(OBJS)
	rm -f $(OBJS:.o=.d)

install: install-headers install-kernel
	@echo -e "\033[0;34mBuilt $(KERNEL_BIN)\033[0m"

install-headers: 
	@mkdir -p $(DESTDIR)/usr/include/kernel
	@cp -R include/* $(DESTDIR)/usr/include/kernel

install-kernel: $(KERNEL_BIN)
	@mkdir -p $(DESTDIR)/boot
	@cp $(KERNEL_BIN) $(DESTDIR)/boot