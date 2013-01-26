CC = cc
CFLAGS = -g -Wall

LIB_H += set.h

all: part-1-run

part-1-run: z-notation
	./part-1-run

z-notation: pair.o set.o value.o
	$(CC) $(CFLAGS) -o part-1-run z-notation-parser.c set.o pair.o value.o

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

pair.o: pair.c
	$(CC) $(CFLAGS) -c pair.c

value.o: value.c
	$(CC) $(CFLAGS) -c value.c
	
clean:
	rm -v part-1-run *.o
