src = $(wildcard src/*.c)
cflags = -Isrc/memdebug -O3 -Wextra -Werror -Wall

all: build run

build: 
	gcc $(src) src/memdebug/memdebug.c -o main $(cflags)

run:
	./main