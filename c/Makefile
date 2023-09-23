CC=gcc
CFLAGS=-Wall -g

calc: src/calc.c
	${CC} ${CFLAGS} -c -o bin/calc.o src/calc.c

main: calc src/main.c
	${CC} ${CFLAGS} -o bin/main src/main.c bin/calc.o

test: calc src/test.c
	${CC} ${CFLAGS} -o bin/test src/test.c bin/calc.o
	bin/test

clean:
	rm -rf bin
	mkdir bin

