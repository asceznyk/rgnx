CC := gcc
CFLAGS := -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -std=c11
CPPFLAGS := -D_POSIX_C_SOURCE=200809L -Isrc -Isrc/core -Isrc/event
LDFLAGS := -fsanitize=address

SRC := $(shell find src -name '*.c')
OBJ := $(SRC:src/%.c=out/%.o)
TARGET := out/rgnx

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

out/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf out

.PHONY: all clean
