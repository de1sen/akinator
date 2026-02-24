CC      = gcc
CFLAGS  = -Wall -Wextra -g
TARGET  = akinator
INC_DIR = include

all: $(TARGET)

$(TARGET): src/main.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -o $(TARGET) src/main.c

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean