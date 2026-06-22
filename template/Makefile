# Host-side unit tests for the pure domain layer.
# Runs on your machine with the system compiler — no Flipper, no device.
#
#   make test     build & run domain tests
#   make clean    remove test binaries
#
# The fap itself is built with ufbt, not this Makefile.

CC = cc
CFLAGS = -Wall -Wextra -std=c11 -I.

.PHONY: test clean
test: test_counter
	./test_counter

test_counter: tests/test_counter.c src/domain/counter.c
	$(CC) $(CFLAGS) -o $@ tests/test_counter.c src/domain/counter.c

clean:
	rm -f test_counter
