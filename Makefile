CC := gcc
CFLAGS := -g -O0 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -std=c11
CPPFLAGS := -D_POSIX_C_SOURCE=200809L -Isrc -Isrc/core -Isrc/event
LDFLAGS := -fsanitize=address

SRC := $(shell find src -name '*.c')
MAIN_SRC := src/core/rgnx.c
LIB_SRC := $(filter-out $(MAIN_SRC),$(SRC))

OBJ := $(SRC:src/%.c=out/%.o)
LIB_OBJ := $(LIB_SRC:src/%.c=out/%.o)

TARGET := out/rgnx

TEST_SRC := $(shell find tests -name '*.c' 2>/dev/null)
TEST_OBJ := $(TEST_SRC:tests/%.c=out/tests/%.o)
TEST_BIN := $(TEST_SRC:tests/%.c=out/tests/%)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

out/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

out/tests/%: out/tests/%.o $(LIB_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

out/tests/%.o: tests/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	@set -e; for test in $(TEST_BIN); do \
		echo "Running $$test"; \
		./$$test; \
	done

clean:
	rm -rf out

.PHONY: all test clean

