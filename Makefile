CC = cc
CFLAGS = -g -Wall

LIB_PATH=lib
INT_LIBS=$(LIB_PATH)/cJSON.c
LIBS=-lm

all: part-4-run

part-4-run: part-4
	./part-4-run

part-4: part-4.c pair.o set.o value.o variable.o
	$(CC) $(CFLAGS) part-4.c $(INT_LIBS) set.o pair.o value.o variable.o $(LIBS) -o part-4-run

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
