CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude -Ilib/cJSON -Igraphviz
TARGET  = akinator
SRC_DIR = src

# === САНИТАЙЗЕРЫ ===
CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer

# Автоматически собираем все .c файлы из src/
SRCS    = $(wildcard $(SRC_DIR)/*.c)

# Добавляем cJSON вручную
SRCS   += lib/cJSON/cJSON.c

# Объектные файлы
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

# Линковка
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Компиляция .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all run clean