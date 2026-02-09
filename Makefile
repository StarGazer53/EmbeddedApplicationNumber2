CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -Iinclude
LDFLAGS=

TARGET=release1_app

SRC=src/main.c src/io.c src/stubs.c src/app.c
OBJ=$(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
