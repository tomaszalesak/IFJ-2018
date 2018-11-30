CFLAGS = -Wall -std=c99 -pedantic -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj2018

all: ifj2018

ifj2018: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

test:
	./tests/script.sh

clean:
	rm *.o $(PROGRAMS)
