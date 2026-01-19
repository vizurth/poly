CC      := clang
OBJDUMP ?= objdump

# Флаги компиляции — отключаем PIC/PIE
CFLAGS  := -g -O0 --target=i386-apple-darwin -masm=intel -fno-pic -fno-pie
# Флаги линковки — отключаем PIE на уровне линкера
LDFLAGS := -Wl,-no_pie

SRCS     := 00.c
PROGS    := lab_00
ASMS     := 00.s
OBJS     := 00.o
OBJDUMPS := 00.objdump

.PHONY: all build asm obj objdump clean

all: build

build: $(PROGS)

lab_00: 00.s

# produce asm (.s) from .c
%.s: %.c
	$(CC) $(CFLAGS) -S $< -o $@

# produce object (.o) from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# create objdump with source interleaved
%.objdump: %.o
	$(OBJDUMP) -d -M intel -S $< > $@

asm: $(ASMS)
obj: $(OBJS)
objdump: $(OBJDUMPS)

clean:
	rm -f $(PROGS) $(ASMS) $(OBJS) $(OBJDUMPS)
