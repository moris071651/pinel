CC ?= cc
AR ?= ar

CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -O2 -Wall -Wextra -Wpedantic
ARFLAGS := rcs

LIB := libpinel.a

SRC := $(sort $(wildcard src/*.c))
OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

TEST_SRC := $(sort $(wildcard tests/test_*.c))
TEST_BIN := $(TEST_SRC:tests/%.c=build/tests/%)

SAN_FLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer -g

.DEFAULT_GOAL := all

.PHONY: all test asan clean

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build/tests/%: tests/%.c $(LIB) tests/test.h
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -L. -lpinel -o $@

test: $(TEST_BIN)
	@set -e; \
	for test in $(TEST_BIN); do \
		echo "==> $$test"; \
		./$$test; \
	done

asan:
	@mkdir -p build/asan
	@set -e; \
	for source in $(TEST_SRC); do \
		name=$$(basename $$source .c); \
		echo "==> asan: $$name"; \
		$(CC) $(CPPFLAGS) $(CFLAGS) $(SAN_FLAGS) $(SRC) $$source -o build/asan/$$name; \
		./build/asan/$$name; \
	done

clean:
	rm -rf build $(OBJ) $(DEP) $(LIB)

-include $(DEP)
