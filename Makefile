CC = gcc
.PHONY : clean

OBJ_FILES = obj/assertions.o obj/build.o obj/children.o obj/print.o
TARGETS = obj/confer.o $(OBJ_FILES)

CC_OPTIONS = -pedantic
CC_INCLUDE = -I./src
WARN = -Wall -Wextra
CFLAGS = $(CC_INCLUDE) $(CC_OPTIONS) $(WARN)

obj/confer.o: $(OBJ_FILES)
	@echo "⚙ $(OBJ_FILES) -> $@"
	ld -r $(OBJ_FILES) -o $@

obj/%.o: src/%.c
	@echo "⚙ $< -> $@"
	@$(CC) -I./src -c $< -o $@ $(CFLAGS)

bin/test: test/test.c $(OBJ_FILES) obj/confer.o
	@$(CC) "test/test.c" obj/confer.o $(OBJ_FILES) $(CFLAGS) -I./test -I./src -o "bin/test"

clean:
	rm ${TARGETS}
