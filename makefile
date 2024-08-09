src = $(wildcard src/*.c)
cflags = -O3 -Wextra -Werror -Wall

all: build run

build: 
	gcc $(src) -o main $(cflags)

run:
	./main