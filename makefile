all: clean part-1-run

part-1-run:
	gcc -Wall -o part-1-run z-notation-parser.c

clean:
	rm -v part-1-run
