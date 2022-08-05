# AUTOMATICALLY GENERATED FILE. MODIFY WITH CAUTION.
CC = gcc
.PHONY : clean

OBJ_FILES = obj/plus.o obj/minus.o
TARGETS = $(OBJ_FILES)

CC_OPTIONS = -pedantic
CC_INCLUDE = -I./src
WARN = -Wall -Wextra
CFLAGS = $(CC_INCLUDE) $(CC_OPTIONS) $(WARN)

obj/%.o: src/%.c
	@echo "⚙ $< -> $@"
	@$(CC) -I. -I./src -c $< -o $@ $(CFLAGS)

bin/test: test/test.c $(OBJ_FILES)
	@$(CC) "test/test.c" ../../obj/confer.o $(OBJ_FILES) $(CFLAGS) -I../../src/ -o "bin/test"

clean:
	rm ${TARGETS}
