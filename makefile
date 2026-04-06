CC = gcc
CFLAGS = -Wall -pedantic -g 

fast_parser: src/main.c src/parser.c src/io_core.c src/compute.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o fast_parser