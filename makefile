CC = gcc
CFLAGS = -Wall -O3 -march=native

fast_parser: src/main.c src/parser.c src/io_core.c src/compute.c src/engine.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f src/*.o fast_parser