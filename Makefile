CC = cc
CFLAGS = -g -Wall

LIB_PATH=lib
INT_LIBS=$(LIB_PATH)/cJSON.c
LIBS=-lm

all: part-1-run

part-1-run: z-notation
	./part-1-run

z-notation: pair.o set.o value.o
	$(CC) $(CFLAGS) -o part-1-run z-notation-parser.c set.o pair.o value.o

file-reader: pair.o set.o value.o variable.o
	$(CC) $(CFLAGS) file-reader.c $(INT_LIBS) set.o pair.o value.o variable.o $(LIBS) -o filereader

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

pair.o: pair.c
	$(CC) $(CFLAGS) -c pair.c

value.o: value.c
	$(CC) $(CFLAGS) -c value.c
	
varible.o: variable.c
	$(CC) $(CFLAGS) -c variable.c

clean:
	rm -v part-1-run *.o
