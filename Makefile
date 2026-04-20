CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude
TARGET  = akinator
SRC_DIR = src
INC_DIR = include

# Все исходники
SRCS    = $(wildcard $(SRC_DIR)/*.c)
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

# Линковка всех объектных файлов
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Компиляция каждого .c в .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all run clean