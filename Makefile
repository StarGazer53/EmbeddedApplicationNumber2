CC=gcc
# Build-time overrides (examples):
#   make CFLAGS_EXTRA='-DUSE_FPGA_IO=1'
#   make CFLAGS_EXTRA='-DUSE_FPGA_IO=1 -DLOOP_DELAY_MS=50'
CFLAGS=-Wall -Wextra -std=c99 -Iinclude $(CFLAGS_EXTRA)
LDFLAGS=

TARGET=release2_app

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