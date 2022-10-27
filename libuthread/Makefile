# Target library
lib:= libuthread.a

all: $(lib)

## TODO: Phase 1
libuthread.a: queue.o
	gcc -Wall -Wextra -Werror -o libuthread.a queue.o
queue.o: queue.c queue.h
	gcc -Wall -Wextra -Werror -c -o queue.o queue.c

clean:
	rm -f libuthread.a queue.o 
