# ============ 1. 工具链 ============
# 本地编译（在板子上练）用 gcc；
# 交叉编译（在 PC 上）用下面这行，编译时加 make CROSS=1 即可切换。
CROSS ?= 0
ifeq ($(CROSS),1)
CC      = aarch64-linux-gnu-gcc
else
CC      = gcc
endif

# ============ 2. 编译选项 ============
CFLAGS  = -Wall -Wextra -O2 -std=c11
LDFLAGS =

# ============ 3. 文件与目标名 ============
TARGET  = hello
SRCS    = main.c greet.c
OBJS    = $(SRCS:.c=.o)          # 把 .c 换成 .o：main.o greet.o

# ============ 4. 默认目标 ============
# 谁写在第一个，make 不传参数时就执行谁
all: $(TARGET)

# 链接：把所有 .o 合成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# 编译规则（模式规则）：任何 .c 生成同名 .o
# $< = 第一个依赖(main.c)，$@ = 目标(main.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 头文件改了，所有 .o 都要重编（简化写法）
$(OBJS): greet.h

# 清理
.PHONY: all clean
clean:
	rm -f $(OBJS) $(TARGET)

# 装到板子（PC 上交叉编译后执行）：make install BOARD=orangepi@172.16.5.150
.PHONY: install
install: $(TARGET)
	scp $(TARGET) $(BOARD):~/
