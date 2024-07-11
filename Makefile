CC = "$(shell (which clang 2>/dev/null || which gcc 2>/dev/null) | head -1)"
PYTHON = "$(shell (which python3 2>/dev/null || which python 2>/dev/null) | head -1)"
O = 3

FLAGS := -pedantic -Wall -Wextra -Werror
FLAGS := $(FLAGS) -std=c11

ifeq ($(DEBUG),2)
	FLAGS := $(FLAGS) -gdwarf-4
else ifeq ($(DEBUG),1)
	FLAGS := $(FLAGS) -g
endif

ifeq ($(SAN),2)
	# address is implied by memory
	FLAGS := $(FLAGS) -fsanitize=memory,undefined
else ifeq ($(SAN),1)
	FLAGS := $(FLAGS) -fsanitize=address
endif

BASIC = src/basic/str.c src/basic/str_builder.c src/basic/read_line.c src/basic/str_to_int.c
SRC = src/main.c src/graph.c src/names.c

.PHONY: build
build:
	$(CC) -o main.out $(WARN) -O$(O) $(SRC) $(BASIC) $(FLAGS)


