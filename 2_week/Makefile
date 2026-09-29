# 沿用第一周的 Makefile 结构
CROSS ?= 0
ifeq ($(CROSS),1)
CC = aarch64-linux-gnu-gcc
else
CC = gcc
endif

CFLAGS = -Wall -Wextra -O2 -std=c11

all: cat cp

cat: cat.c
	$(CC) $(CFLAGS) -o $@ $<

cp: cp.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
clean:
	rm -f cat cp
