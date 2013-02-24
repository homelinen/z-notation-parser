CC = cc
CFLAGS = -g -Wall

LIB_PATH=lib
INT_LIBS=$(LIB_PATH)/cJSON.c
LIBS=-lm

all: part-1-run part-2-run

part-1-run: part-1
	./part-1-run

part-2-run: part-2
	./part-2-run

part-1: pair.o set.o value.o
	$(CC) $(CFLAGS) -o part-1-run part-1.c set.o pair.o value.o

part-2: part-2.c pair.o set.o value.o variable.o
	$(CC) $(CFLAGS) part-2.c $(INT_LIBS) set.o pair.o value.o variable.o $(LIBS) -o part-2-run

set.o: set.c
	$(CC) $(CFLAGS) -c set.c

pair.o: pair.c
	$(CC) $(CFLAGS) -c pair.c

value.o: value.c
	$(CC) $(CFLAGS) -c value.c
	
varible.o: variable.c
	$(CC) $(CFLAGS) -c variable.c

docs: part-1-doc part-2-doc

part-1-doc: doc/part-1.tex
	cd doc
	pdflatex part-1.tex
	cd

part-2-doc: doc/part-2.tex
	cd doc
	pdflatex part-2.tex
	cd

clean:
	rm -v part-1-run *.o
