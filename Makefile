CC = clang
AR = ar

CFLAGS = -Wall -Wextra -std=c11 -Iinclude

PREFIX = /usr/local
INCLUDE_DIR = $(PREFIX)/include
LIB_DIR = $(PREFIX)/lib

all:
	mkdir -p build
	$(CC) $(CFLAGS) -c src/buffer/ring_buffer.c -o build/ring_buffer.o
	$(AR) rcs build/libfkae-shtae.a build/ring_buffer.o

test: all
	$(CC) $(CFLAGS) tests/ring_buffer_test.c build/libfkae-shtae.a -o build/ring_buffer_test
	./build/ring_buffer_test

install: all
	mkdir -p $(INCLUDE_DIR)/libfkae-shtae
	mkdir -p $(LIB_DIR)
	cp -R include/libfkae-shtae/* $(INCLUDE_DIR)/libfkae-shtae/
	cp build/libfkae-shtae.a $(LIB_DIR)/

uninstall:
	rm -rf $(INCLUDE_DIR)/libfkae-shtae
	rm -f $(LIB_DIR)/libfkae-shtae.a

clean:
	rm -rf build

.PHONY: all test install uninstall clean
