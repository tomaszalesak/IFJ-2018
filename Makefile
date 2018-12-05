CFLAGS = -Wall -std=c99 -pedantic -lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
PROGRAMS = ifj2018

all: ifj2018

ifj2018: $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@

%.o: %.c
	gcc $(CFLAGS) -c $^ -o $@

test: all
	./tests/colorful_script_no_input.sh

nocolortest: all
	./tests/script_no_input.sh

exampletest: all
	./tests/test_just_example.sh

schooltest: all
	./tests/test_from_school.sh

clean:
	rm *.o $(PROGRAMS)
