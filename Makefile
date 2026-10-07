CC := gcc
CFLAGS := -g -O0 -Wall -Wextra -Werror -std=c11
CPPFLAGS := -D_POSIX_C_SOURCE=200809L -Isrc -Isrc/core

SRC_DIR := src
TEST_DIR := tests
OUT_DIR := out

TARGET := $(OUT_DIR)/rgnx

SRCS := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(OUT_DIR)


