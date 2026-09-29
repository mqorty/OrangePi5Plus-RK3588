CROSS ?= 0
ifeq ($(CROSS),1)
CC = aarch64-linux-gnu-gcc
else
CC = gcc
endif

CFLAGS = -Wall -Wextra -O2 -std=c11

# 所有要编译的程序列表
PROGS := cat cp mytouch filesize cpwithcheck

all: $(PROGS)

# 模式规则：任何 % 目标，依赖 %.c，编译命令统一写一次
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all clean
clean:
	rm -f $(PROGS)

